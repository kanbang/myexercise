#include "StdAfx.h"
#include "LinkedGEModifiedReactor.h"

#include "../MineGE/LinkedGE.h"
#include "../ArxHelper/HelperClass.h"

LinkedGEModifiedReactor::LinkedGEModifiedReactor ( AcDbDatabase* pDb ) : AcDbDatabaseReactor(), mpDatabase( pDb )
{
    if ( pDb )
    {
        //acutPrintf(_T("\nMineGEErase_DbReactor : %ld"), (long)pDb);
        pDb->addReactor ( this ) ;
    }
}

LinkedGEModifiedReactor::~LinkedGEModifiedReactor ()
{
    Detach () ;
}

void LinkedGEModifiedReactor::Attach ( AcDbDatabase* pDb )
{
    Detach () ;
    if ( mpDatabase == NULL )
    {
        if ( ( mpDatabase = pDb ) != NULL )
            pDb->addReactor ( this ) ;
    }
}

void LinkedGEModifiedReactor::Detach ()
{
    if ( mpDatabase )
    {
        mpDatabase->removeReactor ( this ) ;
        mpDatabase = NULL ;
    }
}

AcDbDatabase* LinkedGEModifiedReactor::Subject () const
{
    return ( mpDatabase ) ;
}

bool LinkedGEModifiedReactor::IsAttached () const
{
    return ( mpDatabase != NULL ) ;
}

void LinkedGEModifiedReactor::objectOpenedForModify( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectOpenedForModify ( dwg, dbObj );
    if( dbObj->isKindOf( LinkedGE::desc() ) )
    {
        LinkedGE* pEdge = LinkedGE::cast( dbObj );
        m_objId = pEdge->objectId();
        pEdge->getSEPoint( m_spt, m_ept );
    }
}

// 在objectModified中无法启动事务
void LinkedGEModifiedReactor::objectModified( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    // 长期的想法
    // 巷道等发生变化，风流方向、风门、风机等也会随之变化
    // 暂时不需要这么高级的功能

    // 临时的想法
    // 变化的时候只考虑风流方向是否需要调整
    // 例如删除风流方向
    AcDbDatabaseReactor::objectModified ( dwg, dbObj );
    if( dbObj->isKindOf( LinkedGE::desc() ) )
    {
        LinkedGE* pEdge = LinkedGE::cast( dbObj );
        if( pEdge->objectId() == m_objId )
        {
            // 上一次记录的id
        }
    }
}