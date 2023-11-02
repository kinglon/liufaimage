// CompareDlg.cpp: 实现文件
//

#include "pch.h"
#include "liufaimage2.h"
#include "afxdialogex.h"
#include "CompareDlg.h"
#include <ctime>
#include <atlimage.h>


// CCompareDlg 对话框

IMPLEMENT_DYNAMIC(CCompareDlg, CDialogEx)

CCompareDlg::CCompareDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPARE, pParent)
{

}

CCompareDlg::~CCompareDlg()
{
	for (auto& handle : m_imageHandle)
	{
		DeleteObject(handle);
	}
	m_imageHandle.clear();
}

void CCompareDlg::SetImages(CArray<CImageItem>* images, int nCurrentIndex)
{
	m_images = images;
	m_currentIndex = nCurrentIndex;
}

void CCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODELCTRL1, m_modelCtrl1);
	DDX_Control(pDX, IDC_MODELCTRL2, m_modelCtrl2);
	DDX_Control(pDX, IDC_IMAGECTRL1, m_imageCtrl1);
	DDX_Control(pDX, IDC_IMAGECTRL2, m_imageCtrl2);
	DDX_Control(pDX, IDC_STATUSCTRL1, m_statusCtrl1);
	DDX_Control(pDX, IDC_STATUSCTRL2, m_statusCtrl2);
	DDX_Control(pDX, IDC_REMARKCTRL1, m_remarkCtrl1);
	DDX_Control(pDX, IDC_REMARKCTRL2, m_remarkCtrl2);
	DDX_Control(pDX, IDC_LASTPAGECTRL, m_lastPieceCtrl1);
	DDX_Control(pDX, IDC_NEXTPAGECTRL, m_nextPieceCtrl);
}

void CCompareDlg::ShowImage(int imageIndex)
{
	if (m_images == nullptr)
	{
		return;
	}

	if (imageIndex < 0 || imageIndex >= m_images->GetSize())
	{
		return;
	}

	// 获取今年的年份
	time_t currentTime = time(NULL);
	std::tm currentTm;
	localtime_s(&currentTm, &currentTime);
	int currentYear = currentTm.tm_year + 1900;  // Add 1900 to get the actual year

	// 获取当前图片的年份
	int currentImgYear = m_images->GetAt(imageIndex).m_nYear;

	if (currentImgYear == currentYear)
	{
		m_leftImageItem = m_images->GetAt(imageIndex);
		CArray<CImageItem> lastYearImages;
		CImageManager::GetInstance()->FindImage(m_leftImageItem.m_strModel, currentYear - 1,
			0, 0, 1, lastYearImages);
		if (lastYearImages.GetSize() > 0)
		{
			m_rightImageItem = lastYearImages.GetAt(0);
		}
		else
		{
			m_rightImageItem.m_strModel = L"";
		}
	}
	else
	{
		m_rightImageItem = m_images->GetAt(imageIndex);
		CArray<CImageItem> thisYearImages;
		CImageManager::GetInstance()->FindImage(m_rightImageItem.m_strModel, currentYear, 0,
			0, 1, thisYearImages);
		if (thisYearImages.GetSize() > 0)
		{
			m_leftImageItem = thisYearImages.GetAt(0);
		}
		else
		{
			m_leftImageItem.m_strModel = L"";
		}
	}

	ResetAllCtrl();

	if (!m_leftImageItem.m_strModel.IsEmpty())
	{
		CString strModel;
		strModel.Format(L"%d年  %s", m_leftImageItem.m_nYear, m_leftImageItem.m_strModel);
		m_modelCtrl1.SetWindowText(strModel);
		InitImageCtrl(&m_imageCtrl1, m_leftImageItem.m_strFilePath);
		m_statusCtrl1.SetCurSel(m_leftImageItem.m_nStatus - 1);
		m_remarkCtrl1.SetWindowText(m_leftImageItem.m_strRemark);
	}

	if (!m_rightImageItem.m_strModel.IsEmpty())
	{
		CString strModel;
		strModel.Format(L"%d年  %s", m_rightImageItem.m_nYear, m_rightImageItem.m_strModel);		
		m_modelCtrl2.SetWindowText(strModel);
		InitImageCtrl(&m_imageCtrl2, m_rightImageItem.m_strFilePath);
		m_statusCtrl2.SetCurSel(m_rightImageItem.m_nStatus - 1);
		m_remarkCtrl2.SetWindowText(m_rightImageItem.m_strRemark);
	}

	m_lastPieceCtrl1.EnableWindow(imageIndex > 0);
	m_nextPieceCtrl.EnableWindow(imageIndex < m_images->GetSize() - 1);
}

