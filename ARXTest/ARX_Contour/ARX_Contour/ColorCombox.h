#pragma once

// ����3��Combox�ؼ����Ա�������Ϊ��
//	1) owner draw -- fixed��variable
//	2) has string -- true (��������ã���������������)
//	3) type -- drop list
// �ڴ��������ã������ڿؼ�����������
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
