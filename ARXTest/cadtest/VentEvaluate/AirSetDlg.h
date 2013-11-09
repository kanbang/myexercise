#pragma once

#include "Resource.h"
#include "afxcmn.h"

// ���¿����ɷ����öԻ���
class AirSetDlg : public CDialog
{
	DECLARE_DYNAMIC(AirSetDlg)

public:
	AirSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AirSetDlg();

// �Ի�������
	enum { IDD = IDD_AIR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_lastPos;
	AcDbObjectIdArray m_objIds;
	CListCtrl m_list;
	AcStringArray m_fields; // �ֶ�
	CMFCPropertyGridCtrl m_propertyDataList;

private:
	void initFields();
	void initTunnelList();
	void readPropertData(const AcDbObjectId& objId);
	void writePropertData(const AcDbObjectId& objId);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedAirList(NMHDR *pNMHDR, LRESULT *pResult);
};
