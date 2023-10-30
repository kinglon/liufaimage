#pragma once

class CImgFolderManager
{
public:
	static CImgFolderManager* GetInstance();

public:
	CString GetNextModelName();

	CString AddImage(const CString& imageFilePath, const CString& year, const CString& modelPrefix);

	bool ReplaceImage(const CString& srcImageFilePath, const CString& destImageFilePath);

	bool DeleteImage(const CString& imageFilePath);

private:
	CString GetImagePath(const CString& year, const CString& modelPrefix);
};
