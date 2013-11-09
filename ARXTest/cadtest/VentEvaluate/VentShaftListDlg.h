#pragma once

#include "afxcmn.h"
#include "Resource.h"

class VentShaftListDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(VentShaftListDlg)

public:
	VentShaftListDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // ��׼���캯��
	virtual ~VentShaftListDlg();

// �Ի�������
	enum { IDD = IDD_VENT_SHAFT_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	bool m_listFinishInitial; // ��ʾlistctrl�Ƿ��ʼ�����???
	AcArray<Adesk::UInt16> m_colors;
	AcDbObjectIdArray m_objIds;
	int m_lastPos;
	CListCtrl m_list;

	CString m_name;
	CString m_area;
	CString m_velocity;
	CString m_temp;

private:
	void initTunnelList();
	void clearAll();
	void updateLastSelect();
	void readPropertyData(const AcDbObjectId& objId);
	void writePropertyData(const AcDbObjectId& objId);

public:
	// ����dock bar�رջ�����ʱ�����
	/* �ֶ���ӣ����Ǵӻ��������� */
	void OnClosing ();

	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	afx_msg void OnBnClickedVentInShaftUpdateBtn();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedVentShaftParamBtn();
};
