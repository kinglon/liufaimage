#include "pch.h"
#include "ImageManager.h"

CString CImageItem::GetDateTimeString()
{
	CTime time(m_nUnixTimeStamp);
	CString formattedDateTime = time.Format(_T("%Y-%m-%d %H:%M"));
	return formattedDateTime;
}

CString CImageItem::GetStatusString()
{
	CString strStatus;
	switch (m_nStatus)
	{
	case IMAGE_STATUS_NOT_PROCESS:
		strStatus = L"未处理";
		break;
	case IMAGE_STATUS_HAVE_PROBLEM:
		strStatus = L"有问题";
		break;
	case IMAGE_STATUS_NOT_PROBLEM:
		strStatus = L"没问题";
		break;
	default:
		break;
	}

	return strStatus;
}