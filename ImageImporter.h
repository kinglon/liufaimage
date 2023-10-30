#pragma once

#include <vector>

class CImgImportDlg;
class CImageImporter
{
public:
	void Import(CWnd* parentWnd, const CString& strImagePath);

private:
	std::vector<std::wstring> GetImageFilePaths(const std::wstring& folderPath);
};
