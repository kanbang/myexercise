#pragma once
#include "afxcmn.h"
#include "resource.h"

class QuestionNaireDlg : public CDialog
{
	DECLARE_DYNAMIC(QuestionNaireDlg)

public:
	QuestionNaireDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~QuestionNaireDlg();

	// ��Ҫ���������
	CString m_regulateDB;             // ������ݿ�
	CString m_regulationTable;        // ����(��Ź����Ҫ��Ϣ)
	void addClause(int clause);
	int getResult(int clause) const;

// �Ի�������
	enum { IDD = IDD_QuestionNaire_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CRichEditCtrl m_contentRichEdit;
	BOOL m_yes_or_no;

	int getCurrentClause() const;

	// ���������ţ����richEdit�ؼ�����
	void fillRichEditContent(int clause);

	// �޸�����ĵ�����
	void setQuestionNaireResult(int pos, int yes_or_no);
	// ��ȡ������
	int getQuestionNaireResult(int clause);

	// ������
	struct QuestionNaireResult
	{
		int clause;    // ������
		int yes_or_no; // ���������ϸ�/���ϸ�
	};

	// ��¼������
	AcArray<QuestionNaireResult> m_qnrs;
	int m_pos; // ��ǰ��ʾ������(������m_qnrs������ֵͬ��)

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedPreviousBtn();
	afx_msg void OnBnClickedNextBtn();
	afx_msg void OnBnClickedYesRadio();
	afx_msg void OnBnClickedNoRadio();
};
