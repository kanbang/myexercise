#include "StdAfx.h"
#include "ObjectContextMenuHelper.h"

// ȫ�ֶ���˵�����
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

    return acedAddObjectContextMenu( pClass, pContext , appId ); //  ��AutoCADӦ�������ʵ����������Ĳ˵�
}

static bool UnRegisterObjectContexMenuByClassName( const CString& className, AcEdUIContext* pContext )
{
    AcRxClass* pClass = GetArxClass( className );
    if( pClass == 0 ) return false;

    return acedRemoveObjectContextMenu( pClass, pContext ); // �Ƴ�ʵ����������Ĳ˵�
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

// �������룬������ѧϰ
void UIHelper::RegisterObjectContextMenu(const void *appId)
{
// 1) �����ȴ����˵������Ķ���
// ��Ϊ���캯���н�������Դ�л�
gp_object_ws_context_menu = new WSContextMenu();


// 2) ע�������Ĳ˵�
CAcModuleResourceOverride myResources; // ��pushResourceHandle��popResourceHandle�ǵȼ۵�

//acedAddDefaultContextMenu(gpDefDemoCM, pAppID);                 // ��AutoCADӦ�������Ĭ�������Ĳ˵�
//acedAddObjectContextMenu(AcDbEntity::desc(), gpEntDemoCM , appId); //  ��AutoCADӦ�������ʵ����������Ĳ˵�
// ��AutoCADӦ�����������ʱ�����Ĳ˵�
// myCmd��һ�����������һ�������������������ڶ���������ȫ����������
// �����������Ǳ��������������ĸ�����������ģʽ����5��6�������Ͳ���˵�ˣ����Ӧ�������ˡ�:-0
//acedRegCmds->addCommand("MyGrp", "MyDemo", "MyDemo", ACRX_CMD_MODAL, &myCmd, gpCmdDemoCM );

RegisterObjectContextMenuByClassName(_T("AcDbLine"), gp_object_ws_context_menu, appId);
}

void UIHelper::UnRegisterObjectContextMenu()
{
CAcModuleResourceOverride myResources;

//acedRemoveDefaultContextMenu(gpDefDemoCM ); // �Ƴ�Ĭ�������Ĳ˵�
//acedRemoveObjectContextMenu(AcDbEntity::desc(), gpEntDemoCM ); // �Ƴ�ʵ����������Ĳ˵�
//acedRegCmds->removeGroup("MyGrp"); // �Ƴ�������"MyGrp"

//delete gpDefDemoCM;
//delete gpEntDemoCM;
//delete gpCmdDemoCM;

// WorkSurface
UnRegisterObjectContexMenuByClassName(_T("AcDbLine"), gp_object_ws_context_menu);
delete gp_object_ws_context_menu;
gp_object_ws_context_menu = 0;

}

*/