#pragma once

#include "dlimexp.h"

// ������д����
class DAO_HELPER_EXPORT_API DataWriter
{
public:
	DataWriter(CDaoDatabase* pDB);
	~DataWriter(void);

	// ��ȡ���ݿ������ļ���·��
	CString getDBDirPath() const;

	DataWriter* clone();

	// ��ʼд������
	void beginWriteData(const CString& tableName);

	// ����д������
	long endWriteData();

	// д���ַ�������(Ҳ����"��ע"���͵��ֶ�����)
	void writeStringData(const CString& fieldName, const CString& value);

	// д����������
	void writeIntData(const CString& fieldName, int value);

	// д��ʵ������
	void writeRealData(const CString& fieldName, double value);

	// д�벼������
	void writeBoolData(const CString& fieldName, bool value);

private:
	CDaoRecordset* m_pRecordSet;
	CDaoDatabase* m_pDB;
};