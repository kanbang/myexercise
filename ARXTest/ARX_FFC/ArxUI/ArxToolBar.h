#pragma once

/*
ʹ�÷�����
ArxToolBar toolbar;
toolbar.AddToolBar(_T("ͨ��ͼԪ"));
toolbar.AddToolButton(_T("���"), _T("JL.DrawTunnel"), _T("�������"), _T("1.bmp"));
toolbar.AddToolButton(_T("����"), _T("JL.DrawFan"), _T("���Ʒ���"), _T("2.bmp"));
toolbar.AddSeparator();
toolbar.AddToolButton(_T("���վ"), _T("JL.DrawStation"), _T("���Ʋ��վ"), _T("3.bmp"));
toolbar.AddToolBar(_T("��˹ͼԪ"));
toolbar.AddToolButton(_T("��ú���"), _T("JL.DrawCoalHole"), _T("���Ƽ�ú���"), _T("4.bmp"));
toolbar.CreateToolBars();
*/

#include "acadi.h"

// ��������ť
struct CADToolButton
{
	TCHAR name[256];
	TCHAR help[256];
	TCHAR order[256];
	TCHAR bmp[256];
};

class ArxToolBar
{
public:
	ArxToolBar();
	virtual ~ArxToolBar();

	// ��ӹ�����
	void AddToolBar(const TCHAR* toolBarName);
	// ��Ӱ�ť��buttonName-�˵���"--"Ϊ�ָ���,commandҪ�ӿո�
	void AddToolButton(const TCHAR* name, const TCHAR* command, const TCHAR* help = NULL, const TCHAR* bmp= NULL);
	// ��ӷָ���
	void AddSeparator();

	// ����������
	void CreateToolBars(const TCHAR* bmpDir);
	// ɾ��������
	void CleanUpToolBars();

	static bool GetAcadApplication(IAcadApplication *&pAcadApp);
	static bool GetAcadToolBar(IAcadToolbar** pVal, LPCTSTR menuname);
	static bool GetAcadApplication(LPDISPATCH * pVal);
	static bool GetAcadMenuGroup(IAcadMenuGroup  **pVal);

	static void ClearToolBar(IAcadToolbar *toolbar);

private:

	class CCADToolButtonArray : public CArray <CADToolButton*, CADToolButton*>
	{
	public:
		int AddButton(CADToolButton& button);
		int AddButton(const TCHAR* name, const TCHAR* command, const TCHAR* help, const TCHAR* bmp);
		~CCADToolButtonArray();
	};

	CArray <CCADToolButtonArray*, CCADToolButtonArray*> m_menus;
	int m_menuCount;  // �˵�����

	CStringArray m_TopMenuNames; // �˵���
};
