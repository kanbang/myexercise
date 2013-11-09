#pragma once

#include "Resource.h"

// ��羮��
class WindStationDockBarChildDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(WindStationDockBarChildDlg)

public:
	WindStationDockBarChildDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // ��׼���캯��
	virtual ~WindStationDockBarChildDlg();

// �Ի�������
	enum { IDD = IDD_WIND_STATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	bool m_listFinishInitial; // ��ʾlistctrl�Ƿ��ʼ�����???
	int m_lastPos;
	AcArray<Adesk::UInt16> m_colors;
	AcDbObjectIdArray m_objIds;
	AcStringArray m_reasons;

	AcDbObjectIdArray m_hosts; // ��¼�÷�ص�Ĳ��վ�������

	CListCtrl m_list;
	BOOL m_hasStation;

	CString m_name;
	CString m_area;
	CString m_v;
	CString m_q;
private:
	void updateLastSelect();
	void initTunnelList();
	void readPropertyData(const AcDbObjectId& objId);
	void writePropertyData(const AcDbObjectId& objId);
	void clearAll();
	void clearAllData();

public:
	virtual BOOL OnInitDialog();
	
	// ����dock bar�رջ�����ʱ�����
	/* �ֶ���ӣ����Ǵӻ��������� */
	void OnClosing ();

	afx_msg void OnLvnItemchangedAirList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;
	afx_msg void OnBnClickedAirUpdateBtn();
	afx_msg void OnBnClickedAirSelBtn();
	afx_msg void OnBnClickedStationReadBtn();
	afx_msg void OnBnClickedStationZoomBtn();
};
