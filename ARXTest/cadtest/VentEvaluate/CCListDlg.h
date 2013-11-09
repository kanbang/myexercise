#pragma once
#include "afxcmn.h"

#include "Resource.h"

class CCListDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCListDlg)

public:
	CCListDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // ��׼���캯��
	virtual ~CCListDlg();

// �Ի�������
	enum { IDD = IDD_CC_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	bool m_listFinishInitial; // ��ʾlistctrl�Ƿ��ʼ�����???
	AcArray<Adesk::UInt16> m_colors;
	AcDbObjectIdArray m_objIds;
	AcDbIntArray m_ccIds;
	int m_lastPos;
	CListCtrl m_list;

private:
	void initTunnelList();
	void clearAll();
	void findEdgeByCC(int cc, int& startPos, int& endPos);

public:
	// ����dock bar�رջ�����ʱ�����
	/* �ֶ���ӣ����Ǵӻ��������� */
	void OnClosing ();

	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	afx_msg void OnBnClickedCCListUpdateBtn();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
