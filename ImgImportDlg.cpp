#include "pch.h"
#include "ImgImportDlg.h"
#include "WaterMaskUtil.h"
#include "ImgFolderManager.h"
#include "SettingManager.h"
#include "ImageManager.h"
#include <gdiplus.h>
#include <ctime>
#include "Utility/ImCharset.h"
#include "TimeUtil.h"

using namespace Gdiplus;

#define WM_WATER_MASK_COMPLETE WM_USER+100

void CImgImportDlg::AddWaterMaskComplete(CString originImage, bool isSuccess, CString imageWithMask, CString model)
{
    std::tuple<CString, bool, CString, CString>* lpParam = new  std::tuple<CString, bool, CString, CString>();
    std::get<0>(*lpParam) = originImage;
    std::get<1>(*lpParam) = isSuccess;
    std::get<2>(*lpParam) = imageWithMask;
    std::get<3>(*lpParam) = model;
    PostMessage(WM_WATER_MASK_COMPLETE, 0, (LPARAM)lpParam);
}

void CImgImportDlg::OnAddWaterMaskComplete(CString originImage, bool isSuccess, CString imageWithMask, CString model)
{
    if (!isSuccess)
    {
        m_failedImages.push_back(originImage);        
    }
    else
    {
        time_t captureTime = GetImageCaptureTime((LPCWSTR)originImage);
        if (captureTime == 0)
        {
            m_failedImages.push_back(originImage);
        }
        else
        {
            std::tm timeInfo;
            localtime_s(&timeInfo , &captureTime);
            int year = timeInfo.tm_year + 1900;
            CString imageFilePath = CImgFolderManager::GetInstance()->AddImage(
                imageWithMask, std::to_wstring(year).c_str(), CSettingManager::GetInstance()->GetModelPrefix());

            CImageItem imageItem;
            imageItem.m_nStatus = IMAGE_STATUS_NOT_PROCESS;
            imageItem.m_nUnixTimeStamp = captureTime;
            imageItem.m_strFilePath = imageFilePath;
            imageItem.m_strModel = model;
            imageItem.m_strWaterMaskTitle = CSettingManager::GetInstance()->GetWaterMarkTitle();
            imageItem.m_strWorkContent = CSettingManager::GetInstance()->GetWorkContent();
            CImageManager::GetInstance()->AddImage(imageItem);
        }
    }

    SetCurrentValue(m_progressCtrl.GetPos() + 1);
    CheckIfFinish();    
}

void CImgImportDlg::CheckIfFinish()
{
    int low, upper;
    m_progressCtrl.GetRange(low, upper);
    if (m_progressCtrl.GetPos() == upper)
    {
        if (m_failedImages.empty())
        {
            MessageBox(L"导入图片完成", L"提示", MB_OK);
        }
        else
        {
            if (m_failedImages.size() == upper)
            {
                MessageBox(L"导入图片失败", L"提示", MB_OK);
            }
            else
            {
                MessageBox(L"部分图片导入失败", L"提示", MB_OK);
            }
        }
        PostMessage(WM_CLOSE);
    }
}

