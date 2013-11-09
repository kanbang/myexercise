#pragma once

#include "dlimexp.h"

class CMFCPropertyGridCtrl;

// ��MFCPropertyGridCtrl<-->objId֮����ʾ���༭��������������
class MINEGE_DLLIMPEXP PropertyDataUpdater
{
public:
	// �������������Լ��ֶγ�ʼ��CMFCPropertyGridCtrl
	// ע��pPropDataListָ����ڴ������Ч!!!
	static bool BuildPropGridCtrl(CMFCPropertyGridCtrl* pPropDataList, const CString& type, const AcStringArray& fields);

	// ��ȡ���ݵ�CMFCPropertyGridCtrl��
	// ǰ�᣺ʹ��BuildPropGridCtrl()�����ɹ�����CMFCPropertyGridCtrl
	static bool ReadDataFromGE(CMFCPropertyGridCtrl* pPropDataList, const AcDbObjectId& objId);

	// ��CMFCPropertyGridCtrl�е����ݸ��µ�ͼԪ��
	// ǰ�᣺ʹ��BuildPropGridCtrl()�����ɹ�����CMFCPropertyGridCtrl
	static bool WriteDataToGE(CMFCPropertyGridCtrl* pPropDataList, const AcDbObjectId& objId);
};
