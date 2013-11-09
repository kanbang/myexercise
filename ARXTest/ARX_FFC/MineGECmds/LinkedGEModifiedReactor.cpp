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

// ��objectModified���޷���������
void LinkedGEModifiedReactor::objectModified( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    // ���ڵ��뷨
    // ����ȷ����仯���������򡢷��š������Ҳ����֮�仯
    // ��ʱ����Ҫ��ô�߼��Ĺ���

    // ��ʱ���뷨
    // �仯��ʱ��ֻ���Ƿ��������Ƿ���Ҫ����
    // ����ɾ����������
    AcDbDatabaseReactor::objectModified ( dwg, dbObj );
    if( dbObj->isKindOf( LinkedGE::desc() ) )
    {
        LinkedGE* pEdge = LinkedGE::cast( dbObj );
        if( pEdge->objectId() == m_objId )
        {
            // ��һ�μ�¼��id
        }
    }
}