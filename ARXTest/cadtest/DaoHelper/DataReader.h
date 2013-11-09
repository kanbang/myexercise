#pragma once

#include "dlimexp.h"

// �����ݶ�ȡ��
class DAO_HELPER_EXPORT_API DataReader
{
public:
	DataReader(CDaoDatabase* pDB);
	~DataReader(void);

	// ��ʼ��ȡ����
	// ��ȡ���е��������ݣ�Ŀǰ�ݲ�֧�ֲ�����ѯ
	void beginReadData(const CString& tableName);
	
	// ������ȡ����
	void endReadData();

	// �Ƿ��м�¼�ɶ�
	bool hasNext() const;

	// �ƶ�����һ����¼
	void moveNext();

	// ��ȡ�ַ�������(Ҳ����"��ע"���͵��ֶ�����)
	void readStringData(const CString& fieldName, CString& value);

	// ��ȡ��������
	void readIntData(const CString& fieldName, int& value);

	// ��ȡʵ������
	void readRealData(const CString& fieldName, double& value);

	// ��ȡ��������
	void readBoolData(const CString& fieldName, bool& value);

private:
	CDaoRecordset* m_pRecordSet;
	CDaoDatabase* m_pDB;
};
