#pragma once

#include "FieldInfoDlg.h"

// ͼԪ�ֶι���Ի���
class FieldManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(FieldManagerDlg)

public:
	FieldManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FieldManagerDlg();

// �Ի�������
	enum { IDD = IDD_FIELD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_typeCombo;
	CListBox m_fieldListBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddBtn();
	afx_msg void OnBnClickedDelBtn();
	afx_msg void OnCbnSelchangeTypeCombo2();
	afx_msg void OnBnClickedApplyBtn();
	afx_msg void OnLbnSelchangeFieldList();
	afx_msg void OnClose(); // �ƺ���(����ָ��)
	afx_msg void OnPaint();

private:
	void clearFieldInfoArray();
	void showFieldInfo();
	void clearFieldListBox();
	void fillContent();
	void fillFieldListBox(const CString& type);
	CString getSelType();
	CString getSelField();
	bool isFieldExistInListBox(const CString& field);      // ����ֶ��Ƿ��Ѵ�����fieldlistbox��
	bool isValidField(const CString& field);               // ����ֶ��Ƿ���Ч(�д��޸�)
	void setLastSelIndex(int sel);
	int getLastSelIndex() const;
	bool updateFieldInfo();

	FieldInfoDlg fidlg;
	AcArray<FieldInfo*> m_infoes;
	int m_lastSel;            // ��¼listbox�л�֮ǰ������λ��
};
