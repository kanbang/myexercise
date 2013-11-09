#pragma once

#include "resource.h"
#include "afxcmn.h"

// �������ݶԻ���
class PropertyDataDlg : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg)

public:
	PropertyDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PropertyDataDlg();

// �Ի�������
	enum { IDD = IDD_PROPERTY_DLG };

	// ����ͼԪ
	void setMineGE(const AcDbObjectId& objId);

	// ����ֶ�
	void addField(const CString& field);

	// �Ƿ���ʾȫ������
	void showAllData(bool bFlag);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	CMFCPropertyGridCtrl m_propertyDataList;
	AcDbObjectId m_objId;          // ͼԪid
	AcStringArray m_fields;        // Ҫ��ʾ���ֶ�
	bool m_showAll;                // �Ƿ���ʾȫ������(Ĭ��true)
};
