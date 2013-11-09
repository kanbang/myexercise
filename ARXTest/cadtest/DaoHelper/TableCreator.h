#pragma once

#include "dlimexp.h"

// DAO�������ݿ��
class DAO_HELPER_EXPORT_API TableCreator
{
public:
	TableCreator(CDaoDatabase* pDB);
	~TableCreator(void);

	// ��ȡ���ݿ������ļ���·��
	CString getDBDirPath() const;

	TableCreator* clone();

	// �����Ƿ����
	bool isTableExist(const CString& tableName);

	// ������
	void createTable(const CString& tableName);

	// �رձ�
	void closeTable();

	// ���һ�������ֶ�
	void addPrimaryKeyField(const CString& primarykeyName);

	// ���һ�������ֶ�
	void addIntField(const CString& fieldName);

	// ���һ��ʵ���ֶ�
	void addRealField(const CString& fieldName);

	// ���һ�������ֶ�
	void addBoolField(const CString& fieldName);

	// ���һ���ַ����ֶ�(���255Byte�����ݣ�
	void addStringField(const CString& fieldName);

	// ���һ����ע�ֶ�(��ε�����)
	void addCommentField(const CString& fieldName);

private:
	CDaoTableDef* m_pTableDef;
	CDaoDatabase* m_pDB;
};