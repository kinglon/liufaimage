// SettingDialog.cpp: 实现文件
//

#include "pch.h"
#include "liufaimage2.h"
#include "afxdialogex.h"
#include "SettingDialog.h"


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
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialogEx)
END_MESSAGE_MAP()


// CSettingDialog 消息处理程序
