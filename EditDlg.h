#pragma once
#include "afxdialogex.h"
#include "ImageManager.h"

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEditDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT };
#endif

public:
	void SetImage(const CImageItem& image);

	CImageItem GetImage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
