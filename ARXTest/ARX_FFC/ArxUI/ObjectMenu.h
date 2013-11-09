#pragma once

#include "dlimexp.h"

class ObjectContextMenu;

class ARXUI_DLLIMPEXP ObjectMenu
{
public:
	ObjectMenu(long id);
	~ObjectMenu(void);

	// 注册命令和ID
	void RegMenuCmd(long id, const CString& cmd);

	// 设置对象类型名称
	void SetObjectType(const CString& className);

	// 创建对象菜单
	bool CreateObjectMenu(const void* appId);

private:
	// 删除对象菜单
	bool ClearObjectMenu();

private:
	CString m_className;
	ObjectContextMenu* m_pObjMenu;
};
