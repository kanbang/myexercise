#include "StdAfx.h"

#include "DumyJigEntity.h"

static AcDbEntity* CloneEntity( const AcDbObjectId& objId )
{
    AcDbEntity* pEnt;
    acdbOpenObject( pEnt, objId, AcDb::kForRead );

    AcDbEntity* pClone = AcDbEntity::cast( pEnt->clone() );

    pEnt->close();

    return pClone;
}

static void CopyBack( AcDbEntity* pSrcEnt, const AcDbObjectId& objId )
{
    AcDbEntity* pEnt;
    acdbOpenAcDbEntity( pEnt, objId, AcDb::kForWrite );
    pEnt->copyFrom( pSrcEnt );
    pEnt->close();
}

void DumyJigEntity::addEntity( const AcDbObjectId& objId )
{
    AcDbEntity* pEnt = CloneEntity( objId );
    if( pEnt != 0 )
    {
        m_pEntArray.append( pEnt );
        m_objIds.append( objId );
    }
}

AcDbEntity* DumyJigEntity::getEnity( int pos ) const
{
    return ( AcDbEntity* )m_pEntArray[pos];
}

DumyJigEntity::~DumyJigEntity()
{
    int n = m_pEntArray.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbEntity* pEnt = ( AcDbEntity* )m_pEntArray[i];
        delete pEnt;
    }
    m_pEntArray.removeAll();
    m_objIds.removeAll();
}

void DumyJigEntity::writeBack()
{
    int n = m_pEntArray.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbEntity* pEnt = ( AcDbEntity* )m_pEntArray[i];
        CopyBack( pEnt, m_objIds[i] );
    }
}

Adesk::Boolean DumyJigEntity::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    int n = m_pEntArray.length();
    for( int i = 0; i < n; i++ )
    {
        // 根据arx文档中AcGiGeometry::draw()的说明
        // 如果实体包含其它对象，应该使用draw()方法，而不是worldDraw()
        // draw()方法可以缓存图形(cache the graphics of the object, 不知道什么意思???)
        AcDbEntity* pEnt = ( AcDbEntity* )m_pEntArray[i];
        pEnt->worldDraw( mode );
        //mode->geometry().draw(pEnt);
    }

    return Adesk::kTrue;
}