void CCompareDlg::ResetAllCtrl()
{
	m_modelCtrl1.SetWindowText(L"");
	m_modelCtrl2.SetWindowText(L"");
	m_imageCtrl1.SetBitmap(NULL);
	m_imageCtrl2.SetBitmap(NULL);
	m_statusCtrl1.SetCurSel(0);
	m_statusCtrl2.SetCurSel(0);
	m_remarkCtrl1.SetWindowText(L"");
	m_remarkCtrl2.SetWindowText(L"");

	for (auto& handle : m_imageHandle)
	{
		DeleteObject(handle);
	}
	m_imageHandle.clear();
}

void CCompareDlg::InitImageCtrl(CStatic* imageCtrl, const CString& imgFilePath)
{
	Gdiplus::Bitmap* originalImage = Gdiplus::Bitmap::FromFile(imgFilePath);
	if (originalImage == nullptr)
	{
		LOG_ERROR(L"failed to load image, path is %s", (LPCWSTR)imgFilePath);
		return;
	}

	int imageWidth = originalImage->GetWidth();
	int imageHeight = originalImage->GetHeight();

	CRect rect;
	imageCtrl->GetClientRect(rect);
	int controlWidth = rect.Width();
	int controlHeight = rect.Height();
	double widthRatio = controlWidth * 1.0 / imageWidth;
	double heightRatio = controlHeight * 1.0 / imageHeight;
	double aspectRatio = min(widthRatio, heightRatio);
	int displayWidth = (int)(imageWidth * aspectRatio);
	int displayHeight = (int)(imageHeight * aspectRatio);

	Gdiplus::Bitmap scaledImage(displayWidth, displayHeight, originalImage->GetPixelFormat());
	Gdiplus::Graphics graphics(&scaledImage);
	graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);
	graphics.DrawImage(originalImage, 0, 0, displayWidth, displayHeight);
	delete originalImage;
	originalImage = nullptr;

	HBITMAP imageHandle = NULL;
	Gdiplus::Status status = scaledImage.GetHBITMAP(Gdiplus::Color::Black, &imageHandle);
	if (status != Gdiplus::Ok)
	{
		LOG_ERROR(L"failed to call GetHBITMAP, error is %d", status);
		return;
	}
	imageCtrl->SetBitmap(imageHandle);
	m_imageHandle.push_back(imageHandle);
}


BEGIN_MESSAGE_MAP(CCompareDlg, CDialogEx)
	ON_STN_DBLCLK(IDC_IMAGECTRL1, &CCompareDlg::OnDblclkImagectrl1)
	ON_STN_DBLCLK(IDC_IMAGECTRL2, &CCompareDlg::OnDblclkImagectrl2)
	ON_BN_CLICKED(IDC_LASTPAGECTRL, &CCompareDlg::OnClickedLastpagectrl)
	ON_BN_CLICKED(IDC_NEXTPAGECTRL, &CCompareDlg::OnClickedNextpagectrl)
	ON_BN_CLICKED(IDC_SAVECTRL, &CCompareDlg::OnClickedSavectrl)
	ON_BN_CLICKED(IDC_SHOWORIGINIMAGE, &CCompareDlg::OnBnClickedShoworiginimage)
	ON_BN_CLICKED(IDC_SHOWMASKIMAGE, &CCompareDlg::OnBnClickedShowmaskimage)
END_MESSAGE_MAP()



