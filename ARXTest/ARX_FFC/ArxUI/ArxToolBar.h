#pragma once

/*
使用方法：
ArxToolBar toolbar;
toolbar.AddToolBar(_T("通风图元"));
toolbar.AddToolButton(_T("巷道"), _T("JL.DrawTunnel"), _T("绘制巷道"), _T("1.bmp"));
toolbar.AddToolButton(_T("风门"), _T("JL.DrawFan"), _T("绘制风门"), _T("2.bmp"));
toolbar.AddSeparator();
toolbar.AddToolButton(_T("测风站"), _T("JL.DrawStation"), _T("绘制测风站"), _T("3.bmp"));
toolbar.AddToolBar(_T("瓦斯图元"));
toolbar.AddToolButton(_T("见煤钻孔"), _T("JL.DrawCoalHole"), _T("绘制见煤钻孔"), _T("4.bmp"));
toolbar.CreateToolBars();
*/

#include "acadi.h"

// 工具栏按钮
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

	// 添加工具栏
	void AddToolBar(const TCHAR* toolBarName);
	// 添加按钮，buttonName-菜单名"--"为分隔符,command要加空格
	void AddToolButton(const TCHAR* name, const TCHAR* command, const TCHAR* help = NULL, const TCHAR* bmp= NULL);
	// 添加分隔符
	void AddSeparator();

	// 创建工具栏
	void CreateToolBars(const TCHAR* bmpDir);
	// 删除工具栏
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
	int m_menuCount;  // 菜单个数

	CStringArray m_TopMenuNames; // 菜单名
};
