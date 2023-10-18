#include "pch.h"
#include "framework.h"
#include "liufaimage2.h"
#include "liufaimage2Dlg.h"
#include "afxdialogex.h"
#include "SettingDialog.h"
#include "EditDlg.h"
#include "CompareDlg.h"
#include "ImageImporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 每页显示的行数
const int ITEMS_PER_PAGE = 10;

Cliufaimage2Dlg::Cliufaimage2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIUFAIMAGE2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cliufaimage2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS_LIST, m_statusComboBox);
	DDX_Control(pDX, IDC_PAGE_NUMBER, m_pageNumberCtrl);
	DDX_Control(pDX, IDC_LAST_PAGE, m_lastPageCtrl);
	DDX_Control(pDX, IDC_NEXT_PAGE, m_nextPageCtrl);
	DDX_Control(pDX, IDC_IMAGE_LIST, m_imageListCtrl);
	DDX_Control(pDX, IDC_MODEL_INPUT, m_modelInputCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_yearInputCtrl);
}

BEGIN_MESSAGE_MAP(Cliufaimage2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_SEARCH, &Cliufaimage2Dlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_LAST_PAGE, &Cliufaimage2Dlg::OnBnClickedLastPage)
	ON_BN_CLICKED(IDC_NEXT_PAGE, &Cliufaimage2Dlg::OnBnClickedNextPage)
	ON_BN_CLICKED(IDC_SETTING, &Cliufaimage2Dlg::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_EXPORT_IMAGE, &Cliufaimage2Dlg::OnBnClickedExportImage)
	ON_NOTIFY(NM_RCLICK, IDC_IMAGE_LIST, &Cliufaimage2Dlg::OnNMRClickImageList)
	ON_COMMAND(ID_IMAGE_EDIT, &Cliufaimage2Dlg::OnImageEdit)
	ON_COMMAND(ID_IMAGE_COMPARE, &Cliufaimage2Dlg::OnImageCompare)
	ON_COMMAND(ID_IMAGE_DELETE, &Cliufaimage2Dlg::OnImageDelete)
END_MESSAGE_MAP()

BOOL Cliufaimage2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitAllControl();

	return TRUE;
}

void Cliufaimage2Dlg::InitAllControl()
{
	// 初始化状态列表
	m_statusComboBox.AddString(L"全部");
	m_statusComboBox.AddString(L"未处理");
	m_statusComboBox.AddString(L"有问题");
	m_statusComboBox.AddString(L"没问题");
	m_statusComboBox.SetCurSel(0);

	// 初始化图片列表控件的头部
	m_imageListCtrl.InsertColumn(0, _T("型号"), LVCFMT_CENTER, 100);
	m_imageListCtrl.InsertColumn(1, _T("时间"), LVCFMT_CENTER, 150);
	m_imageListCtrl.InsertColumn(2, _T("状态"), LVCFMT_CENTER, 50);
	m_imageListCtrl.InsertColumn(3, _T("备注"), LVCFMT_CENTER, 250);
	m_imageListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 显示第一页
	m_CurrentImages.Copy(CImageManager::GetInstance()->FindAllImage());
	ShowPage(1);
}

void Cliufaimage2Dlg::ShowPage(int nPageNumber)
{
	if (nPageNumber < 1)
	{
		return;
	}

	// 计算总页数	
	int nPageCount = 0;
	if (m_CurrentImages.GetSize() > 0)
	{
		nPageCount = (m_CurrentImages.GetSize() - 1) / ITEMS_PER_PAGE + 1;
	}

	// 没有任何图片
	if (nPageCount == 0)
	{
		m_pageNumberCtrl.SetWindowText(L"0/0");
		m_lastPageCtrl.EnableWindow(FALSE);
		m_nextPageCtrl.EnableWindow(FALSE);
		m_imageListCtrl.DeleteAllItems();
		return;
	}

	if (nPageNumber > nPageCount)
	{
		nPageNumber = nPageCount;
	}

	// 初始翻页按钮、页码
	CString strPageNumber;
	strPageNumber.Format(L"%d/%d", nPageNumber, nPageCount);
	m_pageNumberCtrl.SetWindowText(strPageNumber);
	m_lastPageCtrl.EnableWindow(nPageNumber <= 1);
	m_nextPageCtrl.EnableWindow(nPageNumber >= nPageCount);

	// 插入数据
	m_imageListCtrl.DeleteAllItems();
	int nStartIndex = (nPageNumber - 1) * ITEMS_PER_PAGE;
	int nEndIndex = nPageNumber * ITEMS_PER_PAGE;
	if (nEndIndex >= m_CurrentImages.GetSize())
	{
		nEndIndex = m_CurrentImages.GetSize();
	}
	for (int i = nStartIndex, j=0; i < nEndIndex; i++, j++)
	{
		CImageItem& Item = m_CurrentImages[i];
		m_imageListCtrl.InsertItem(j, Item.m_strModel);
		m_imageListCtrl.SetItemText(j, 1, Item.GetDateTimeString());
		m_imageListCtrl.SetItemText(j, 2, Item.GetStatusString());
		m_imageListCtrl.SetItemText(j, 4, Item.m_strRemark);
	}

	m_nCurrentPageNumber = nPageNumber;
}

