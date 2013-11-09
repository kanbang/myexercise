#pragma once
#include "afxcmn.h"

#include "resource.h"
#include "ListCtrlEx.h"
#include "ColorCombox.h"
#include "afxwin.h"

// 假设等值线为1.1  1.3  1.5,则应该有4个颜色区间<1.1  1.1~1.3  1.3~1.5 >1.5
// zValues = {1.1, 1.3, 1.5}   colors={<1.1, 1.1~1.3, 1.3~1.5, >1.5}
// 为了方便使用, 扩展zValues==>{1.1, 1.3, 1.5, 1.7}
// 对colors进行变换 ,使用<的语义表示==> colors={<1.1, <1.3, <1.5, <1.7}, 与之前的表示是等价的
// 原因: 因为Contour模块计算出来的边界的zi可能会大于z值的最大索引位置(比如上例就可能zi=3)
// 为了保证zi=3也能够得到颜色值，做如上的处理
class ContourColorDlg : public CDialog
{
	DECLARE_DYNAMIC(ContourColorDlg)

public:
	ContourColorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ContourColorDlg();

// 对话框数据
	enum { IDD = IDD_CONTOUR_COLOR_DLG };

private:
	CComboBox m_layerList;	
	CListCtrlEx m_colorList;
	CColorCombox m_colorCombox;
	BOOL m_bFillColor;

	bool GetLayer(CString& layer);
	int GetColorListCurSel() const;
	void FillLayerList();
	void FillColorCombox();
	void AddColorListColumns();
	void FillColorList(const AcGeDoubleArray& zValues, const AcArray<COLORREF>& colors);
	void DeleteColorListItemDatas();
	void enableColorCtrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeColorCombox();
	afx_msg void OnCbnSelchangeCscLayerList();
	afx_msg void OnBnClickedFillColorCheckbox();
};
