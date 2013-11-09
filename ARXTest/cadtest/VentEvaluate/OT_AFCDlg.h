#pragma once

#include "resource.h"

class OT_AFCDlg : public CDialog
{
	DECLARE_DYNAMIC(OT_AFCDlg)

public:
	OT_AFCDlg(CWnd* pParent = NULL);
	virtual ~OT_AFCDlg();

// �Ի�������
	enum { IDD = IDD_OT_AIR_FLOW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	AcDbObjectId m_objId;
	bool m_isDiesel;  // �Ƿ��ֳ���

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOtAfcGasBtn();
	afx_msg void OnBnClickedOtAfcDieselBtn();
};
