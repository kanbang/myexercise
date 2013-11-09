#pragma once

#include "Resource.h"
#include "afxcmn.h"

class EvalDlg : public CDialog
{
	DECLARE_DYNAMIC(EvalDlg)

public:
	EvalDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EvalDlg();

// �Ի�������
	enum { IDD = IDD_EVAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	AcStringArray m_tables;
	CString m_regulateDB;
	CListCtrl m_list;
	CString m_outputReport;

private:
	// ��ʼ��list ctrl
	void initListCtrl();
	// �����ݿ��ж�ȡ����
	void readFromDB();
	// ��ȡ��ǰѡ��Ĺ�̵����ݿ�������
	CString getSelRegulationTableName();
	// ִ�г��������е���������
	void doNormalEvalWithoutUnit(const CString& outputDirPath);

	// ���ɱ���
	bool generateReport(const CString& outputPath);
	// д�����ݵ�ini�ļ�
	bool writeEvalDataIniFile(const CString& outputPath, bool regulationImpl = false);

	// ��һЩ�򵥼��(�Ƿ�ѡ�����ݿ⡢����Ƿ���ڵ�)
	bool checkInput(); 
	// ��һЩ�򵥼��(�Ƿ�ѡ�����ݿ⡢����Ƿ���ڵ�)
	bool checkOutput();
	// ������ʱ����(pj.mdb��ini�ļ�)
	void cleanTempFile(const CString& outputPath);

public:
	afx_msg void OnBnClickedEvalDbSelBtn();
	afx_msg void OnBnClickedEvalOutputButton();
	afx_msg void OnBnClickedEvalDoBtn();
	virtual BOOL OnInitDialog();
};
