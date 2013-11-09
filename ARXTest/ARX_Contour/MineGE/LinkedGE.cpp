#include "StdAfx.h"
#include "LinkedGE.h"

#include "DrawHelper.h"

#include "../MineGEDraw/LinkedGEDraw.h"
#include "../MineGEDraw/MineGEDrawSystem.h"

Adesk::UInt32 LinkedGE::kCurrentVersionNumber = 1 ;

// ���޸�
ACRX_NO_CONS_DEFINE_MEMBERS ( LinkedGE, MineGE )

LinkedGE::LinkedGE()
{
}

LinkedGE::LinkedGE( const AcGePoint3d& startPt, const AcGePoint3d& endPt )
    : m_startPt( startPt ), m_endPt( endPt )
{
}

void LinkedGE::setSEPoint( const AcGePoint3d& startPt, const AcGePoint3d& endPt )
{
    assertWriteEnabled();

    m_startPt = startPt;
    m_endPt = endPt;
}

void LinkedGE::getSEPoint( AcGePoint3d& startPt, AcGePoint3d& endPt ) const
{
    assertReadEnabled();

    startPt = m_startPt;
    endPt = m_endPt;
}

void LinkedGE::reverse()
{
    assertWriteEnabled();

    LinkedGEDraw* pCGEDraw = LinkedGEDraw::cast( getCurrentDraw() ); // ���ʸ���MineGE��draw����
    if( pCGEDraw == 0 ) return;

    updateDrawParam( false ); // to draw
    pCGEDraw->reverse();
    updateDrawParam( true ); // back to ge
}

void LinkedGE::dealWithStartPointBoundary( const AcGeRay3d& boundaryLine )
{
    assertWriteEnabled();

    LinkedGEDraw* pCGEDraw = LinkedGEDraw::cast( getCurrentDraw() ); // ���ʸ���MineGE��draw����
    if( pCGEDraw == 0 ) return;

    updateDrawParam( false ); // to draw
    pCGEDraw->dealWithStartPointBoundary( boundaryLine );
    updateDrawParam( true ); // back to ge
}

void LinkedGE::dealWithEndPointBoundary( const AcGeRay3d& boundaryLine )
{
    assertWriteEnabled();

    LinkedGEDraw* pCGEDraw = LinkedGEDraw::cast( getCurrentDraw() );
    if( pCGEDraw == 0 ) return;

    updateDrawParam( false );
    pCGEDraw->dealWithEndPointBoundary( boundaryLine );
    updateDrawParam( true );
}

double LinkedGE::getAngle() const
{
    assertReadEnabled();

    AcGeVector3d v = m_endPt - m_startPt;
    return v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

AcGeVector3d LinkedGE::getStartPointInExtendAngle() const
{
    assertReadEnabled();

    LinkedGEDraw* pCGEDraw = LinkedGEDraw::cast( getCurrentDraw() );
    if( pCGEDraw == 0 )
    {
        return AcGeVector3d::kIdentity; // ������
    }
    else
    {
        updateDrawParam( false ); // to draw
        return pCGEDraw->getStartPointInExtendAngle().normalize(); // ��׼��
    }
}

AcGeVector3d LinkedGE::getEndPointInExtendAngle() const
{
    assertReadEnabled();

    LinkedGEDraw* pCGEDraw = LinkedGEDraw::cast( getCurrentDraw() );
    if( pCGEDraw == 0 )
    {
        return AcGeVector3d::kIdentity; // ������
    }
    else
    {
        updateDrawParam( false ); // to draw
        return pCGEDraw->getEndPointInExtendAngle().normalize(); // ��׼��
    }
}

void LinkedGE::extendByLength( double length )
{
    assertWriteEnabled();

    LinkedGEDraw* pCGEDraw = LinkedGEDraw::cast( getCurrentDraw() );
    if( pCGEDraw == 0 ) return;

    updateDrawParam( false ); // to draw
    pCGEDraw->extendByLength( length );
    updateDrawParam( true );
}

void LinkedGE::writeKeyParam( DrawParamWriter& writer ) const
{
    //assertReadEnabled();

    writer.writePoint( m_startPt );
    writer.writePoint( m_endPt );
}

void LinkedGE::readKeyParam( DrawParamReader& reader )
{
    //assertWriteEnabled();

    reader.readPoint( m_startPt );
    reader.readPoint( m_endPt );
}

Acad::ErrorStatus LinkedGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;

    //acutPrintf(_T("\nLinkedGE::dwgOutFields"));
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    if ( ( es = pFiler->writeUInt32 ( LinkedGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // ����ʼĩ������
    pFiler->writeItem( m_startPt );
    pFiler->writeItem( m_endPt );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus LinkedGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;

    //acutPrintf(_T("\nLinkedGE::dwgInFields"));
    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > LinkedGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    // ��ȡʼĩ������
    pFiler->readItem( &m_startPt );
    pFiler->readItem( &m_endPt );

    return ( pFiler->filerStatus () ) ;
}

void LinkedGE::doEdgeGEJunctionClosure()
{
    if( !isNewObject() )
    {
        DrawHelper::LinkedGEJunctionClosure( m_startPt );
        DrawHelper::LinkedGEJunctionClosure( m_endPt );
    }
}

Adesk::Boolean LinkedGE::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();
    return MineGE::subWorldDraw( mode );
}

Acad::ErrorStatus LinkedGE::subTransformBy( const AcGeMatrix3d& xform )
{
    //acutPrintf(_T("\nid:%d call LinkedGE::subTransformBy().."), objectId());

    doEdgeGEJunctionClosure(); // ����պ�(��ǰ���ڱ༭��ͼԪ������պϴ���)

    return MineGE::subTransformBy( xform );
}

Acad::ErrorStatus LinkedGE::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    //acutPrintf(_T("\nid:%d call LinkedGE::subMoveGripPointsAt()..."), objectId());

    doEdgeGEJunctionClosure(); // ����պ�(��ǰ���ڱ༭��ͼԪ������պϴ���)

    return MineGE::subMoveGripPointsAt( indices, offset );
}

Acad::ErrorStatus LinkedGE::subErase( Adesk::Boolean erasing )
{
    //acutPrintf(_T("\nid:%d call LinkedGE::subErase()..."), objectId());

    Acad::ErrorStatus retCode = MineGE::subErase ( erasing ) ;

    if( Acad::eOk == retCode )
    {
        doEdgeGEJunctionClosure(); // ����պ�(��ǰ���ڱ༭��ͼԪ������պϴ���)
    }

    return Acad::eOk;
}
