#pragma once

#include "resource.h"
#include "afxwin.h"

// Բ�νڵ���ӻ�Ч�����öԻ���
class CircularJointDraw_ConfigDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CircularJointDraw_ConfigDlg)

public:
	CircularJointDraw_ConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CircularJointDraw_ConfigDlg();

// �Ի�������
	enum { IDD = IDD_CIRCULAR_JOINT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CAcUiNumericEdit m_radiusEdit;
	CString m_strRadius;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusRadiusEdit();
	afx_msg void OnBnClickedOk();
};
