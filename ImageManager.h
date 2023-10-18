#pragma once

// 图片处理状态
#define IMAGE_STATUS_NOT_PROCESS  1   // 未处理
#define IMAGE_STATUS_HAVE_PROBLEM  2  // 已处理，有问题
#define IMAGE_STATUS_NOT_PROBLEM  3  // 已处理，没问题

class CImageItem
{
public:
	CString GetDateTimeString();

	CString GetStatusString();

public:
	// 图片唯一标识
	UINT m_id;

	CString m_strModel;

	// Unix 时间戳，单位秒
	time_t m_nUnixTimeStamp = 0;

	CString m_strFilePath;

	// 图片状态
	int m_nStatus = IMAGE_STATUS_NOT_PROCESS;

	// 备注
	CString m_strRemark;
};

class CImageManager
{
public:
	static CImageManager* GetInstance();

	// strModel为空表示全部模型
	// nYear为0表示所有年份
	// nStatus为0表示所有状态
	CArray<CImageItem> FindImage(const CString& strModel, int nYear,  int nStatus);

	CArray<CImageItem> FindAllImage();

	bool DeleteImage(UINT nImageId);

	bool UpdateImage(const CImageItem& image);
};