#pragma once

#include "PropertyData_DockBarChildDlg.h"

class RoomDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(RoomDockBarChildDlg)

public:
	RoomDockBarChildDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // ��׼���캯��
	virtual ~RoomDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_ROOM_PARAM_DLG };

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

protected:
	void readPropertyData();
	void writePropertyData();

private:
	// ��д���¿��ƴ�ʩ
	void readControlMeasure();	
	void writeControlMeasure();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRoomAfcBtn();
};
