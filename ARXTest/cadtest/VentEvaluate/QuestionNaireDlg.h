#pragma once
#include "afxcmn.h"
#include "resource.h"

class QuestionNaireDlg : public CDialog
{
	DECLARE_DYNAMIC(QuestionNaireDlg)

public:
	QuestionNaireDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~QuestionNaireDlg();

	// 需要传入的数据
	CString m_regulateDB;             // 规程数据库
	CString m_regulationTable;        // 主表(存放规程主要信息)
	void addClause(int clause);
	int getResult(int clause) const;

// 对话框数据
	enum { IDD = IDD_QuestionNaire_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CRichEditCtrl m_contentRichEdit;
	BOOL m_yes_or_no;

	int getCurrentClause() const;

	// 根据条款编号，填充richEdit控件内容
	void fillRichEditContent(int clause);

	// 修改条款的调查结果
	void setQuestionNaireResult(int pos, int yes_or_no);
	// 获取调查结果
	int getQuestionNaireResult(int clause);

	// 调查结果
	struct QuestionNaireResult
	{
		int clause;    // 条款编号
		int yes_or_no; // 调查结果（合格/不合格）
	};

	// 记录调查结果
	AcArray<QuestionNaireResult> m_qnrs;
	int m_pos; // 当前显示的条款(和数据m_qnrs的索引值同步)

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedPreviousBtn();
	afx_msg void OnBnClickedNextBtn();
	afx_msg void OnBnClickedYesRadio();
	afx_msg void OnBnClickedNoRadio();
};
