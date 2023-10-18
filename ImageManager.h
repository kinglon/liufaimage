#pragma once

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

	// Unix ʱ�������λ��
	time_t m_nUnixTimeStamp = 0;

	CString m_strFilePath;

	// ͼƬ״̬
	int m_nStatus = IMAGE_STATUS_NOT_PROCESS;

	// ��ע
	CString m_strRemark;
};

class CImageManager
{
public:
	static CImageManager* GetInstance();

	// strModelΪ�ձ�ʾȫ��ģ��
	// nYearΪ0��ʾ�������
	// nStatusΪ0��ʾ����״̬
	CArray<CImageItem> FindImage(const CString& strModel, int nYear,  int nStatus);

	CArray<CImageItem> FindAllImage();

	bool DeleteImage(UINT nImageId);

	bool UpdateImage(const CImageItem& image);
};