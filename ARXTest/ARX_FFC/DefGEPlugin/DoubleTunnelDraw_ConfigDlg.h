#pragma once

#include "resource.h"
#include "afxwin.h"

// ˫��������ӻ�Ч�����öԻ���
class DoubleTunnelDraw_ConfigDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(DoubleTunnelDraw_ConfigDlg)

public:
	DoubleTunnelDraw_ConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DoubleTunnelDraw_ConfigDlg();

// �Ի�������
	enum { IDD = IDD_DOUBLE_TUNNEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CAcUiNumericEdit m_widthEdit;
	CString m_strWidth;
	
	CComboBox m_jointDrawList;
	int m_jdt;
	afx_msg void OnEnKillfocusWidthEdit();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
