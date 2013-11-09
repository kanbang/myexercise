#pragma once

#include "Resource.h"
#include "afxcmn.h"

class EvalDlg : public CDialog
{
	DECLARE_DYNAMIC(EvalDlg)

public:
	EvalDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EvalDlg();

// 对话框数据
	enum { IDD = IDD_EVAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	AcStringArray m_tables;
	CString m_regulateDB;
	CListCtrl m_list;
	CString m_outputReport;

private:
	// 初始化list ctrl
	void initListCtrl();
	// 从数据库中读取数据
	void readFromDB();
	// 获取当前选择的规程的数据库表的名称
	CString getSelRegulationTableName();
	// 执行常规评价中的逐条评价
	void doNormalEvalWithoutUnit(const CString& outputDirPath);

	// 生成报告
	bool generateReport(const CString& outputPath);
	// 写入数据到ini文件
	bool writeEvalDataIniFile(const CString& outputPath, bool regulationImpl = false);

	// 做一些简单检查(是否选择数据库、规程是否存在等)
	bool checkInput(); 
	// 做一些简单检查(是否选择数据库、规程是否存在等)
	bool checkOutput();
	// 清理临时数据(pj.mdb和ini文件)
	void cleanTempFile(const CString& outputPath);

public:
	afx_msg void OnBnClickedEvalDbSelBtn();
	afx_msg void OnBnClickedEvalOutputButton();
	afx_msg void OnBnClickedEvalDoBtn();
	virtual BOOL OnInitDialog();
};
