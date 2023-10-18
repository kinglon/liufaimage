// EditDlg.cpp: 实现文件
//

#include "pch.h"
#include "liufaimage2.h"
#include "afxdialogex.h"
#include "EditDlg.h"


// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT, pParent)
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
END_MESSAGE_MAP()


// CEditDlg 消息处理程序
