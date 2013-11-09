#pragma once

#include "DataObject.h"

// ���ݲ��ݸ�����(MineGEģ���ڲ�ʹ��)
// �ڲ�����ֱ��ʹ�����ݶ���ָ�룬����������������Ч�ʽϺ�
class DataHelperImpl
{
public:
	// ���캯�����������ݶ���ָ����Ϊ����
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