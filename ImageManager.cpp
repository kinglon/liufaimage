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
		strStatus = L"δ����";
		break;
	case IMAGE_STATUS_HAVE_PROBLEM:
		strStatus = L"������";
		break;
	case IMAGE_STATUS_NOT_PROBLEM:
		strStatus = L"û����";
		break;
	default:
		break;
	}

	return strStatus;
}