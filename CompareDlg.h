#pragma once
#include "afxdialogex.h"
#include "ImageManager.h"

// CCompareDlg 对话框

class CCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareDlg)

public:
	CCompareDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCompareDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE };
#endif

public:
	void SetImages(CArray<CImageItem>* images, int nCurrentIndex);

private:
	void ShowImage(int imageIndex);

	void ResetAllCtrl();

	void InitImageCtrl(CStatic* imageCtrl, const CString& imgFilePath);

private:
	CArray<CImageItem>* m_images = nullptr;

	// 从0开始
	int m_currentIndex = 0;

	// model为空表示该图片无效
	CImageItem m_leftImageItem;

	// model为空表示该图片无效
	CImageItem m_rightImageItem;

	std::vector<HBITMAP> m_imageHandle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_modelCtrl1;
	CStatic m_modelCtrl2;
	CStatic m_imageCtrl1;
	CStatic m_imageCtrl2;
	CComboBox m_statusCtrl1;
	CComboBox m_statusCtrl2;
	CEdit m_remarkCtrl1;
	CEdit m_remarkCtrl2;
	CButton m_lastPieceCtrl1;
	CButton m_nextPieceCtrl;
	afx_msg void OnDblclkImagectrl1();
	afx_msg void OnDblclkImagectrl2();
	afx_msg void OnClickedLastpagectrl();
	afx_msg void OnClickedNextpagectrl();
	afx_msg void OnClickedSavectrl();
	afx_msg void OnBnClickedShoworiginimage();
	afx_msg void OnBnClickedShowmaskimage();
};
