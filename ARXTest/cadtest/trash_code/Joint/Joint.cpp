#include "StdAfx.h"
#include "Joint.h"

Adesk::UInt32 Joint::kCurrentVersionNumber = 1 ;

//ACRX_CONS_DEFINE_MEMBERS(Joint, MineGE, 1)
ACRX_DXF_DEFINE_MEMBERS (
    Joint, MineGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    JOINT, DEFGEAPP
)

Joint::Joint ()
{
}

Joint::Joint( const AcGePoint3d& insertPt ) : m_insertPt( insertPt )
{
}

Joint::~Joint ()
{
}

void Joint::setInsertPoint( const AcGePoint3d& insertPt )
{
    assertWriteEnabled();

    m_insertPt = insertPt;
}

AcGePoint3d Joint::getInsertPoint() const
{
    assertReadEnabled();

    return m_insertPt;
}

void Joint::pushKeyParamToWriter( DrawParamWriter& writer ) const
{
    assertReadEnabled();

    writer.writePoint( m_insertPt );
}

void Joint::pullKeyParamFromReader( DrawParamReader& reader )
{
    assertWriteEnabled();

    reader.readPoint( m_insertPt );
}

Acad::ErrorStatus Joint::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( Joint::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // 写入插入点坐标
    pFiler->writeItem( m_insertPt );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus Joint::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > Joint::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;
    //- Uncomment the 2 following lines if your current object implementation cannot
    //- support previous version of that object.
    //if ( version < Chimney::kCurrentVersionNumber )
    //	return (Acad::eMakeMeProxy) ;

    // 读取插入点坐标
    pFiler->readItem( &m_insertPt );

    return ( pFiler->filerStatus () ) ;
}