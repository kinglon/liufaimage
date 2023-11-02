#include "pch.h"
#include "ImageImporter.h"
#include <afxdlgs.h>
#include "SettingManager.h"
#include "ImgImportDlg.h"

bool CImageImporter::CheckAndHint(CWnd* parentWnd)
{
    CString strTitle = CSettingManager::GetInstance()->GetWaterMarkTitle();
    CString strModelPrefix = CSettingManager::GetInstance()->GetModelPrefix();
    CString strWorkContent = CSettingManager::GetInstance()->GetWorkContent();
    if (strTitle.IsEmpty() || strModelPrefix.IsEmpty() || strWorkContent.IsEmpty())
    {
        MessageBox(parentWnd->m_hWnd, L"������ˮӡ��Ϣ", L"��ʾ", MB_OK);
        return false;
    }

    CString strHint;
    strHint.Format(L"ˮӡ���⣺%s, �ͺ�ǰ׺��%s, ʩ�����ݣ�%s, ��ݣ�%d",
        strTitle, strModelPrefix, strWorkContent, CSettingManager::GetInstance()->GetYear());
    if (MessageBox(parentWnd->m_hWnd, strHint, L"��ʾ", MB_YESNO) == IDNO)
    {
        return false;
    }

    return true;
}

void CImageImporter::DoImport(CWnd* parentWnd, const std::vector<std::wstring>& images)
{
    CImgImportDlg imgImportDlg(parentWnd);
    imgImportDlg.SetTitle(L"����ͼƬ");
    imgImportDlg.SetMaxValue(images.size());
    imgImportDlg.SetCurrentValue(0);
    imgImportDlg.SetImages(images);
    imgImportDlg.DoModal();
}

void CImageImporter::ImportImageFiles(CWnd* parentWnd)
{
    if (!CheckAndHint(parentWnd))
    {
        return;
    }

    CFileDialog fileDialog(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, parentWnd, 0, TRUE);
    fileDialog.m_ofn.lpstrFilter = _T("Image Files (*.jpg;*.jpeg;*.png;*.bmp)\0*.jpg;*.jpeg;*.png;*.bmp\0");
    TCHAR szFileBuffer[MAX_PATH * 1024] = { 0 };
    fileDialog.m_ofn.lpstrFile = szFileBuffer;
    fileDialog.m_ofn.nMaxFile = sizeof(szFileBuffer) / sizeof(*szFileBuffer);
    if (fileDialog.DoModal() != IDOK)
    {
        return;
    }

    std::vector<std::wstring> images;
    POSITION pos = fileDialog.GetStartPosition();
    while (pos != NULL)
    {
        CString filePath = fileDialog.GetNextPathName(pos);
        DWORD attributes = GetFileAttributes(filePath);
        if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            images.push_back((LPCWSTR)filePath);
        }        
    }	
    if (images.empty())
    {
        MessageBox(parentWnd->m_hWnd, L"û��ѡ��ͼƬ", L"��ʾ", MB_OK);
        return;
    }

    DoImport(parentWnd, images);
}

void CImageImporter::ImportImageFolder(CWnd* parentWnd)
{
    if (!CheckAndHint(parentWnd))
    {
        return;
    }

    CFolderPickerDialog folderDlg(nullptr, OFN_PATHMUSTEXIST, parentWnd);
    if (folderDlg.DoModal() != IDOK)
    {
        return;
    }
    CString selectedFolderPath = folderDlg.GetFolderPath();
    std::vector<std::wstring> images = GetImageFilePaths((LPCWSTR)selectedFolderPath);
    if (images.empty())
    {
        MessageBox(parentWnd->m_hWnd, L"ѡ����ļ���û��ͼƬ", L"��ʾ", MB_OK);
        return;
    }

    DoImport(parentWnd, images);
}

std::vector<std::wstring> CImageImporter::GetImageFilePaths(const std::wstring& folderPath)
{
    std::vector<std::wstring> filePaths;
    WIN32_FIND_DATA findData;
    HANDLE hFind;

    // Create the search path by appending wildcard pattern
    std::wstring searchPath = folderPath + L"\\*";

    // Start finding files in the specified folder
    hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Skip directories
                continue;
            }

            std::wstring fileName = findData.cFileName;
            std::wstring filePath = folderPath + L"\\" + fileName;
            int index = fileName.rfind(L'.');
            if (index != -1)
            {
                std::wstring ext = fileName.substr(index + 1);
                if (ext == L"jpg" || ext == L"jpeg" || ext == L"png" || ext == L"bmp")
                {
                    filePaths.push_back(filePath);
                }
            }
        } 
        while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }

    return filePaths;
}