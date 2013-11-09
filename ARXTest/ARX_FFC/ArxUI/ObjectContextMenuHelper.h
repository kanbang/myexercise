#pragma once

#include "dlimexp.h"
#include "ObjectContextMenu.h"

/*
 右键对象菜单辅助类
 使用方法：
 // 1) 创建对象菜单对象，传入对象类型名称和菜单资源ID
 ObjectContextMenu* pObjMenu = new ObjectContextMenu(_T("AcDbLine"), UI_LOADER_MNU_OBJECT_WS);

 // 2) 关联菜单项和名称
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS1, _T("test1"));
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS2, _T("test2"));
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS3, _T("test3"));
 pObjMenu->regCmd(UI_LOADER_MNU_ADVANCE_WS4, _T("test4"));

 // 3) 注册菜单
 // appId参数有kLoadAppMsg消息传入(pkt)
 ObjectContextMenuHelper::RegObjectContextMenu(pObjMenu, appId);

 // 4) 如果要卸载菜单，请调用UnRegObjectContextMenu()方法

 */
class ARXUI_DLLIMPEXP ObjectContextMenuHelper
{
public:
	// 注册对象菜单
	static bool RegObjectContextMenu(const CString& className, ObjectContextMenu* pObjMenu, const void *appId);
	// 删除对象菜单
	static bool UnRegObjectContextMenu(const CString& className, ObjectContextMenu* pObjMenu);
};
