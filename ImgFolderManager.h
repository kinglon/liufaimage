#pragma once

class CImgFolderManager
{
public:
	static CImgFolderManager* GetInstance();

public:
	CString GetNextModelName(int year, const std::string& modelPrefix);

	// 添加带有水印的图片
	CString AddImage(const CString& imageFilePath, const CString& year, const CString& modelPrefix);

	// 添加原图片
	CString AddOriginImage(const CString& imageFilePath);

	bool ReplaceImage(const CString& srcImageFilePath, const CString& destImageFilePath);

	// 删除带有水印的图片
	bool DeleteImage(const CString& imageFilePath);

	// 删除原图片
	bool DeleteOriginImage(const CString& imageFilePath);

private:
	static CString GetGuid();

private:
	CString GetImagePath(const CString& year, const CString& modelPrefix);
};
