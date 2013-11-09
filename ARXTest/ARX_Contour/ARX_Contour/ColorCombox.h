#pragma once

// 如下3个Combox控件属性必须设置为：
//	1) owner draw -- fixed或variable
//	2) has string -- true (如果不设置，则会出现文字乱码)
//	3) type -- drop list
// 在代码中设置，或者在控件属性中设置
class CColorCombox : public CComboBox
{
public:
	CColorCombox();
	virtual ~CColorCombox();

	void InitColorCB();
	COLORREF GetCurColor();
	void SetCurColor(COLORREF clr);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	int AddItem(LPCTSTR lpszText, COLORREF clrValue);

protected:
	//{{AFX_MSG(CColorCombox)
	afx_msg void OnSelendok();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
