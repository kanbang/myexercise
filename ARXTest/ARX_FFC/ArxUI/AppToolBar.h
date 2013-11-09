#pragma once

#include "dlimexp.h"

class ArxToolBar;

class ARXUI_DLLIMPEXP AppToolBar
{
public:
	AppToolBar(void);
	~AppToolBar(void);

	// 添加工具栏
	void AddToolBar(const TCHAR* toolBarName);
	// 添加按钮，buttonName-菜单名"--"为分隔符,command要加空格
	void AddToolButton(const TCHAR* name, const TCHAR* command, const TCHAR* help = NULL, const TCHAR* bmp= NULL);
	// 添加分隔符
	void AddSeparator();

	// 创建工具栏
	// bmpDir -- 图标所在的文件夹路径
	void CreateToolBars(const TCHAR* bmpDir);

private:
	// 删除工具栏
	void CleanUpToolBars(); //创建了Menu一定要调用这个函数

private:
	ArxToolBar* m_pToolBar;
};
