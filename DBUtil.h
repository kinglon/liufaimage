//
//������CDBUtil	���ݿ������
//ְ���������ݿ����ز���������ɾ�������С�ɾ���У�ʹ��sqlite��
//

#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>

enum  DB_DATA_TYPE	//���ݿ���������
{
	DB_DATA_TYPE_INT,		//����
	DB_DATA_TYPE_STR,		//�ַ���
};

typedef struct _tagColumnItem  //��
{
	std::string strName;	//����
	DB_DATA_TYPE dataType;	//��������
	std::string strValue;	//ֵ�������ַ�����ʾ������10����"10"��ʾ
	bool IsPrimaryKey;	//�Ƿ�Ϊ����
    bool IsAutoIncrement; //�����Ƿ��Զ�����

	_tagColumnItem()
	{
		dataType = DB_DATA_TYPE_INT;
		IsPrimaryKey = false;
        IsAutoIncrement = false;
	}

} COLUMN_ITEM, *PCOLUMN_ITEM;

//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
typedef bool (__stdcall *PEnumRecord)(void* context, const std::vector<COLUMN_ITEM>& recordVec);

class CDBUtil
{
public:
	CDBUtil();
	~CDBUtil();

	enum
	{
		ERROR_DBUtil_Success = 0,		//�ɹ�
		ERROR_DBUtil_Error = 1,			//����
		ERROR_DBUtil_HasOpen = 2,		//�Ѿ���
		ERROR_DBUtil_ToUtf8 = 3,		//ת��Ϊutf8ʱʧ��
		ERROR_DBUtil_HasNotOpen = 4,  //���ݿ���δ��

		//sqlite lib�����ʧ�ܣ�����ֵ����API�����ϼ��ϸû�ֵ
		ERROR_DBUtil_SQLiteLib_Base = 100,	
	};

public:
	//�򿪹ر����ݿ⣬strDBFile���ݿ��ļ�����·��������ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Open(const std::string& strDBFile);
	int Close();
    bool IsAlreadyOpen() { return m_pDBConnection != nullptr; }

	//����ɾ��columnVec�е�strValue�����壬����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int CreateTable(const std::string& strTableName, const std::vector<COLUMN_ITEM>& columnVec);
	int DropTable(const std::string& strTableName);

	//����һ�����ݣ�columnVec�е�strName��dataType��strValue������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Insert(const std::string& strTableName, const std::vector<COLUMN_ITEM>& columnVec);

	//����������ݵ��øú���Ч�ʸ��ߣ�columnVec�е�strName��dataType��strValue������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Insert(const std::string& strTableName, const std::vector<std::vector<COLUMN_ITEM>>& columnVec);

	//����һ�����ݣ�columnVec�е�strName��dataType��strValue������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Update(const std::string& strTableName, const std::vector<COLUMN_ITEM>& columnVec, const std::string& strWhere);

	//ɾ�����ݣ�columnVec�������������ָ����Ϊָ��ֵ�����ûָ���κ�������ɾ��������
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Delete(const std::string& strTableName, const std::vector<COLUMN_ITEM>& columnVec);

	//��ѯ����strWhere�ļ�¼����ѯ�����лص�EnumRecord��������context����ѯ��¼��Ϊ�������nOffset��nLimit���ڷ�ҳ��ѯ
	//����ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	int Query(const std::string& strTableName, const std::string& strWhere, int nOffset, int nLimit, PEnumRecord EnumRecord, void* context);

protected:	
	static int ExecuteCallbac(void* context, int nColumnCount, char** ppColumnValue, char** ppColumnName);  //sqlִ�лص�����
	std::string ConstructInsertSqlStatement(const std::string& strTableName, const std::vector<COLUMN_ITEM>& columnVec);  //����һ��SQL���

protected:
	sqlite3* m_pDBConnection = nullptr;			//���ݿ�����
};