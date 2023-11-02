#pragma once

#include "ProgressDlg.h"
#include <vector>

class CImgImportDlg : public CProgressDlg
{
public:
    CImgImportDlg(CWnd* pParent = nullptr) : CProgressDlg(pParent) { }

public:
    void SetImages(std::vector<std::wstring> images) { m_images = images; }

    void AddWaterMaskComplete(CString originImage, bool isSuccess, CString imageWithMask, CString model);

private:
    void OnAddWaterMaskComplete(CString originImage, bool isSuccess, CString imageWithMask, CString model);

private:
    static DWORD __stdcall AddWaterMaskProc(LPVOID lpParam);

    void BeginAddWaterMask();

    static time_t GetImageCaptureTime(const std::wstring& imagePath);

    void CheckIfFinish();

private:
    std::vector<std::wstring> m_images;

    std::vector<CString> m_failedImages;

public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

