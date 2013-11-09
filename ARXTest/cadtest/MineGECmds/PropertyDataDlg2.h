#pragma once

#include "afxcmn.h"
#include "resource.h"

#include "ListCtrlEx.h"

class PropertyDataDlg2 : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg2)

public:
	PropertyDataDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PropertyDataDlg2();

// �Ի�������
	enum { IDD = IDD_PROPERTY_DLG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	// ����ͼԪ
	void setMineGE(const AcDbObjectId& objId);

	// ����ֶ�
	void addField(const CString& field);

	// �Ƿ���ʾȫ������
	void showAllData(bool bFlag);

public:
	AcDbObjectId m_objId;          // ͼԪid
	AcStringArray m_fields;        // Ҫ��ʾ���ֶ�
	bool m_showAll;                // �Ƿ���ʾȫ������(Ĭ��true)

	CListCtrlEx m_propertyDataList;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
