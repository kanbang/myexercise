#pragma once

#include "PropertyData_DockBarChildDlg.h"

class SinkAirLeakDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(SinkAirLeakDockBarChildDlg)

public:
	SinkAirLeakDockBarChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SinkAirLeakDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_FFC_SINK_AIR_LEAK_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	void readPropertyData();
	void writePropertyData();

public:
	CString m_q;
	virtual BOOL OnInitDialog();
};
