#include "pch.h"
#include "ImageImporter.h"
#include <afxdlgs.h>
#include "SettingManager.h"
#include "ImgImportDlg.h"

void CImageImporter::Import(CWnd* parentWnd)
{
	CString strTitle = CSettingManager::GetInstance()->GetWaterMarkTitle();
	CString strModelPrefix = CSettingManager::GetInstance()->GetModelPrefix();
	CString strWorkContent = CSettingManager::GetInstance()->GetWorkContent();
	if (strTitle.IsEmpty() || strModelPrefix.IsEmpty() || strWorkContent.IsEmpty())
	{
		MessageBox(parentWnd->m_hWnd, L"先设置水印信息", L"提示", MB_OK);
		return;
	}

	CString strHint;
	strHint.Format(L"水印标题：%s, 型号前缀：%s, 施工内容：%s", strTitle, strModelPrefix, strWorkContent);
	if (MessageBox(parentWnd->m_hWnd, strHint, L"提示", MB_YESNO) == IDNO)
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
        MessageBox(parentWnd->m_hWnd, L"选择的文件夹没有图片", L"提示", MB_OK);
        return;
    }

    CImgImportDlg imgImportDlg;
    imgImportDlg.SetTitle(L"导入图片");
    imgImportDlg.SetMaxValue(images.size());
    imgImportDlg.SetCurrentValue(0);
    imgImportDlg.SetImages(images);
    imgImportDlg.DoModal();
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

            // Check if the file has an image extension
            if (fileName.length() > 4 &&
                (fileName.substr(fileName.length() - 4) == L".jpg" ||
                    fileName.substr(fileName.length() - 4) == L".jpeg" ||
                    fileName.substr(fileName.length() - 4) == L".png" ||
                    fileName.substr(fileName.length() - 4) == L".bmp"))
            {
                filePaths.push_back(filePath);
            }
        } 
        while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }

    return filePaths;
}