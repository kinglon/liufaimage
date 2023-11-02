#pragma once

#include <vector>

class CImgImportDlg;
class CImageImporter
{
public:
	void ImportImageFiles(CWnd* parentWnd);

	void ImportImageFolder(CWnd* parentWnd);

private:
	bool CheckAndHint(CWnd* parentWnd);

	void DoImport(CWnd* parentWnd, const std::vector<std::wstring>& images);

private:
	std::vector<std::wstring> GetImageFilePaths(const std::wstring& folderPath);
};
