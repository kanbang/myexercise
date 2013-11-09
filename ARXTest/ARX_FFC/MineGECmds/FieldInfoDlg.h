#pragma once

#include "resource.h"
#include "afxwin.h"

#include "../MineGE/FieldInfo.h"

// FieldInfoDlg �Ի���
class FieldInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(FieldInfoDlg)

public:
	FieldInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FieldInfoDlg();

// �Ի�������
	enum { IDD = IDD_FIELD_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedUseList();
	afx_msg void OnBnClickedSetDef();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedDisplay();
	afx_msg void OnCbnSelchangeVarNameList();

public:
	// �ڶԻ�����ʾ֮ǰ����
	// dlg.readFromFieldInfo(info);
	// dlg.showwindow() ����dlg.domodal()
	void readFromFieldInfo(const FieldInfo& info);

	// �ӶԻ�������ȡFieldInfo��Ϣ
	void writeToFieldInfo(FieldInfo& info);

	// ��֤���ݸ�ʽ������Ч��
	bool validateFieldInfo();

private:
	bool validDataformat(const CString& str, bool isIntType=true);
	bool validMinMaxValue(long id, bool isIntType = true);
	bool compareMinMaxValue(long id1, long id2, bool isIntType = true);
	bool checkEmptyValue(long id1);
	void displayControls();
	void fillVarList();
	CString getSelVarName();	
	
	int m_dt;
	
	double m_dMinValue;   // ����������
	double m_dMaxValue;
	
	int m_nMinValue;      // ��������
	int m_nMaxValue;
	
	BOOL m_unEnable;
	CString m_descr;

	int m_lt;
	CString m_varName;

	CComboBox m_varList;

	BOOL m_showInTooltip;
public:
	afx_msg void OnBnClickedRadio6();
};
