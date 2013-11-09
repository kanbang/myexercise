#pragma once

#include "DataObject.h"

// ���ݲ��ݸ�����
// ֱ��ʹ������ָ�룬����������������Ч�ʽϺ�
class DataHelperImpl
{
public:
	DataHelperImpl(DataObject* pObj);

	// ��ȡ��������
	// ����ֶβ����ڣ�����false
	bool getPropertyData(const CString& fieldName, CString& value);

	// �޸���������
	// ����ֶβ����ڣ�����false
	bool setPropertyData(const CString& fieldName, const CString& value);

private:
	DataObject* m_pObj;
};