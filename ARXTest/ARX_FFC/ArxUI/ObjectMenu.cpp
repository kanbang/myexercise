#include "StdAfx.h"
#include "ObjectMenu.h"
#include "ObjectContextMenu.h"

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

ObjectMenu::ObjectMenu( long id )
{
    m_pObjMenu = new ObjectContextMenu( id );
}

ObjectMenu::~ObjectMenu( void )
{
    ClearObjectMenu();
    delete m_pObjMenu;
}

void ObjectMenu::RegMenuCmd( long id, const CString& cmd )
{
    m_pObjMenu->regMenuCmd( id, cmd );
}

bool ObjectMenu::CreateObjectMenu( const void* appId )
{
    CAcModuleResourceOverride myResources;
    return RegisterObjectContextMenuByClassName( m_className, m_pObjMenu, appId );
}

bool ObjectMenu::ClearObjectMenu()
{
    CAcModuleResourceOverride myResources;
    return UnRegisterObjectContexMenuByClassName( m_className, m_pObjMenu );
}

void ObjectMenu::SetObjectType( const CString& className )
{
    m_className = className;
}