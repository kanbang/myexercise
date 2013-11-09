#include "StdAfx.h"
#include "ObjectContextMenuHelper.h"

// 全局对象菜单数组
//static AcGeVoidPointerArray global_object_context_menu_array;

static AcRxClass* GetArxClass( const CString& className )
{
    return AcRxClass::cast( acrxClassDictionary->at( className ) );
}

static bool RegisterObjectContextMenuByClassName( const CString& className, AcEdUIContext* pContext, const void* appId )
{
    if( pContext == 0 ) return false;

    AcRxClass* pClass = GetArxClass( className );
    if( pClass == 0 ) return false;

    return acedAddObjectContextMenu( pClass, pContext , appId ); //  向AutoCAD应用中添加实体对象上下文菜单
}

static bool UnRegisterObjectContexMenuByClassName( const CString& className, AcEdUIContext* pContext )
{
    AcRxClass* pClass = GetArxClass( className );
    if( pClass == 0 ) return false;

    return acedRemoveObjectContextMenu( pClass, pContext ); // 移除实体对象上下文菜单
}

bool ObjectContextMenuHelper::RegObjectContextMenu( const CString& className, ObjectContextMenu* pObjMenu, const void* appId )
{
    if( pObjMenu == 0 ) return false;

    CAcModuleResourceOverride myResources;
    return RegisterObjectContextMenuByClassName( pObjMenu->getClassName(), pObjMenu, appId );
}

bool ObjectContextMenuHelper::UnRegObjectContextMenu( ObjectContextMenu* pObjMenu )
{
    if( pObjMenu == 0 ) return false;

    CAcModuleResourceOverride myResources;
    return UnRegisterObjectContexMenuByClassName( pObjMenu->getClassName(), pObjMenu );
}

//void ObjectContextMenuHelper::UnRegAllObjectContextMenu()
//{
//	int n = global_object_context_menu_array.length();
//	for(int i=0;i<n;i++)
//	{
//		CAcModuleResourceOverride myResources;
//		ObjectContextMenu* pObjMenu = (ObjectContextMenu*)global_object_context_menu_array[i];
//		UnRegisterObjectContexMenuByClassName(pObjMenu->getClassName(), pObjMenu);
//		delete pObjMenu;
//	}
//	global_object_context_menu_array.removeAll();
//}


/*

// 残留代码，仅用于学习
void UIHelper::RegisterObjectContextMenu(const void *appId)
{
// 1) 必须先创建菜单上下文对象
// 因为构造函数中进行了资源切换
gp_object_ws_context_menu = new WSContextMenu();


// 2) 注册上下文菜单
CAcModuleResourceOverride myResources; // 与pushResourceHandle和popResourceHandle是等价的

//acedAddDefaultContextMenu(gpDefDemoCM, pAppID);                 // 向AutoCAD应用中添加默认上下文菜单
//acedAddObjectContextMenu(AcDbEntity::desc(), gpEntDemoCM , appId); //  向AutoCAD应用中添加实体对象上下文菜单
// 向AutoCAD应用中添加命令时上下文菜单
// myCmd是一个命令函数。第一个参数是命令组名，第二个参数是全局命令名，
// 第三个参数是本地命令名，第四个参数是命令模式，第5和6个参数就不用说了，大家应该明白了。:-0
//acedRegCmds->addCommand("MyGrp", "MyDemo", "MyDemo", ACRX_CMD_MODAL, &myCmd, gpCmdDemoCM );

RegisterObjectContextMenuByClassName(_T("AcDbLine"), gp_object_ws_context_menu, appId);
}

void UIHelper::UnRegisterObjectContextMenu()
{
CAcModuleResourceOverride myResources;

//acedRemoveDefaultContextMenu(gpDefDemoCM ); // 移除默认上下文菜单
//acedRemoveObjectContextMenu(AcDbEntity::desc(), gpEntDemoCM ); // 移除实体对象上下文菜单
//acedRegCmds->removeGroup("MyGrp"); // 移除命令组"MyGrp"

//delete gpDefDemoCM;
//delete gpEntDemoCM;
//delete gpCmdDemoCM;

// WorkSurface
UnRegisterObjectContexMenuByClassName(_T("AcDbLine"), gp_object_ws_context_menu);
delete gp_object_ws_context_menu;
gp_object_ws_context_menu = 0;

}

*/