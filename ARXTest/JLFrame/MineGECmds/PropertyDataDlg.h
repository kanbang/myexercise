#pragma once

#include "resource.h"
#include "afxcmn.h"

// 属性数据对话框
class PropertyDataDlg : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg)

public:
	PropertyDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PropertyDataDlg();

// 对话框数据
	enum { IDD = IDD_PROPERTY_DLG };

	// 关联图元
	void setMineGE(const AcDbObjectId& objId);

	// 添加字段
	void addField(const CString& field);

	// 是否显示全部数据
	void showAllData(bool bFlag);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	CMFCPropertyGridCtrl m_propertyDataList;
	AcDbObjectId m_objId;          // 图元id
	AcStringArray m_fields;        // 要显示的字段
	bool m_showAll;                // 是否显示全部数据(默认true)
};
