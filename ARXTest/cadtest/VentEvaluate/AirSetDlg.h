#pragma once

#include "Resource.h"
#include "afxcmn.h"

// 井下空气成分设置对话框
class AirSetDlg : public CDialog
{
	DECLARE_DYNAMIC(AirSetDlg)

public:
	AirSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AirSetDlg();

// 对话框数据
	enum { IDD = IDD_AIR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_lastPos;
	AcDbObjectIdArray m_objIds;
	CListCtrl m_list;
	AcStringArray m_fields; // 字段
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
