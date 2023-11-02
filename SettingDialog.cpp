// SettingDialog.cpp: 实现文件
//

#include "pch.h"
#include "liufaimage2.h"
#include "afxdialogex.h"
#include "SettingDialog.h"
#include "SettingManager.h"


// CSettingDialog 对话框

IMPLEMENT_DYNAMIC(CSettingDialog, CDialogEx)

CSettingDialog::CSettingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING, pParent)
{

}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WATERMARK_TITLE, m_titleCtrl);
	DDX_Control(pDX, IDC_MODEL_PREFIX, m_modelPrefixCtrl);
	DDX_Control(pDX, IDC_WORK_CONTENT, m_workContentCtrl);
	DDX_Control(pDX, IDC_YEARCTRL, m_yearCtrl);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSettingDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSettingDialog 消息处理程序


void CSettingDialog::OnBnClickedOk()
{
	CString strTitle;
	m_titleCtrl.GetWindowText(strTitle);
	CString strModelPrefix;
	m_modelPrefixCtrl.GetWindowText(strModelPrefix);
	CString strWorkContent;
	m_workContentCtrl.GetWindowText(strWorkContent);
	CString strYear;
	m_yearCtrl.GetWindowText(strYear);
	if (strTitle.IsEmpty() || strModelPrefix.IsEmpty() || strWorkContent.IsEmpty() || strYear.IsEmpty())
	{
		MessageBox(L"所有字段必填", L"提示", MB_OK);
		return;
	}
	

	CSettingManager::GetInstance()->SetWaterMarkTitle(strTitle);
	CSettingManager::GetInstance()->SetModelPrefix(strModelPrefix);
	CSettingManager::GetInstance()->SetWorkContent(strWorkContent);
	CSettingManager::GetInstance()->SetYear(_tstoi((LPCWSTR(strYear))));
	CSettingManager::GetInstance()->Save();

	CDialogEx::OnOK();
}


void CSettingDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_titleCtrl.SetWindowText(CSettingManager::GetInstance()->GetWaterMarkTitle());
	m_modelPrefixCtrl.SetWindowText(CSettingManager::GetInstance()->GetModelPrefix());
	m_workContentCtrl.SetWindowText(CSettingManager::GetInstance()->GetWorkContent());
	m_yearCtrl.SetWindowText(std::to_wstring(CSettingManager::GetInstance()->GetYear()).c_str());

	return TRUE;
}
