#pragma once

#include "Resource.h"
#include "afxwin.h"

class ProjectInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(ProjectInfoDlg)

public:
	ProjectInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProjectInfoDlg();

// 对话框数据
	enum { IDD = IDD_PROJECT_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_evalBasis;
	COleDateTime m_prepare_solution_begin;
	COleDateTime m_prepare_solution_end;
	COleDateTime m_prepare_info_begin;
	COleDateTime m_prepare_info_end;
	COleDateTime m_prepare_contract_begin;
	COleDateTime m_prepare_contract_end;
	COleDateTime m_survey_learn_begin;
	COleDateTime m_survey_learn_end;
	COleDateTime m_work_do_begin;
	COleDateTime m_work_do_end;
	COleDateTime m_work_report_begin;
	COleDateTime m_work_report_end;

public:
	CString getEvalBasis();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
