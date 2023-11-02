#pragma once

#include <vector>

class CModelIndexItem
{
public:
	std::string m_year;

	std::string m_modelPrefix;

	int m_yd = 1;

	int m_v = 1;

	int m_a = 1;
};

class CWaterMaskSetting
{
public:
	int m_offsetLeft = 40;

	int m_offsetBottom = 40;

	int m_width = 450;

	int m_height = 270;

	CString m_fontColor1;

	CString m_fontColor2;
};

class CSettingManager
{
protected:
	CSettingManager();

public:
	static CSettingManager* GetInstance();

public:
	int GetLogLevel() { return m_nLogLevel; }
	
	CString GetWaterMarkTitle() { return m_strWaterMarkTitle; }

	void SetWaterMarkTitle(const CString& strWaterMarkTitle) { m_strWaterMarkTitle = strWaterMarkTitle; }

	CString GetModelPrefix() { return m_strModelPrefix; }

	void SetModelPrefix(const CString& strModelPrefix) { m_strModelPrefix = strModelPrefix; }

	CString GetWorkContent() { return m_strWorkContent; }

	void SetWorkContent(const CString& strWorkContent) { m_strWorkContent = strWorkContent; }

	const std::vector<CModelIndexItem>& GetModelIndexItems() { return m_modeIndexItems; }

	void SetModelItemIndex(const std::vector<CModelIndexItem>& modelIndexItems);

	const CWaterMaskSetting GetWaterMaskSetting() { return m_waterMaskSetting; }

	int GetYear() { return m_nYear; }

	void SetYear(int nYear) { m_nYear = nYear; }

	void Save();

private:
	void Load();

private:
	int m_nLogLevel = 2;  // debug

	CString m_strWaterMarkTitle;

	CString m_strModelPrefix;

	CString m_strWorkContent;

	std::vector<CModelIndexItem> m_modeIndexItems;

	CWaterMaskSetting m_waterMaskSetting;

	int m_nYear = 2023;
};
