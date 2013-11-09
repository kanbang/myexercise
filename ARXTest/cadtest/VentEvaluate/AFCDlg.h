#pragma once

#include "Resource.h"

enum AFC_TYPE
{
	AT_WS = 0, 
	AT_TWS = 1,
	AT_DHC = 2,
	AT_OT  = 3
};

class AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(AFCDlg)

public:
	AFCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AFCDlg();

// 对话框数据
	enum { IDD = IDD_AFC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	AFC_TYPE afc_type;
	
	/* 回采工作面需要的参数 */
	bool m_inHighGas;  // 是否位于高瓦斯区域
	bool m_lowGasMine; // 是否低瓦斯矿井

	/* 其它需风巷道需要的参数 */
	bool m_isDiesel;  // 是否胶轮车巷

	void readPropertyData();
	void writePropertyData();

private:
	double userAllocateQ;

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedAfcBtn();
	afx_msg void OnBnClickedAfcUserAllocateQCheck();
protected:
	virtual void OnCancel();
};
