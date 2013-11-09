#pragma once

#include "resource.h"
#include "afxwin.h"

// 圆形节点可视化效果配置对话框
class CircularJointDraw_ConfigDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CircularJointDraw_ConfigDlg)

public:
	CircularJointDraw_ConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CircularJointDraw_ConfigDlg();

// 对话框数据
	enum { IDD = IDD_CIRCULAR_JOINT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CAcUiNumericEdit m_radiusEdit;
	CString m_strRadius;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusRadiusEdit();
	afx_msg void OnBnClickedOk();
};
