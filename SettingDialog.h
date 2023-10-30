#pragma once
#include "afxdialogex.h"


// CSettingDialog 对话框

class CSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSettingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_titleCtrl;
	CEdit m_modelPrefixCtrl;
	CEdit m_workContentCtrl;
	virtual BOOL OnInitDialog();
};
