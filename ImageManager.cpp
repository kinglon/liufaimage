#include "pch.h"
#include "ImageManager.h"
#include "Utility/ImPath.h"
#include "Utility/ImCharset.h"
#include <chrono>

using namespace std;

#define DB_FILE_NAME L"image.db"
#define TABLE_NAME  "image"
#define COLUMN_PRIMARY_KEY_NAME    "id"
#define COLUMN_MODEL    "model"
#define COLUMN_YEAR    "year"
#define COLUMN_TIMESTAMP    "time"
#define COLUMN_FILE_PATH    "filepath"
#define COLUMN_ORIGIN_FILE_PATH    "originfilepath"
#define COLUMN_STATUS    "status"
#define COLUMN_REMARK    "remark"
#define COLUMN_WATERMASK_TITLE    "title"
#define COLUMN_WATERMASK_WORKCONTENT    "workcontent"

CString CImageItem::GetCapturingDateTimeString()
{
	CTime time(m_nUnixTimeStamp);
	CString formattedDateTime = time.Format(_T("%Y-%m-%d %H:%M"));
	return formattedDateTime;
}

CString CImageItem::GetStatusString()
{
	CString strStatus;
	switch (m_nStatus)
	{
	case IMAGE_STATUS_NOT_PROCESS:
		strStatus = L"未处理";
		break;
	case IMAGE_STATUS_HAVE_PROBLEM:
		strStatus = L"有问题";
		break;
	case IMAGE_STATUS_NOT_PROBLEM:
		strStatus = L"没问题";
		break;
	default:
		break;
	}

	return strStatus;
}

CImageManager* CImageManager::GetInstance()
{
	static  CImageManager* instance = new CImageManager();
	return instance;
}

CImageManager::CImageManager()
{
	wstring strDbFile = CImPath::GetDataPath() + DB_FILE_NAME;
	if (m_dbUtil.Open(CImCharset::UnicodeToUTF8(strDbFile.c_str())) != CDBUtil::ERROR_DBUtil_Success)
	{
		return;
	}

    if (!CreateTable())
    {
        return;
    }
}

