#pragma once
#include "afxcmn.h"

#include "resource.h"
#include "ListCtrlEx.h"
#include "ColorCombox.h"
#include "afxwin.h"

// �����ֵ��Ϊ1.1  1.3  1.5,��Ӧ����4����ɫ����<1.1  1.1~1.3  1.3~1.5 >1.5
// zValues = {1.1, 1.3, 1.5}   colors={<1.1, 1.1~1.3, 1.3~1.5, >1.5}
// Ϊ�˷���ʹ��, ��չzValues==>{1.1, 1.3, 1.5, 1.7}
// ��colors���б任 ,ʹ��<�������ʾ==> colors={<1.1, <1.3, <1.5, <1.7}, ��֮ǰ�ı�ʾ�ǵȼ۵�
// ԭ��: ��ΪContourģ���������ı߽��zi���ܻ����zֵ���������λ��(���������Ϳ���zi=3)
// Ϊ�˱�֤zi=3Ҳ�ܹ��õ���ɫֵ�������ϵĴ���
class ContourColorDlg : public CDialog
{
	DECLARE_DYNAMIC(ContourColorDlg)

public:
	ContourColorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ContourColorDlg();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeColorCombox();
	afx_msg void OnCbnSelchangeCscLayerList();
	afx_msg void OnBnClickedFillColorCheckbox();
};
