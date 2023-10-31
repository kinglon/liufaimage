#pragma once
#include "afxdialogex.h"


// CProgressDlg 对话框

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProgressDlg();

public:
	void SetTitle(const CString& strTitle);

	void SetMaxValue(int nMaxValue);

	void SetCurrentValue(int nCurrentValue);

private:
	void UpdateHintCtrl();

private:
	CString m_strTitle;

	int m_nMaxValue = 1;

	int m_nCurrentValue = 0;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progressCtrl;
	CStatic m_hintCtrl;
	virtual BOOL OnInitDialog();
};
