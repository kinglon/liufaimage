#pragma once

class CWaterMaskUtil
{
public:
	CString AddWaterMask(const CString& imageFilePath, const CString& title, time_t time, const CString& model,
		const CString& workContent);

private:
	static Gdiplus::Color ParseColorString(const CString& colorString);
};

