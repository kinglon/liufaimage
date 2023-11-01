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
	void SetImage(const CImageItem& image) { m_imageItem = image; }

	CImageItem GetImage() { return m_imageItem; }

private:
	void InitAllCtrl();

	void ShowImage();

private:
	CImageItem m_imageItem;

	HBITMAP m_imageHandle = NULL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_imageCtrl;
	afx_msg void OnBnClickedUpdateimagectrl();
	CEdit m_modelCtrl;
	CComboBox m_statusCtrl;
	CEdit m_remarkCtrl;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDblclkImagectrl();
};
