#pragma once
#include "afxdialogex.h"
#include "ImageManager.h"

// CCompareDlg 对话框

class CCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareDlg)

public:
	CCompareDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCompareDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE };
#endif

public:
	void SetImages(CArray<CImageItem>* images, int nCurrentIndex);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