int Cliufaimage2Dlg::GetCurrentSelImageIndex()
{
	int nIndex = m_imageListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex == -1)
	{
		return -1;
	}
	int nImageIndex = (m_nCurrentPageNumber - 1) * ITEMS_PER_PAGE + nIndex;
	return nImageIndex;
}

void Cliufaimage2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cliufaimage2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cliufaimage2Dlg::OnBnClickedSearch()
{
	CString strModel;
	m_modelInputCtrl.GetWindowText(strModel);

	CString strYear;
	m_yearInputCtrl.GetWindowText(strYear);
	int nYear = _ttoi(strYear);

	int nStatus = m_statusComboBox.GetCurSel();

	m_CurrentImages.Copy(CImageManager::GetInstance()->FindImage(strModel, nYear, nStatus));
	ShowPage(1);
}


void Cliufaimage2Dlg::OnBnClickedLastPage()
{
	m_nCurrentPageNumber--;
	ShowPage(m_nCurrentPageNumber);
}


void Cliufaimage2Dlg::OnBnClickedNextPage()
{
	m_nCurrentPageNumber++;
	ShowPage(m_nCurrentPageNumber);
}

void Cliufaimage2Dlg::OnBnClickedSetting()
{
	CSettingDialog settingDlg;
	settingDlg.DoModal();
}


void Cliufaimage2Dlg::OnBnClickedExportImage()
{
	CFolderPickerDialog dlg(nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, this);
	if (dlg.DoModal() == IDOK)
	{
		CString selectedFolderPath = dlg.GetFolderPath();
		CImageImporter imgImporter;
		imgImporter.Import(this, selectedFolderPath);
	}
}

void Cliufaimage2Dlg::OnNMRClickImageList(NMHDR* pNMHDR, LRESULT* pResult)
{	
	*pResult = 0;

	CMenu menu;
	menu.LoadMenu(IDR_IMAGE_OPERATE);

	CMenu* pContextMenu = menu.GetSubMenu(0);
	if (pContextMenu == nullptr)
	{
		return;
	}

	CPoint point;
	GetCursorPos(&point);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void Cliufaimage2Dlg::OnImageEdit()
{
	int nSelIndex = GetCurrentSelImageIndex();
	if (nSelIndex < 0 || nSelIndex >= m_CurrentImages.GetSize())
	{
		return;
	}

	CEditDlg dlg;
	dlg.SetImage(m_CurrentImages[nSelIndex]);
	if (dlg.DoModal() == IDOK)
	{
		m_CurrentImages[nSelIndex] = dlg.GetImage();
		CImageManager::GetInstance()->UpdateImage(m_CurrentImages[nSelIndex]);
		ShowPage(m_nCurrentPageNumber);
	}
}


void Cliufaimage2Dlg::OnImageCompare()
{
	int nSelIndex = GetCurrentSelImageIndex();
	if (nSelIndex < 0 || nSelIndex >= m_CurrentImages.GetSize())
	{
		return;
	}

	CCompareDlg dlg;
	dlg.SetImages(&m_CurrentImages, nSelIndex);
	dlg.DoModal();
	ShowPage(m_nCurrentPageNumber);
}

void Cliufaimage2Dlg::OnImageDelete()
{
	int nImageIndex = GetCurrentSelImageIndex();
	if (nImageIndex < 0 || nImageIndex >= m_CurrentImages.GetSize())
	{
		return;
	}

	if (MessageBox(L"您确定要删除选择的图片吗？", L"提示", MB_OKCANCEL) == IDOK)
	{
		CImageManager::GetInstance()->DeleteImage(m_CurrentImages[nImageIndex].m_id);
		m_CurrentImages.RemoveAt(nImageIndex);
		ShowPage(m_nCurrentPageNumber);
	}
}
