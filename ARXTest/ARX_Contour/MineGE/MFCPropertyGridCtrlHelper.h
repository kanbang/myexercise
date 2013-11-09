#pragma once

#include "dlimexp.h"

typedef bool (*ClickPropModifyCallBack)(const CString& oldValue, CString& newValue);

class CMFCPropertyGridCtrl;

// PropertyGridCtrl������
class MINEGE_DLLIMPEXP MFCPropertyGridCtrlHelper
{
public:
	MFCPropertyGridCtrlHelper(CMFCPropertyGridCtrl* m_pWndPropList);

	// ��ӵ����ַ�������
	void addStringProperty(const CString& name, const CString& value, LPCTSTR descr=NULL, bool bEnable=true);

	// �����������[minValue, maxValue]֮��ĵ�������/����������
	void addIntProperty(const CString& name, int value, int nMinValue, int nMaxValue, LPCTSTR descr=NULL, bool bEnable=true);
	void addDoubleProperty(const CString& name, double value, double dMinValue, double dMaxValue, unsigned short precise=6, LPCTSTR descr=NULL, bool bEnable=true);

	// ��Ӳ���ֵ����
	void addBoolProperty(const CString& name, bool value, LPCTSTR descr=NULL, bool bEnable=true);

	// �����������
	void addDateTimeProperty(const CString& name, const COleDateTime& value, LPCTSTR descr = NULL, bool bEnable=true);

	// �����ɢ���ַ��������б�
	void addStringPropertyList(const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr=NULL, bool bEnable=true);

	// �����ɢ��(����<--�ַ���)�����б�
	// ����һЩ�̶�������, һ��ʹ��ö��(����)��ʾ
	// Ϊ����߿ɶ���, �ڽ�����һ���ý����Ե��ַ���ӳ�䵽ö��	
	// �������CMFCPropertyGridProperty�����˼򵥵�����
	// ʹ���ⲿ��ʾ�����ַ���, �ڲ���������������
	void addIntPropertList(const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR descr=NULL, bool bEnable=true);
	
	void addDataObjectPropertyList(const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr=NULL, bool bEnable=true);

	// ***��ӵ��������(�ݲ�ʹ��)***
	void addClickProperty(const CString& name, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR descr, bool bEnable);

private:
	CMFCPropertyGridCtrl* m_pWndPropList;
};
