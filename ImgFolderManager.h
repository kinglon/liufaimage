#pragma once

class CImgFolderManager
{
public:
	static CImgFolderManager* GetInstance();

public:
	CString GetNextModelName(int year, const std::string& modelPrefix);

	// ��Ӵ���ˮӡ��ͼƬ
	CString AddImage(const CString& imageFilePath, const CString& year, const CString& modelPrefix);

	// ���ԭͼƬ
	CString AddOriginImage(const CString& imageFilePath);

	bool ReplaceImage(const CString& srcImageFilePath, const CString& destImageFilePath);

	// ɾ������ˮӡ��ͼƬ
	bool DeleteImage(const CString& imageFilePath);

	// ɾ��ԭͼƬ
	bool DeleteOriginImage(const CString& imageFilePath);

private:
	static CString GetGuid();

private:
	CString GetImagePath(const CString& year, const CString& modelPrefix);
};
