#pragma once

#include "dlimexp.h"

class ARXUI_DLLIMPEXP ObjectContextMenu :public AcEdUIContext
{
public:
	ObjectContextMenu(long id);
	~ObjectContextMenu(void);
	virtual void* getMenuContext(const AcRxClass *pClass, const AcDbObjectIdArray& ids) ;
	virtual void onCommand(Adesk::UInt32 cmdIndex);
	//virtual void OnUpdateMenu();

public:
	// 注册命令和ID
	void regMenuCmd(long id, const CString& cmd);

private:
	CMenu *m_pMenu;    // 用来增加菜单项的MFC菜单对象，使用它是为了加载我们在VC中增加的菜单资源。
	HMENU m_hDemoMenu; // 菜单项所对应的句柄，这才是我们真正要加载的的菜单项，它是m_pDemoMenu中的一项。

	// 后续增加的字段
private:
	CString m_className;    // 对象类型名称
	long m_menu_id;         // 菜单的资源ID
	AcGeIntArray m_cmdIds;  // 菜单项id
	AcStringArray m_cmds;   // 菜单对应的命令
};