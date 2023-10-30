// EditDlg.cpp: 实现文件
//

#include "pch.h"
#include "liufaimage2.h"
#include "afxdialogex.h"
#include "EditDlg.h"
#include <atlimage.h>
#include "WaterMaskUtil.h"
#include "SettingManager.h"
#include "ImgFolderManager.h"

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
	DDX_Control(pDX, IDC_IMAGECTRL, m_imageCtrl);
	DDX_Control(pDX, IDC_MODELCTRL, m_modelCtrl);
	DDX_Control(pDX, IDC_STATUSCTRL, m_statusCtrl);
	DDX_Control(pDX, IDC_REMARKCTRL, m_remarkCtrl);
}

void CEditDlg::InitAllCtrl()
{
	ShowImage();
	m_modelCtrl.SetWindowText(m_imageItem.m_strModel);
	m_remarkCtrl.SetWindowText(m_imageItem.m_strRemark);	
	m_statusCtrl.AddString(L"未处理");
	m_statusCtrl.AddString(L"有问题");
	m_statusCtrl.AddString(L"没问题");
	m_statusCtrl.SetCurSel(m_imageItem.m_nStatus - 1);
}

void CEditDlg::ShowImage()
{
	CImage image;
	HRESULT hr = image.Load(m_imageItem.m_strFilePath);
	if (!SUCCEEDED(hr))
	{
		LOG_ERROR(L"failed to load image %s, error is %x", (LPCWSTR)m_imageItem.m_strFilePath, hr);
		return;
	}
	
	int imageWidth = image.GetWidth();
	int imageHeight = image.GetHeight();	
	
	CRect rect;
	m_imageCtrl.GetClientRect(rect);
	int controlWidth = rect.Width();
	int controlHeight = rect.Height();

	double widthRatio = controlWidth * 1.0 / imageWidth;
	double heightRatio = controlHeight * 1.0 / imageHeight;
	double aspectRatio = min(widthRatio, heightRatio);
	int displayWidth = (int)(imageWidth * aspectRatio);
	int displayHeight = (int)(imageHeight * aspectRatio);
	int xPos = (controlWidth - displayWidth) / 2;
	int yPos = (controlHeight - displayHeight) / 2;
	
	// Create a compatible device context
	CDC dc;
	dc.CreateCompatibleDC(NULL);

	// Create a CBitmap and select it into the device context
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, image.GetWidth(), image.GetHeight());
	CBitmap* pOldBitmap = dc.SelectObject(&bitmap);

	// Draw the image onto the bitmap
	image.BitBlt(dc.GetSafeHdc(), 0, 0);

	// Clean up
	dc.SelectObject(pOldBitmap);

	// Draw the scaled bitmap on the control
	CDC* pStaticDC = m_imageCtrl.GetDC();
	pStaticDC->StretchBlt(xPos, yPos, displayWidth, displayHeight, &dc, 0, 0, imageWidth, imageHeight, SRCCOPY);
	m_imageCtrl.ReleaseDC(pStaticDC);
}

BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_UPDATEIMAGECTRL, &CEditDlg::OnBnClickedUpdateimagectrl)
	ON_BN_CLICKED(IDCANCEL, &CEditDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
	ON_STN_DBLCLK(IDC_IMAGECTRL, &CEditDlg::OnDblclkImagectrl)
END_MESSAGE_MAP()


// CEditDlg 消息处理程序


BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitAllCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEditDlg::OnBnClickedUpdateimagectrl()
{
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("Image Files (*.bmp; *.jpg; *.jpeg; *.png)|*.bmp;*.jpg;*.jpeg;*.png||"), this);
	if (fileDialog.DoModal() == IDOK)
	{
		CString filePath = fileDialog.GetPathName();
		CWaterMaskUtil waterMaskUtil;
		CString imageFilePath = waterMaskUtil.AddWaterMask(filePath, m_imageItem.m_strWaterMaskTitle, 
			m_imageItem.m_nUnixTimeStamp, m_imageItem.m_strModel, m_imageItem.m_strWorkContent);
		CImgFolderManager::GetInstance()->ReplaceImage(imageFilePath, m_imageItem.m_strFilePath);
		ShowImage();
	}
}

void CEditDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CEditDlg::OnBnClickedOk()
{
	CString model;
	m_modelCtrl.GetWindowText(model);
	if (model.IsEmpty())
	{
		MessageBox(L"型号不能为空", L"提示", MB_OK);
		return;
	}

	m_imageItem.m_strModel = model;
	m_remarkCtrl.GetWindowText(m_imageItem.m_strRemark);
	m_imageItem.m_nStatus = m_statusCtrl.GetCurSel() + 1;
	CDialogEx::OnOK();
}


void CEditDlg::OnDblclkImagectrl()
{
	HINSTANCE result = ShellExecute(NULL, L"open", m_imageItem.m_strFilePath, NULL, NULL, SW_SHOWNORMAL);
	if ((int)result <= 32)
	{
		LOG_ERROR(L"failed to open the image %s", (LPCWSTR)m_imageItem.m_strFilePath);
	}
}
