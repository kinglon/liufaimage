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
};
