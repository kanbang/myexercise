#pragma once

#include "afxcmn.h"
#include "afxpropertygridctrl.h"
#include "resource.h"

class PropertyDataDlg : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg)

public:
	PropertyDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PropertyDataDlg();

	enum { IDD = IDD_PROPERTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CMFCPropertyGridCtrl m_propertyDataList;
	AcStringArray m_fields;        // 要显示的字段
	AcStringArray m_values;        // 要显示的数据

public:
	virtual BOOL OnInitDialog();
};
