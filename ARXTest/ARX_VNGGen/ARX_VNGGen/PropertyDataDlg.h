#pragma once

#include "afxcmn.h"
#include "afxpropertygridctrl.h"
#include "resource.h"

class PropertyDataDlg : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg)

public:
	PropertyDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PropertyDataDlg();

	enum { IDD = IDD_PROPERTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CMFCPropertyGridCtrl m_propertyDataList;
	AcStringArray m_fields;        // Ҫ��ʾ���ֶ�
	AcStringArray m_values;        // Ҫ��ʾ������

public:
	virtual BOOL OnInitDialog();
};
