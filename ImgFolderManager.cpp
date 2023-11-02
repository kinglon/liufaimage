#include "pch.h"
#include "ImgFolderManager.h"
#include "SettingManager.h"
#include "Utility/ImPath.h"

#define MAX_YD 26
#define MAX_V 4
#define MAX_A 8

CImgFolderManager* CImgFolderManager::GetInstance()
{
	static CImgFolderManager instance;
	return &instance;
}

CString CImgFolderManager::GetNextModelName(int year, const std::string& modelPrefix)
{
	std::string thisYear = std::to_string(year);
	auto modelIndexItems = CSettingManager::GetInstance()->GetModelIndexItems();
	for (auto& item : modelIndexItems)
	{
		if (item.m_year == thisYear && item.m_modelPrefix == modelPrefix)
		{
			if (item.m_yd >= MAX_YD && item.m_v >= MAX_V && item.m_a >= MAX_A)
			{
				LOG_ERROR(L"the model name has reached the max value");				
			}
			else
			{
				item.m_a += 1;
				if (item.m_a > MAX_A)
				{
					item.m_a %= MAX_A;
					item.m_v += 1;
					if (item.m_v > MAX_V)
					{
						item.m_v %= MAX_V;
						item.m_yd += 1;
					}
				}
				CSettingManager::GetInstance()->SetModelItemIndex(modelIndexItems);
			}

			CString modelIndex;
			modelIndex.Format(L"%c.V%d.A%d", item.m_yd - 1 + 'A', item.m_v, item.m_a);
			return CSettingManager::GetInstance()->GetModelPrefix() + modelIndex;
		}
	}

	// 如果没有找到当年的索引配置
	CModelIndexItem thisYearIndexItem;
	thisYearIndexItem.m_year = thisYear;
	thisYearIndexItem.m_modelPrefix = modelPrefix;
	thisYearIndexItem.m_yd = 1;
	thisYearIndexItem.m_v = 1;
	thisYearIndexItem.m_a = 1;
	modelIndexItems.push_back(thisYearIndexItem);
	CSettingManager::GetInstance()->SetModelItemIndex(modelIndexItems);
	return CSettingManager::GetInstance()->GetModelPrefix() + L"A.V1.A1";
}

CString CImgFolderManager::GetImagePath(const CString& year, const CString& modelPrefix)
{
	// 目录结构：data\images\year\modelprefix\imagefile
	std::wstring imagesPath = CImPath::GetDataPath() + L"images";
	if (!PathFileExists(imagesPath.c_str()))
	{
		CreateDirectory(imagesPath.c_str(), NULL);
	}

	std::wstring yearPath = imagesPath + L"\\" + (LPCWSTR)year;
	if (!PathFileExists(yearPath.c_str()))
	{
		CreateDirectory(yearPath.c_str(), NULL);
	}

	std::wstring modePrefixPath = yearPath + L"\\" + (LPCWSTR)modelPrefix + L"\\";
	if (!PathFileExists(modePrefixPath.c_str()))
	{
		CreateDirectory(modePrefixPath.c_str(), NULL);
	}

	return modePrefixPath.c_str();
}

CString CImgFolderManager::AddImage(const CString& imageFilePath, const CString& year, const CString& modelPrefix)
{
	CString newPath = GetImagePath(year, modelPrefix);
	CString newFilePath = newPath + imageFilePath.Mid(imageFilePath.ReverseFind(L'\\') + 1);
	if (!CopyFile(imageFilePath, newFilePath, TRUE))
	{
		LOG_ERROR(L"failed to add image %s", (LPCTSTR)newFilePath);
	}
	return newFilePath;
}

CString CImgFolderManager::AddOriginImage(const CString& imageFilePath)
{
	CString newPath = (CImPath::GetDataPath() + L"origin_images\\").c_str();
	if (!PathFileExists((LPCWSTR)newPath))
	{
		CreateDirectory((LPCWSTR)newPath, NULL);
	}
	CString guid = GetGuid();
	CString newFilePath = newPath;
	if (!guid.IsEmpty())
	{
		newFilePath += GetGuid() + imageFilePath.Mid(imageFilePath.ReverseFind(L'.'));
	}
	else
	{
		newFilePath += imageFilePath.Mid(imageFilePath.ReverseFind(L'\\') + 1);
	}
	if (!CopyFile(imageFilePath, newFilePath, TRUE))
	{
		LOG_ERROR(L"failed to add origin image %s", (LPCTSTR)newFilePath);
	}
	return newFilePath;
}

bool CImgFolderManager::ReplaceImage(const CString& srcImageFilePath, const CString& destImageFilePath)
{
	DeleteFile(destImageFilePath);
	if (MoveFile(srcImageFilePath, destImageFilePath))
	{		
		return true;
	}
	else
	{		
		LOG_ERROR(L"failed to move file from %s to %s, error is %d",
			(LPCWSTR)srcImageFilePath, (LPCWSTR)destImageFilePath, GetLastError());
		return false;
	}
}

bool CImgFolderManager::DeleteImage(const CString& imageFilePath)
{
	return DeleteFile(imageFilePath) ? true : false;
}

bool CImgFolderManager::DeleteOriginImage(const CString& imageFilePath)
{
	return DeleteFile(imageFilePath) ? true : false;
}

CString CImgFolderManager::GetGuid()
{
	GUID guid;
	if (CoCreateGuid(&guid) == S_OK) 
	{
		wchar_t guidString[40] = { 0 };
		if (StringFromGUID2(guid, guidString, 40) != 0) 
		{
			return guidString;
		}
	}
	
	return L"";
}