time_t CImgImportDlg::GetImageCaptureTime(const std::wstring& imagePath)
{
    time_t unixTime = 0;    
    
    // 从图片的metadata中获取
    std::wstring captureTime;
    Bitmap* bitmap = new Bitmap(imagePath.c_str(), FALSE);
    if (bitmap->GetLastStatus() == Ok)
    {        
        UINT propertySize = bitmap->GetPropertyItemSize(PropertyTagExifDTOrig);
        if (propertySize > 0)
        {
            PropertyItem* propertyItem = (PropertyItem*)new char[propertySize];
            Status status = bitmap->GetPropertyItem(PropertyTagExifDTOrig, propertySize, propertyItem);
            if (status != Ok)
            {
                LOG_ERROR(L"failed to get the property of date time orig, path is %s, error is %d",
                    imagePath.c_str(), status);
            }
            else
            {
                captureTime = CImCharset::AnsiToUnicode(
                    std::string((char*)propertyItem->value, propertyItem->length).c_str());
                LOG_DEBUG(L"the date time is %s in the image of %s", captureTime.c_str(), imagePath.c_str());
            }

            delete[](char*) propertyItem;
        }
        else
        {
            LOG_INFO(L"the image does not have the time of capturing, path is %s", imagePath.c_str());
        }
    }
    else
    {
        LOG_ERROR(L"failed to load the image, path is %s, error is %d", imagePath.c_str(), bitmap->GetLastStatus());
    }
    delete bitmap;
    bitmap = nullptr;

    if (!captureTime.empty())
    {
        std::tm tm = {};
        swscanf_s(captureTime.c_str(), L"%d:%d:%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        unixTime = std::mktime(&tm);
    }

    // metadata没有拍摄时间，就取文件的时间
    if (unixTime == 0)
    {        
        HANDLE hFile = CreateFile(imagePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            FILETIME creationTime, lastAccessTime, lastWriteTime;
            if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
            {
                // Convert FILETIME to ULARGE_INTEGER for arithmetic operations
                ULARGE_INTEGER createTime;
                createTime.LowPart = creationTime.dwLowDateTime;
                createTime.HighPart = creationTime.dwHighDateTime;

                // Convert 100-nanosecond intervals to seconds
                ULONGLONG createTimeSeconds = createTime.QuadPart / 10000000ULL;

                // Subtract the number of seconds between 1601-01-01 and 1970-01-01
                const ULONGLONG epochDiff = 11644473600ULL;
                unixTime = createTimeSeconds - epochDiff;
            }

            CloseHandle(hFile);
        }

        if (unixTime == 0)
        {
            LOG_ERROR(L"failed to get the file creation time, path is %s", imagePath.c_str());
        }
    }

    return unixTime;
}

DWORD __stdcall CImgImportDlg::AddWaterMaskProc(LPVOID lpParam)
{
    std::tuple<std::vector<std::wstring>, CImgImportDlg*>* pParamTuple = (std::tuple<std::vector<std::wstring>, CImgImportDlg*>*)lpParam;
    std::vector<std::wstring> images = std::get<0>(*pParamTuple);
    CImgImportDlg* imgImportDlg = std::get<1>(*pParamTuple);
    delete pParamTuple;
    pParamTuple = nullptr;

    for (const auto& image : images)
    {
        time_t captureTime = CImgImportDlg::GetImageCaptureTime(image.c_str());
        if (captureTime == 0)
        {
            imgImportDlg->AddWaterMaskComplete(image.c_str(), false, L"", L"");
            continue;
        }

        int year = CTimeUtil::GetYear(captureTime);
        CString model = CImgFolderManager::GetInstance()->GetNextModelName(year);
        CWaterMaskUtil waterMaskUtil;
        CString imageWithMask = waterMaskUtil.AddWaterMask(image.c_str(),
            CSettingManager::GetInstance()->GetWaterMarkTitle(), captureTime, model,
            CSettingManager::GetInstance()->GetWorkContent());
        imgImportDlg->AddWaterMaskComplete(image.c_str(), !imageWithMask.IsEmpty(),
            imageWithMask, model);
    }

    return 0;
}

void CImgImportDlg::BeginAddWaterMask()
{
    std::tuple<std::vector<std::wstring>, CImgImportDlg*>* pThreadParam = new  std::tuple<std::vector<std::wstring>, CImgImportDlg*>();
    std::get<0>(*pThreadParam) = m_images;
    std::get<1>(*pThreadParam) = this;
    HANDLE hThread = ::CreateThread(NULL, 0, &CImgImportDlg::AddWaterMaskProc, (LPVOID)pThreadParam, 0, NULL);
    if (hThread == NULL)
    {
        LOG_ERROR(L"failed to start the thread of adding watermask, error is %d", GetLastError());
    }
}

BOOL CImgImportDlg::OnInitDialog()
{
	CProgressDlg::OnInitDialog();

    BeginAddWaterMask();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CImgImportDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_WATER_MASK_COMPLETE)
    {
        std::tuple<CString, bool, CString, CString>* lpParam = (std::tuple<CString, bool, CString, CString>*)pMsg->lParam;
        OnAddWaterMaskComplete(
            std::get<0>(*lpParam),
            std::get<1>(*lpParam),
            std::get<2>(*lpParam),
            std::get<3>(*lpParam)
            );
        return TRUE;
    }

    return CProgressDlg::PreTranslateMessage(pMsg);
}
