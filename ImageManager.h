#pragma once

#include "DBUtil.h"

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

	// 图片拍摄时间，Unix 时间戳，单位秒
	time_t m_nUnixTimeStamp = 0;

	CString m_strFilePath;

	// 图片状态
	int m_nStatus = IMAGE_STATUS_NOT_PROCESS;

	// 备注
	CString m_strRemark;

	// 水印标题
	CString m_strWaterMaskTitle;

	// 水印上的工作内容
	CString m_strWorkContent;
};

class CImageManager
{
protected:
	CImageManager();

public:
	static CImageManager* GetInstance();

	bool AddImage(const CImageItem& image);

	bool DeleteImage(UINT nImageId);

	bool UpdateImage(const CImageItem& image);

	// strModel为空表示全部模型
	// nYear为0表示所有年份
	// nStatus为0表示所有状态
	bool FindImage(const CString& strModel, int nYear, int nStatus, unsigned int nOffset, unsigned int nLimit, CArray<CImageItem>& images);

private:
	bool CreateTable();

	static bool __stdcall EnumRecord(void* context, const std::vector<COLUMN_ITEM>& recordVec);

	time_t GetUnixTime(int year);

	// 获取所有列项，除了id
	void GetColumnItems(const CImageItem& image, std::vector<COLUMN_ITEM>& columnVec);

private:
	CDBUtil m_dbUtil;
};