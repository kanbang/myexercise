#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include <map>
#include <deque>

#include "Resource.h"
#include "MutiTreeCtrl.h"
#include "../EvalBase/EvalResult.h"
using namespace PJ;

struct EvalUnitInfo;
typedef std::deque<int> ClauseCollection;
typedef std::map<CString, EvalUnitInfo> EvalUnitInfoMap;
typedef std::map<CString, EvalResult>   EvalResultMap;

struct EvalUnitInfo                  // ���۵�Ԫ��Ϣ
{
	CString parent_unit;             // �����۵�Ԫ
	ClauseCollection clauses;        // ������
	int hierarchy_num;               // ��α��
};

// �������۶Ի���
class NormalEvalSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(NormalEvalSettingDlg)

public:
	NormalEvalSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NormalEvalSettingDlg();

// �Ի�������
	enum { IDD = IDD_NORMAL_EVAL_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_regulationInfoFileName;
	CListCtrl m_regulationListCtrl;
	CMutiTreeCtrl m_evalUnitTreeCtrl;
	CImageList m_imgList;
	CImageList m_imgState;
	CButton m_normalEvalTypeRadio;
	CString m_outputEvalReportPath;

private:
	void initRegulationListCtrl();                         // ��ʼ��list ctrl
	void readRegulationInfoFromDB();                       // �����ݿ��ж�ȡ����

	void initEvalUnitTreeCtrl();                           // ��ʼ��treectrl
	void readEvalUnitFromDB(const CString& tableName);     // �����ݿ��ж�ȡ���۵�Ԫ����

	// ��������
	// �������۵�Ԫ����״�ṹ
	void initEvalUntiHierarchy(const CString& tableName, EvalUnitInfoMap& euim);
	void caclEvalUnitHierarchy(EvalUnitInfoMap& euim);
	void buildTree(EvalUnitInfoMap& euim);                 // ��treectrl�в������ݣ��������ṹ
	void buildSubTree(EvalUnitInfoMap& euim, TV_INSERTSTRUCT tvinsert, const CString& parent_unit, int hierarchy_num);
	void insertLeafNode(EvalUnitInfoMap& euim, TV_INSERTSTRUCT tvinsert, const CString& parent_unit);	
	void visitTree(EvalUnitInfoMap& euim, CTreeCtrl& tree, HTREEITEM hItem);      // ����treectrl���õ�ѡ������۵�Ԫ�����Ϣ

	CString getSelRegulationTableName();                  // ��ȡ��ǰѡ��Ĺ�̵����ݿ�������
	void getClauseCollection(ClauseCollection& clauses);  // ��ȡ��ǰ����е�����������
	void doNormalEvalWithUnit_firstStep(LPCTSTR dbPath, const CString& regulationTable, EvalUnitInfoMap& euim);
	void doNormalEvalWithUnit_secondStep(CDaoDatabase* pDB, EvalUnitInfoMap& euim, EvalResultMap& er_map);
	void doNormalEvalWithUnit_thirdStep(CDaoDatabase* pDB, EvalResultMap& er_map);

	int getEvalMethodType();                                           // ��ȡ���۷�������
	void doNormalEvalWithoutUnit(const CString& outputEvalReportPath); // ִ�г��������е���������
	void getSelEvalUnitMap(EvalUnitInfoMap& euim);                     // ����treectrl���õ�ѡ������۵�Ԫ�����Ϣ
	void doNormalEvalWithUnit(const CString& outputEvalReportPath, EvalUnitInfoMap& euim);    // ִ�г��������еİ����۵�Ԫ���ֵ�����
	
	// ���ɱ���
	bool generateReport(const CString& outputPath);
	bool writeEvalDataIniFile(const CString& outputPath, bool regulationImpl = false);

	bool checkInput();      // ��һЩ�򵥼��(�Ƿ�ѡ�����ݿ⡢����Ƿ���ڵ�)
	bool checkOutput();     // ��һЩ�򵥼��(�Ƿ�ѡ�����ݿ⡢����Ƿ���ڵ�)
	void cleanTempData(const CString& outputPath);   // ������ʱ����(pj.mdb��ini�ļ�)

	// ���û�ͨ���ʾ����ָ���Ľ�������۽���ϲ�
	// �ϲ����÷�Χ�����۽��״̬--����û��ʵ�֣����۽��--δ֪���
	// �����Ľ������ϲ�
	void mergeEvalResultByUser(
		const CString& evalResultDBPath,
		const CString& regulationDBPath,
		const CString& regulationTable);

	// �ӹ����Ϣ���ݿ��л�ȡ����ĳ�ʼ������
	int getInitQuestionNaireResult(
		CDaoDatabase* pRegulationDB, 
		const CString& regulationTable, 
		int clauseNum);

public:
	afx_msg void OnBnClickedDbPathSelButton();
	afx_msg void OnBnClickedEvalType1Radio();
	afx_msg void OnBnClickedEvalType2Radio();
	afx_msg void OnLvnItemchangedRegulationList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOutputButton();
	afx_msg void OnBnClickedInitBtn();
};
