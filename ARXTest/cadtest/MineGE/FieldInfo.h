#pragma once

#include "dlimexp.h"

#include "FieldType.h"

// �ֶ���Ϣ
class MINEGE_DLLIMPEXP FieldInfo
{
public:
	FieldInfo();
	
	/*
	 * Ĭ������
	 * ����3����������ͬʱ����
	 *     1) m_dt = DT_STRING
	 *     2) m_enable = true
	 *     3) m_descr = _T("")
	 */
	bool isDefault() const;
	bool isObjectListType() const;
	void revise();                     //  У����������
	void copyFrom(const FieldInfo& info);

public:

	DATA_TYPE m_dt;                    // Ĭ��Ϊ�ַ�������(DT_STRING)
	
	int m_minValue2, m_maxValue2;      // ��������

	double m_minValue, m_maxValue;     // ����������

	LIST_TYPE m_lt;                    // �б�����
	CString m_varName;                 // �б��������
		
	bool m_enable;                     // Ĭ�����Կɱ༭(true)
	CString m_descr;                   // ��Ҫ˵��

	bool m_showInTooltip;              // �Ƿ���tooltip����ʾ

private:
	void initDefault();                // �ָ�Ĭ������
	void initIntMinMaxValue();         // ����������Ĭ��ֵm_minValue2=m_maxValue2=0
	void initNumericMinMaxValue();     // ���ø�����Ĭ��ֵm_minValue=m_maxValue=0
	void initListType();               // �����б�Ĭ������m_lt=DT_LIST m_varName=_T("")
};