bool CImageManager::CreateTable()
{
    //主键字段
    vector<COLUMN_ITEM> columnVec;
    COLUMN_ITEM columnPrimary;
    columnPrimary.strName = COLUMN_PRIMARY_KEY_NAME;
    columnPrimary.dataType = DB_DATA_TYPE_INT;
    columnPrimary.IsPrimaryKey = true;
    columnPrimary.IsAutoIncrement = true;
    columnVec.push_back(columnPrimary);

    // model
    COLUMN_ITEM modelColumn;
    modelColumn.strName = COLUMN_MODEL;
    modelColumn.dataType = DB_DATA_TYPE_STR;
    columnVec.push_back(modelColumn);

    // year
    COLUMN_ITEM yearColumn;
    yearColumn.strName = COLUMN_YEAR;
    yearColumn.dataType = DB_DATA_TYPE_INT;
    columnVec.push_back(yearColumn);

    // time
    COLUMN_ITEM timeColumn;
    timeColumn.strName = COLUMN_TIMESTAMP;
    timeColumn.dataType = DB_DATA_TYPE_INT;
    columnVec.push_back(timeColumn);

    // file path
    COLUMN_ITEM filePathColumn;
    filePathColumn.strName = COLUMN_FILE_PATH;
    filePathColumn.dataType = DB_DATA_TYPE_STR;
    columnVec.push_back(filePathColumn);

    // origin file path
    COLUMN_ITEM originFilePathColumn;
    originFilePathColumn.strName = COLUMN_ORIGIN_FILE_PATH;
    originFilePathColumn.dataType = DB_DATA_TYPE_STR;
    columnVec.push_back(originFilePathColumn);

    // status
    COLUMN_ITEM statusColumn;
    statusColumn.strName = COLUMN_STATUS;
    statusColumn.dataType = DB_DATA_TYPE_INT;
    columnVec.push_back(statusColumn);

    // remark
    COLUMN_ITEM remarkColumn;
    remarkColumn.strName = COLUMN_REMARK;
    remarkColumn.dataType = DB_DATA_TYPE_STR;
    columnVec.push_back(remarkColumn);

    // watermask title
    COLUMN_ITEM waterMaskTitleColumn;
    waterMaskTitleColumn.strName = COLUMN_WATERMASK_TITLE;
    waterMaskTitleColumn.dataType = DB_DATA_TYPE_STR;
    columnVec.push_back(waterMaskTitleColumn);

    // work content
    COLUMN_ITEM workContentColumn;
    workContentColumn.strName = COLUMN_WATERMASK_WORKCONTENT;
    workContentColumn.dataType = DB_DATA_TYPE_STR;
    columnVec.push_back(workContentColumn);    

    if (m_dbUtil.CreateTable(TABLE_NAME, columnVec) != CDBUtil::ERROR_DBUtil_Success)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CImageManager::GetColumnItems(const CImageItem& image, std::vector<COLUMN_ITEM>& columnVec)
{
    // model
    COLUMN_ITEM modelColumn;
    modelColumn.strName = COLUMN_MODEL;
    modelColumn.dataType = DB_DATA_TYPE_STR;
    modelColumn.strValue = CImCharset::UnicodeToUTF8((LPCWSTR)image.m_strModel);
    columnVec.push_back(modelColumn);

    // year
    COLUMN_ITEM yearColumn;
    yearColumn.strName = COLUMN_YEAR;
    yearColumn.dataType = DB_DATA_TYPE_INT;
    yearColumn.strValue = std::to_string(image.m_nYear);
    columnVec.push_back(yearColumn);

    // time
    COLUMN_ITEM timeColumn;
    timeColumn.strName = COLUMN_TIMESTAMP;
    timeColumn.dataType = DB_DATA_TYPE_INT;
    timeColumn.strValue = std::to_string(image.m_nUnixTimeStamp);
    columnVec.push_back(timeColumn);

    // file path
    COLUMN_ITEM filePathColumn;
    filePathColumn.strName = COLUMN_FILE_PATH;
    filePathColumn.dataType = DB_DATA_TYPE_STR;
    filePathColumn.strValue = CImCharset::UnicodeToUTF8((LPCWSTR)image.m_strFilePath);
    columnVec.push_back(filePathColumn);

    // origin file path
    COLUMN_ITEM originFilePathColumn;
    originFilePathColumn.strName = COLUMN_ORIGIN_FILE_PATH;
    originFilePathColumn.dataType = DB_DATA_TYPE_STR;
    originFilePathColumn.strValue = CImCharset::UnicodeToUTF8((LPCWSTR)image.m_strOriginFilePath);
    columnVec.push_back(originFilePathColumn);

    // status
    COLUMN_ITEM statusColumn;
    statusColumn.strName = COLUMN_STATUS;
    statusColumn.dataType = DB_DATA_TYPE_INT;
    statusColumn.strValue = std::to_string(image.m_nStatus);
    columnVec.push_back(statusColumn);

    // remark
    COLUMN_ITEM remarkColumn;
    remarkColumn.strName = COLUMN_REMARK;
    remarkColumn.dataType = DB_DATA_TYPE_STR;
    remarkColumn.strValue = CImCharset::UnicodeToUTF8((LPCWSTR)image.m_strRemark);
    columnVec.push_back(remarkColumn);

    // watermask title
    COLUMN_ITEM waterMaskTitleColumn;
    waterMaskTitleColumn.strName = COLUMN_WATERMASK_TITLE;
    waterMaskTitleColumn.dataType = DB_DATA_TYPE_STR;
    waterMaskTitleColumn.strValue = CImCharset::UnicodeToUTF8((LPCWSTR)image.m_strWaterMaskTitle);
    columnVec.push_back(waterMaskTitleColumn);

    // work content
    COLUMN_ITEM workContentColumn;
    workContentColumn.strName = COLUMN_WATERMASK_WORKCONTENT;
    workContentColumn.dataType = DB_DATA_TYPE_STR;
    workContentColumn.strValue = CImCharset::UnicodeToUTF8((LPCWSTR)image.m_strWorkContent);
    columnVec.push_back(workContentColumn);
}

bool CImageManager::AddImage(const CImageItem& image)
{
    if (!m_dbUtil.IsAlreadyOpen())
    {
        LOG_ERROR(L"the database is not open");
        return false;
    }

    //主键字段
    vector<COLUMN_ITEM> columnVec;
    COLUMN_ITEM columnPrimary;
    columnPrimary.dataType = DB_DATA_TYPE_INT;
    columnPrimary.strName = COLUMN_PRIMARY_KEY_NAME;
    columnVec.push_back(columnPrimary);

    // 添加其他字段
    GetColumnItems(image, columnVec);

    //插入数据库
    return m_dbUtil.Insert(TABLE_NAME, columnVec) == CDBUtil::ERROR_DBUtil_Success;
}

bool CImageManager::UpdateImage(const CImageItem& image)
{
    if (!m_dbUtil.IsAlreadyOpen())
    {
        LOG_ERROR(L"the database is not open");
        return false;
    }

    vector<COLUMN_ITEM> columnVec;
    GetColumnItems(image, columnVec);

    string strWhere = string("id = ") + to_string(image.m_id);
    return m_dbUtil.Update(TABLE_NAME, columnVec, strWhere) == CDBUtil::ERROR_DBUtil_Success;
}

bool CImageManager::DeleteImage(UINT nImageId)
{
    if (!m_dbUtil.IsAlreadyOpen())
    {
        LOG_ERROR(L"the database is not open");
        return false;
    }

    COLUMN_ITEM columnPrimary;
    columnPrimary.dataType = DB_DATA_TYPE_INT;
    columnPrimary.strName = COLUMN_PRIMARY_KEY_NAME;
    columnPrimary.strValue = std::to_string(nImageId);
    int nResult = m_dbUtil.Delete(TABLE_NAME, vector<COLUMN_ITEM>({ columnPrimary }));
    if (nResult != 0)
    {
        return false;
    }

    return true;
}

bool CImageManager::EnumRecord(void* context, const std::vector<COLUMN_ITEM>& recordVec)
{
    CArray<CImageItem>* images = (CArray<CImageItem>*)context;
    CImageItem imageItem;
    for (auto record : recordVec)
    {        
        if (record.strName == COLUMN_PRIMARY_KEY_NAME)
        {
            imageItem.m_id = std::stoi(record.strValue.c_str());
        }
        if (record.strName == COLUMN_MODEL)
        {
            imageItem.m_strModel = CImCharset::UTF8ToUnicode(record.strValue.c_str()).c_str();
        }
        if (record.strName == COLUMN_YEAR)
        {
            imageItem.m_nYear = std::stoi(record.strValue.c_str());
        }
        if (record.strName == COLUMN_TIMESTAMP)
        {
            imageItem.m_nUnixTimeStamp = std::stoi(record.strValue.c_str());
        }
        if (record.strName == COLUMN_FILE_PATH)
        {
            imageItem.m_strFilePath = CImCharset::UTF8ToUnicode(record.strValue.c_str()).c_str();
        }
        if (record.strName == COLUMN_ORIGIN_FILE_PATH)
        {
            imageItem.m_strOriginFilePath = CImCharset::UTF8ToUnicode(record.strValue.c_str()).c_str();
        }
        if (record.strName == COLUMN_STATUS)
        {
            imageItem.m_nStatus = std::stoi(record.strValue.c_str());
        }
        if (record.strName == COLUMN_REMARK)
        {
            imageItem.m_strRemark = CImCharset::UTF8ToUnicode(record.strValue.c_str()).c_str();
        }
        if (record.strName == COLUMN_WATERMASK_TITLE)
        {
            imageItem.m_strWaterMaskTitle = CImCharset::UTF8ToUnicode(record.strValue.c_str()).c_str();
        }
        if (record.strName == COLUMN_WATERMASK_WORKCONTENT)
        {
            imageItem.m_strWorkContent = CImCharset::UTF8ToUnicode(record.strValue.c_str()).c_str();
        }        
    }
    images->Add(imageItem);

    return true;
}

std::string CImageManager::GetCondition(const CString& strModel, int nYear, int nStatus)
{
    std::string strWhere;
    if (!strModel.IsEmpty())
    {
        strWhere = strWhere + COLUMN_MODEL + " like " + "\"%" + CImCharset::UnicodeToUTF8((LPCWSTR)strModel) + "%\"";
    }

    if (nYear != 0)
    {
        if (!strWhere.empty())
        {
            strWhere += " AND ";
        }
        strWhere = strWhere + COLUMN_YEAR + "=" + to_string(nYear);
    }

    if (nStatus != 0)
    {
        if (!strWhere.empty())
        {
            strWhere += " AND ";
        }
        strWhere = strWhere + COLUMN_STATUS + "=" + to_string(nStatus);
    }

    return strWhere;
}

bool CImageManager::FindImage(const CString& strModel, int nYear, int nStatus, unsigned int nOffset, unsigned int nLimit, CArray<CImageItem>& images)
{
    if (!m_dbUtil.IsAlreadyOpen())
    {
        LOG_ERROR(L"the database is not open");
        return false;
    }

    images.RemoveAll();

    std::string strWhere = GetCondition(strModel, nYear, nStatus);
    int nResult = m_dbUtil.Query(TABLE_NAME, strWhere, nOffset, nLimit, EnumRecord, &images);
    if (nResult != 0)
    {
        return false;
    }

    return true;
}

int CImageManager::GetCount(const CString& strModel, int nYear, int nStatus)
{
    if (!m_dbUtil.IsAlreadyOpen())
    {
        LOG_ERROR(L"the database is not open");
        return false;
    }

    std::string strWhere = GetCondition(strModel, nYear, nStatus);
    int nCount = m_dbUtil.Count(TABLE_NAME, strWhere);
    return nCount;
}

time_t CImageManager::GetUnixTime(int year)
{
    std::tm timeStruct = {};
    timeStruct.tm_year = year - 1900; // Year offset from 1900
    timeStruct.tm_mon = 0; // January (0-based)
    timeStruct.tm_mday = 1; // Day 1
    timeStruct.tm_hour = 0; // Midnight
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;

    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(std::mktime(&timeStruct));
    std::time_t unixTime = std::chrono::system_clock::to_time_t(timePoint);
    
    return unixTime;
}