#pragma once

#include "PropertyData_DockBarChildDlg.h"

// ������������������
class WSDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(WSDockBarChildDlg)

public:
	WSDockBarChildDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // ��׼���캯��
	virtual ~WSDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_WS_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString m_name;
	CString m_area;
	CString m_velocity;
	CString m_q;
	CString m_temp;
	int m_cm;
	CString m_startZ;
	CString m_endZ;
	BOOL m_hasDangerous;

	bool lowGasMine;
	BOOL m_inHighGas;

protected:
	void readPropertyData();
	void writePropertyData();

private:
	void readSEZLevelData();
	void writeSEZLevelData();

	void readControlMeasure();
	void writeControlMeasure();
	
	void readInHighGas();
	void writeInHighGas();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAirCompositionBtn();
	afx_msg void OnBnClickedAfcBtn();
};
