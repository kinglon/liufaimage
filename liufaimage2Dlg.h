#pragma once

#include "ImageManager.h"

class Cliufaimage2Dlg : public CDialogEx
{
public:
	Cliufaimage2Dlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIUFAIMAGE2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void InitAllControl();

	// 显示指定页数据，页码从1开始
	void ShowPage(int nPageNumber);

	// 获取的索引是m_CurrentImages容器里的索引
	int GetCurrentSelImageIndex();

private:
	CArray<CImageItem> m_CurrentImages;

	// 页码从1开始
	int m_nCurrentPageNumber = 1;

	CString m_filterModel;

	int m_filterYear = 0; // 0 is all

	int m_filterStatus = 0;  // 0 is all

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_statusComboBox;
	CStatic m_pageNumberCtrl;
	CStatic m_totalCtrl;
	CButton m_lastPageCtrl;
	CButton m_nextPageCtrl;
	CListCtrl m_imageListCtrl;
	afx_msg void OnBnClickedSearch();
	CEdit m_modelInputCtrl;
	CEdit m_yearInputCtrl;
	afx_msg void OnBnClickedLastPage();
	afx_msg void OnBnClickedNextPage();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedExportImage();
	afx_msg void OnNMRClickImageList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImageEdit();
	afx_msg void OnImageCompare();
	afx_msg void OnImageDelete();
	afx_msg void OnBnClickedImportFolderCtrl();
};