// ProgressDlg.cpp: 实现文件
//

#include "pch.h"
#include "liufaimage2.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"


// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROGRESS, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSCTRL, m_progressCtrl);
	DDX_Control(pDX, IDC_HINTCTRL, m_hintCtrl);
}

void CProgressDlg::SetTitle(const CString& strTitle)
{
	m_strTitle = strTitle;
	if (m_hWnd != NULL)
	{
		SetWindowText(strTitle);
	}
}

void CProgressDlg::SetMaxValue(int nMaxValue)
{
	m_nMaxValue = nMaxValue;	
	if (m_hWnd != NULL)
	{
		m_progressCtrl.SetRange(0, nMaxValue);
		UpdateHintCtrl();
	}
}

void CProgressDlg::SetCurrentValue(int nCurrentValue)
{
	m_nCurrentValue = nCurrentValue;	
	if (m_hWnd != NULL)
	{
		m_progressCtrl.SetPos(nCurrentValue);
		UpdateHintCtrl();
	}
}

void CProgressDlg::UpdateHintCtrl()
{
	int low, upper;
	m_progressCtrl.GetRange(low, upper);
	CString hint;
	hint.Format(L"%d/%d", m_progressCtrl.GetPos(), upper);
	m_hintCtrl.SetWindowText(hint);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_strTitle.IsEmpty())
	{
		SetWindowText(m_strTitle);
	}

	m_progressCtrl.SetRange(0, m_nMaxValue);
	m_progressCtrl.SetPos(m_nCurrentValue);

	UpdateHintCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
