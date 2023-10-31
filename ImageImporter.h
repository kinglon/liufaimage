#pragma once

#include <vector>

class CImgImportDlg;
class CImageImporter
{
public:
	void Import(CWnd* parentWnd);

private:
	std::vector<std::wstring> GetImageFilePaths(const std::wstring& folderPath);
};
