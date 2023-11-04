#include "pch.h"
#include "DBUtil.h"
#include "Utility/ImCharset.h"

using namespace std;

struct EnumRecordStruct
{
	PEnumRecord EnumRecord;
	void* context = nullptr;
};

CDBUtil::CDBUtil()
{
	//
}

CDBUtil::~CDBUtil()
{
	if (m_pDBConnection)
	{
		int nRet = sqlite3_close(m_pDBConnection);
		if (nRet != SQLITE_OK)
		{
            LOG_ERROR(L"failed to call sqlite3_close, error=%d", nRet);
		}
		m_pDBConnection = NULL;
	}
}

int CDBUtil::Open(const std::string& strDBFile)
{
	ASSERT(m_pDBConnection == NULL);
	if (m_pDBConnection)
	{
		return ERROR_DBUtil_HasOpen;
	}

    int nRet = sqlite3_open(strDBFile.c_str(), &m_pDBConnection);
	if (nRet != SQLITE_OK)
	{
		LOG_ERROR(L"failed to call sqlite3_open, error=%d", nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Close()
{
	if (m_pDBConnection == NULL) //已关闭
	{
		return ERROR_DBUtil_Success;
	}

	int nRet = sqlite3_close(m_pDBConnection);
	m_pDBConnection = NULL;
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to call sqlite3_close, error=%d", nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::CreateTable(const std::string& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	std::string strColumnDefine;	
	for (vector<COLUMN_ITEM>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strColumnDefine += ", ";
		}

		strColumnDefine += (*it).strName;  //列名

		if (it->dataType == DB_DATA_TYPE_INT)
		{
			strColumnDefine += " INTEGER";
		}
		else
		{
			strColumnDefine += " VARCHAR(1000)";
		}

		if (it->IsPrimaryKey)
		{
			strColumnDefine += " PRIMARY KEY ";
            if (it->IsAutoIncrement)
            {
                strColumnDefine += "AUTOINCREMENT ";
            }
		}
	}

    char szSQL[10 * MAX_PATH];
    memset(szSQL, 0, sizeof(szSQL));
    sprintf_s(szSQL, 10 * MAX_PATH, "create table if not exists %s (%s)", strTableName.c_str(), strColumnDefine.c_str());	
    int nRet = sqlite3_exec(m_pDBConnection, szSQL, NULL, NULL, NULL);
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to execute sql (%s), error=%d", szSQL, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::DropTable(const std::string& strTableName)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

    char szSQL[10 * MAX_PATH];
    memset(szSQL, 0, sizeof(szSQL));
    sprintf_s(szSQL, 10 * MAX_PATH, "DROP TABLE %s", strTableName.c_str());
    int nRet = sqlite3_exec(m_pDBConnection, szSQL, NULL, NULL, NULL);
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to execute sql (%s), error=%d", CImCharset::UTF8ToUnicode(szSQL).c_str(), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

std::string CDBUtil::ConstructInsertSqlStatement(const std::string& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	std::string strColumnName;	
	std::string strColumnValue;
	for (vector<COLUMN_ITEM>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strColumnName += ", ";
			strColumnValue += ", ";
		}

		strColumnName += it->strName;  //列名
        if (it->dataType == DB_DATA_TYPE_INT)
        {
            if (it->strValue.empty())
            {
                strColumnValue += "NULL";
            }
            else
            {
                strColumnValue += it->strValue;
            }
        }
        else
        {
            strColumnValue += "\"" + it->strValue + "\"";  //值	
        }
	}

    char szSQL[10 * MAX_PATH];
    memset(szSQL, 0, sizeof(szSQL));
    sprintf_s(szSQL, 10 * MAX_PATH, "insert or replace into %s (%s) values (%s)", strTableName.c_str(), strColumnName.c_str(), strColumnValue.c_str());
    
    return string(szSQL);
}

int CDBUtil::Insert(const std::string& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	std::string strSqlStatement = ConstructInsertSqlStatement(strTableName, columnVec);
    int nRet = sqlite3_exec(m_pDBConnection, strSqlStatement.c_str(), NULL, NULL, NULL);
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to execute sql(%s), error=%d", CImCharset::UTF8ToUnicode(strSqlStatement.c_str()).c_str(), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Insert(const std::string& strTableName, const vector<vector<COLUMN_ITEM>>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	std::string strSqlStatement;
	for (vector<vector<COLUMN_ITEM>>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strSqlStatement += "; ";
		}

		strSqlStatement += ConstructInsertSqlStatement(strTableName, (*it));
	}

    int nRet = sqlite3_exec(m_pDBConnection, strSqlStatement.c_str(), NULL, NULL, NULL);
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to execute sql(%s), error=%d", CImCharset::UTF8ToUnicode(strSqlStatement.c_str()).c_str(), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Update(const std::string& strTableName, const std::vector<COLUMN_ITEM>& columnVec, const std::string& strWhere)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	std::string setStatement;
	for (auto& item : columnVec)
	{
		if (!setStatement.empty())
		{
			setStatement += ",";
		}
		if (item.dataType == DB_DATA_TYPE_INT)
		{
			setStatement += item.strName + "=" + item.strValue;
		}
		else
		{
			setStatement += item.strName + "=\"" + item.strValue + "\"";
		}
	}

	std::string strSqlStatement = "update " + strTableName + " set " + setStatement + " where " + strWhere;
	int nRet = sqlite3_exec(m_pDBConnection, strSqlStatement.c_str(), NULL, NULL, NULL);
	if (nRet != SQLITE_OK)
	{
		LOG_ERROR(L"failed to execute sql(%s), error=%d", CImCharset::UTF8ToUnicode(strSqlStatement.c_str()).c_str(), nRet);
		return ERROR_DBUtil_SQLiteLib_Base + nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Delete(const std::string& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	std::string strWhere;
	for (vector<COLUMN_ITEM>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strWhere += " and ";
		}

        strWhere += it->strName + "=";
        if (it->dataType == DB_DATA_TYPE_INT)
        {
            if (it->strValue.empty())
            {
                strWhere += "NULL";
            }
            else
            {
                strWhere += it->strValue;
            }
        }
        else
        {
            strWhere +=string("\"") + it->strValue + "\"";
        }		
	}

    char szSQL[10 * MAX_PATH];
    memset(szSQL, 0, sizeof(szSQL));
	if (strWhere.empty())  //无条件
	{
        sprintf_s(szSQL, 10 * MAX_PATH, "delete from %s", strTableName.c_str());
	}
	else
	{
        sprintf_s(szSQL, 10 * MAX_PATH, "delete from %s where %s", strTableName.c_str(), strWhere.c_str());
	}

    int nRet = sqlite3_exec(m_pDBConnection, szSQL, NULL, NULL, NULL);
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to execute sql(% s), error = % d", CImCharset::UTF8ToUnicode(szSQL).c_str(), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Query(const std::string& strTableName, const std::string& strWhere, int nOffset, int nLimit, PEnumRecord EnumRecord, void* context)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}	

	EnumRecordStruct tempStruct;
	tempStruct.EnumRecord = EnumRecord;
	tempStruct.context = context;

    char szSQL[10 * MAX_PATH];
    memset(szSQL, 0, sizeof(szSQL));
    if (strWhere.empty())  //无条件
    {
        sprintf_s(szSQL, 10 * MAX_PATH, "select * from %s order by id DESC limit %d offset %d", strTableName.c_str(), nLimit, nOffset);
    }
    else
    {
        sprintf_s(szSQL, 10 * MAX_PATH, "select * from %s where %s order by id DESC limit %d offset %d", strTableName.c_str(), strWhere.c_str(), nLimit, nOffset);
    }

    int nRet = sqlite3_exec(m_pDBConnection, szSQL, QueryExecuteCallbac, &tempStruct, NULL);
	if (nRet != SQLITE_OK)
	{
        LOG_ERROR(L"failed to execute sql(%s), error=%d", CImCharset::UTF8ToUnicode(szSQL).c_str(), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Count(const std::string& strTableName, const std::string& strWhere)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	char szSQL[10 * MAX_PATH];
	memset(szSQL, 0, sizeof(szSQL));
	if (strWhere.empty())  //无条件
	{
		sprintf_s(szSQL, 10 * MAX_PATH, "select count(*) from %s", strTableName.c_str());
	}
	else
	{
		sprintf_s(szSQL, 10 * MAX_PATH, "select count(*) from %s where %s", strTableName.c_str(), strWhere.c_str());
	}

	int count = 0;
	int nRet = sqlite3_exec(m_pDBConnection, szSQL, CountExecuteCallbac, &count, NULL);
	if (nRet != SQLITE_OK)
	{
		LOG_ERROR(L"failed to execute sql(%s), error=%d", CImCharset::UTF8ToUnicode(szSQL).c_str(), nRet);
		return 0;
	}

	return count;
}

int CDBUtil::CountExecuteCallbac(void* context, int nColumnCount, char** ppColumnValue, char** ppColumnName)
{
	if (nColumnCount == 1)
	{
		*(int*)context = stoi(ppColumnValue[0]);
	}
	return 0;
}

int CDBUtil::QueryExecuteCallbac(void* context, int nColumnCount, char** ppColumnValue, char** ppColumnName)
{
	EnumRecordStruct* pContext = (EnumRecordStruct*)context;
	if (pContext->EnumRecord == NULL)  //没有回调
	{
		return 0;  //继续
	}

	vector<COLUMN_ITEM> columnVector;
	for (int i=0; i<nColumnCount; i++)
	{
		COLUMN_ITEM item;
		item.strName = ppColumnName[i];
		item.strValue = ppColumnValue[i];
		columnVector.push_back(item);
	}

	if (pContext->EnumRecord(pContext->context, columnVector)) //继续
	{
		return 0;  //继续
	}
	else
	{
		return 1;  //退出
	}
}