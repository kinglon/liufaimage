#pragma once

#include "DBUtil.h"

// ͼƬ����״̬
#define IMAGE_STATUS_NOT_PROCESS  1   // δ����
#define IMAGE_STATUS_HAVE_PROBLEM  2  // �Ѵ���������
#define IMAGE_STATUS_NOT_PROBLEM  3  // �Ѵ���û����

class CImageItem
{
public:
	CString GetDateTimeString();

	CString GetStatusString();

public:
	// ͼƬΨһ��ʶ
	UINT m_id;

	CString m_strModel;

	// ͼƬ����ʱ�䣬Unix ʱ�������λ��
	time_t m_nUnixTimeStamp = 0;

	CString m_strFilePath;

	// ͼƬ״̬
	int m_nStatus = IMAGE_STATUS_NOT_PROCESS;

	// ��ע
	CString m_strRemark;

	// ˮӡ����
	CString m_strWaterMaskTitle;

	// ˮӡ�ϵĹ�������
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

	// strModelΪ�ձ�ʾȫ��ģ��
	// nYearΪ0��ʾ�������
	// nStatusΪ0��ʾ����״̬
	bool FindImage(const CString& strModel, int nYear, int nStatus, unsigned int nOffset, unsigned int nLimit, CArray<CImageItem>& images);

private:
	bool CreateTable();

	static bool __stdcall EnumRecord(void* context, const std::vector<COLUMN_ITEM>& recordVec);

	time_t GetUnixTime(int year);

	// ��ȡ�����������id
	void GetColumnItems(const CImageItem& image, std::vector<COLUMN_ITEM>& columnVec);

private:
	CDBUtil m_dbUtil;
};