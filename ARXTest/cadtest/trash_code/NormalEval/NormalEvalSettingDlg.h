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

struct EvalUnitInfo                  // 评价单元信息
{
	CString parent_unit;             // 父评价单元
	ClauseCollection clauses;        // 条款编号
	int hierarchy_num;               // 层次编号
};

// 常规评价对话框
class NormalEvalSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(NormalEvalSettingDlg)

public:
	NormalEvalSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NormalEvalSettingDlg();

// 对话框数据
	enum { IDD = IDD_NORMAL_EVAL_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	void initRegulationListCtrl();                         // 初始化list ctrl
	void readRegulationInfoFromDB();                       // 从数据库中读取数据

	void initEvalUnitTreeCtrl();                           // 初始化treectrl
	void readEvalUnitFromDB(const CString& tableName);     // 从数据库中读取评价单元分类

	// 辅助方法
	// 构造评价单元的树状结构
	void initEvalUntiHierarchy(const CString& tableName, EvalUnitInfoMap& euim);
	void caclEvalUnitHierarchy(EvalUnitInfoMap& euim);
	void buildTree(EvalUnitInfoMap& euim);                 // 向treectrl中插入数据，构建树结构
	void buildSubTree(EvalUnitInfoMap& euim, TV_INSERTSTRUCT tvinsert, const CString& parent_unit, int hierarchy_num);
	void insertLeafNode(EvalUnitInfoMap& euim, TV_INSERTSTRUCT tvinsert, const CString& parent_unit);	
	void visitTree(EvalUnitInfoMap& euim, CTreeCtrl& tree, HTREEITEM hItem);      // 遍历treectrl，得到选择的评价单元层次信息

	CString getSelRegulationTableName();                  // 获取当前选择的规程的数据库表的名称
	void getClauseCollection(ClauseCollection& clauses);  // 获取当前规程中的所有条款编号
	void doNormalEvalWithUnit_firstStep(LPCTSTR dbPath, const CString& regulationTable, EvalUnitInfoMap& euim);
	void doNormalEvalWithUnit_secondStep(CDaoDatabase* pDB, EvalUnitInfoMap& euim, EvalResultMap& er_map);
	void doNormalEvalWithUnit_thirdStep(CDaoDatabase* pDB, EvalResultMap& er_map);

	int getEvalMethodType();                                           // 获取评价方法类型
	void doNormalEvalWithoutUnit(const CString& outputEvalReportPath); // 执行常规评价中的逐条评价
	void getSelEvalUnitMap(EvalUnitInfoMap& euim);                     // 遍历treectrl，得到选择的评价单元层次信息
	void doNormalEvalWithUnit(const CString& outputEvalReportPath, EvalUnitInfoMap& euim);    // 执行常规评价中的按评价单元划分的评价
	
	// 生成报告
	bool generateReport(const CString& outputPath);
	bool writeEvalDataIniFile(const CString& outputPath, bool regulationImpl = false);

	bool checkInput();      // 做一些简单检查(是否选择数据库、规程是否存在等)
	bool checkOutput();     // 做一些简单检查(是否选择数据库、规程是否存在等)
	void cleanTempData(const CString& outputPath);   // 清理临时数据(pj.mdb和ini文件)

	// 将用户通过问卷调查指定的结果与评价结果合并
	// 合并适用范围：评价结果状态--条款没有实现；评价结果--未知结果
	// 其它的结果不予合并
	void mergeEvalResultByUser(
		const CString& evalResultDBPath,
		const CString& regulationDBPath,
		const CString& regulationTable);

	// 从规程信息数据库中获取条款的初始调查结果
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
