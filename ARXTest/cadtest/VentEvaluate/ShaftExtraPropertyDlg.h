#pragma once

#include "resource.h"

#include "../MineGE/DataLink.h"

class ShaftExtraPropertyDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	int sp;  // 附加属性
};

class ShaftExtraPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(ShaftExtraPropertyDlg)

public:
	ShaftExtraPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShaftExtraPropertyDlg();

// 对话框数据
	enum { IDD = IDD_SHAFT_EXTRA_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLiftUsageRadio();        // 提升用途切换事件
	afx_msg void OnBnClickedVentOutRadio();
	afx_msg void OnBnClickedLiftCoalRadio();
	afx_msg void OnBnClickedLiftMaterialRadio();
	afx_msg void OnBnClickedOk();

public:
	void setRelatedShaft(const AcDbObjectId& objId);  // 关联一个井筒Shaft

private:
	int m_liftUsage;              // 初始化为0(无提升用途)
	int m_liftCoalMethod;         // 初始化为0(箕斗提升)

	void switchExtraProperty1(bool flag);
	void switchExtraProperty2(bool flag);
	void readExtraPropertyFromShaft();    // 从井筒中读取附加属性
	void readExtraProperty1(int value);    // 提取数据并更新到附加属性1的控件上
	void readExtraProperty2(int value);    // 提取数据并更新到附加属性2的控件上

	void writeExtraPropertyToShaft();     // 将更新后的属性写回井筒中
	int caclLiftUsage();
	int caclLiftCoalMethod();
	int caclExtraPropety1();
	int caclExtraPropety2();
	CButton* getButtonCtrl(long control_id);

	ShaftExtraPropertyDataLink* pSEPM;
};
