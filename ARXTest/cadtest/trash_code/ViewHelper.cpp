#include "StdAfx.h"
#include "ViewHelper.h"
#include "ArxDictTool.h"
#include "MineGE.h"
#include "config.h"

#include "LayerHelper.h"
#include "GETypeNameMapHelper.h"
#include "DrawHelper.h"

static bool IsMineGEType( const CString& geType )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( geType ) );
    if( pClass == 0 ) return false;
    return pClass->isDerivedFrom( MineGE::desc() );
}

static ArxDictTool* GetDictTool( const CString& dictName )
{
    return new ArxDictTool( dictName );
}

bool ViewHelper::AddView( const CString& view )
{
    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    bool ret = pDictTool->addKey( view );
    delete pDictTool;
    return ret;
}

bool ViewHelper::RemoveView( const CString& view )
{
    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    bool ret = pDictTool->removeKey( view );
    delete pDictTool;
    return ret;
}

bool ViewHelper::AddElement( const CString& view, const CString& element )
{
    // 不是MineGE图元类型
    if( !IsMineGEType( element ) ) return false;

    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    int index = pDictTool->addEntry( view, element );
    delete pDictTool;

    return ( index != 0 );
}

bool ViewHelper::RemoveElement( const CString& view, const CString& element )
{
    // 不是MineGE图元类型
    if( !IsMineGEType( element ) ) return false;

    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    int index = pDictTool->removeEntry( view, element );
    delete pDictTool;

    return ( index != 0 );
}

int ViewHelper::CountElements( const CString& view )
{
    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    int count = pDictTool->countEntries( view );
    delete pDictTool;

    return count;
}

int ViewHelper::FindElement( const CString& view, const CString& element )
{
    // 不是MineGE图元类型
    if( !IsMineGEType( element ) ) return 0;

    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    int index = pDictTool->findEntry( view, element );
    delete pDictTool;

    return index;
}

void ViewHelper::GetAllElements( const CString& view, AcStringArray& elements )
{
    ArxDictTool* pDictTool = GetDictTool( MINE_GE_VIEW_DICT );
    pDictTool->getAllEntries( view, elements );
    delete pDictTool;
}

void ViewHelper::SwitchView( const CString& view )
{
    AcStringArray elements;
    GetAllElements( view, elements );
    if( elements.isEmpty() ) return;

    AcStringArray allGETypes;
    DrawHelper::GetAllRegGETypes( allGETypes );
    if( allGETypes.isEmpty() ) return;

    int len = allGETypes.length();
    for( int i = 0; i < len; i++ )
    {
        CString geType = allGETypes[i].constPtr();
        if( elements.contains( geType ) )
        {
            // 解冻图层
            LayerHelper::FrozenLayer( GETypeNameMapHelper::GetTypeNameMap( geType ), false );
        }
        else
        {
            // 冻结图层
            LayerHelper::FrozenLayer( GETypeNameMapHelper::GetTypeNameMap( geType ), true );
        }
    }
}