BOOL CCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_statusCtrl1.AddString(L"未处理");
	m_statusCtrl1.AddString(L"有问题");
	m_statusCtrl1.AddString(L"没问题");
	m_statusCtrl1.SetCurSel(0);
	m_statusCtrl2.AddString(L"未处理");
	m_statusCtrl2.AddString(L"有问题");
	m_statusCtrl2.AddString(L"没问题");
	m_statusCtrl2.SetCurSel(0);

	ShowImage(m_currentIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCompareDlg::OnDblclkImagectrl1()
{
	if (!m_leftImageItem.m_strModel.IsEmpty())
	{
		HINSTANCE result = ShellExecute(NULL, L"open", m_leftImageItem.m_strFilePath, NULL, NULL, SW_SHOWNORMAL);
		if ((int)result <= 32)
		{
			LOG_ERROR(L"failed to open the image %s", (LPCWSTR)m_leftImageItem.m_strFilePath);
		}
	}
}


void CCompareDlg::OnDblclkImagectrl2()
{
	if (!m_rightImageItem.m_strModel.IsEmpty())
	{
		HINSTANCE result = ShellExecute(NULL, L"open", m_rightImageItem.m_strFilePath, NULL, NULL, SW_SHOWNORMAL);
		if ((int)result <= 32)
		{
			LOG_ERROR(L"failed to open the image %s", (LPCWSTR)m_rightImageItem.m_strFilePath);
		}
	}
}


void CCompareDlg::OnClickedLastpagectrl()
{
	if (m_currentIndex > 0)
	{
		m_currentIndex--;
		ShowImage(m_currentIndex);
	}
}


void CCompareDlg::OnClickedNextpagectrl()
{
	if (m_currentIndex < m_images->GetSize() - 1)
	{
		m_currentIndex++;
		ShowImage(m_currentIndex);
	}
}


void CCompareDlg::OnClickedSavectrl()
{
	if (!m_leftImageItem.m_strModel.IsEmpty())
	{		
		m_leftImageItem.m_nStatus = m_statusCtrl1.GetCurSel() + 1;
		m_remarkCtrl1.GetWindowText(m_leftImageItem.m_strRemark);
		CImageManager::GetInstance()->UpdateImage(m_leftImageItem);
		for (int i = 0; i < m_images->GetSize(); i++)
		{
			if (m_images->GetAt(i).m_id == m_leftImageItem.m_id)
			{
				m_images->GetAt(i) = m_leftImageItem;
				break;
			}
		}
	}

	if (!m_rightImageItem.m_strModel.IsEmpty())
	{		
		m_rightImageItem.m_nStatus = m_statusCtrl2.GetCurSel() + 1;
		m_remarkCtrl2.GetWindowText(m_rightImageItem.m_strRemark);
		CImageManager::GetInstance()->UpdateImage(m_rightImageItem);
		for (int i = 0; i < m_images->GetSize(); i++)
		{
			if (m_images->GetAt(i).m_id == m_rightImageItem.m_id)
			{
				m_images->GetAt(i) = m_rightImageItem;
				break;
			}
		}
	}

	MessageBox(L"保存成功", L"提示", MB_OK);
}


void CCompareDlg::OnBnClickedShoworiginimage()
{
	m_imageCtrl1.SetBitmap(NULL);
	m_imageCtrl2.SetBitmap(NULL);
	for (auto& handle : m_imageHandle)
	{
		DeleteObject(handle);
	}
	m_imageHandle.clear();

	if (!m_leftImageItem.m_strModel.IsEmpty())
	{
		InitImageCtrl(&m_imageCtrl1, m_leftImageItem.m_strOriginFilePath);
	}
	if (!m_rightImageItem.m_strModel.IsEmpty())
	{
		InitImageCtrl(&m_imageCtrl2, m_rightImageItem.m_strOriginFilePath);
	}
}


void CCompareDlg::OnBnClickedShowmaskimage()
{
	m_imageCtrl1.SetBitmap(NULL);
	m_imageCtrl2.SetBitmap(NULL);
	for (auto& handle : m_imageHandle)
	{
		DeleteObject(handle);
	}
	m_imageHandle.clear();

	if (!m_leftImageItem.m_strModel.IsEmpty())
	{
		InitImageCtrl(&m_imageCtrl1, m_leftImageItem.m_strFilePath);
	}
	if (!m_rightImageItem.m_strModel.IsEmpty())
	{
		InitImageCtrl(&m_imageCtrl2, m_rightImageItem.m_strFilePath);
	}
}
