#include "StdAfx.h"
#include "ComGE.h"

#include "ArxDataTool.h"
#include "ArxEntityHelper.h"

#include <set>


static bool MergeExtents( const AcDbObjectIdArray& objIds, AcDbExtents& extents )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    int count = 0;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 ) continue;

        AcDbExtents ext;
        if( Acad::eOk == pEnt->getGeomExtents( ext ) )
        {
            //extents.addExt(ext); // �ϲ�
            extents.addPoint( ext.minPoint() );
            extents.addPoint( ext.maxPoint() );
            count++; // ��¼��ȷ�Ĵ���
        }
    }
    actrTransactionManager->endTransaction();
    return ( count != 0 );
}

static void FindAllSections( const AcDbObjectId& thisObjId, AcDbObjectIdArray& allSections )
{
    // �������е�ComGE
    AcDbObjectIdArray allObjIds;
    ArxDataTool::GetEntsByType( _T( "ComGE" ), allObjIds );

    // ��¼����ComGE�а�����section
    // �ų���ǰ���ڲ�����ComGEͼԪ
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( allObjIds[i] == thisObjId ) continue;

        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        ComGE* pCom = ComGE::cast( pObj );
        if( pCom == 0 ) continue;

        AcDbObjectIdArray tempObjIds;
        pCom->getAllEnts( tempObjIds );
        allSections.append( tempObjIds );
    }
    actrTransactionManager->endTransaction();
}

Adesk::UInt32 ComGE::kCurrentVersionNumber = 1 ;

// ���޸ģ�ʹ��ComGE��Ϊ������
ACRX_NO_CONS_DEFINE_MEMBERS ( ComGE, MineGE )

ComGE::ComGE( void )
{
}

void ComGE::showEnt( const AcDbObjectId& objId, bool isVisible )
{
    assertWriteEnabled();

    ArxEntityHelper::ShowEntity( objId, isVisible );
}

bool ComGE::appendEnt( const AcDbObjectId& objId )
{
    assertWriteEnabled();

    // objId������Ч
    if( objId.isNull() ) return false;
    // �����AcDbEntity����(���ӻ�ͼԪ)
    if( !ArxDataTool::IsEqualType( _T( "AcDbEntity" ), objId ) ) return false;

    showEnt( objId, false );         // ����ͼԪ
    simpleAppendEnt( objId );

    return true;
}

void ComGE::getAllEnts( AcDbObjectIdArray& objIds ) const
{
    assertReadEnabled();

    objIds.append( m_geIds );
}

void ComGE::simpleAppendEnt( const AcDbObjectId& objId )
{
    assertWriteEnabled();
    m_geIds.append( objId );
}

void ComGE::removeAllEnts()
{
    assertWriteEnabled();
    m_geIds.removeAll();
}

void ComGE::pushKeyParamToWriter( DrawParamWriter& writer ) const
{

}

void ComGE::pullKeyParamFromReader( DrawParamReader& reader )
{

}

Acad::ErrorStatus ComGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;

    acutPrintf( _T( "\nCompositeGE::dwgOutFields" ) );

    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    if ( ( es = pFiler->writeUInt32 ( ComGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // д��section��Ϣ
    int len = m_geIds.length();
    pFiler->writeInt32( len ); // int->long
    for( int i = 0; i < len; i++ )
    {
        pFiler->writeHardPointerId( m_geIds[i] );
    }

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ComGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;

    acutPrintf( _T( "\nCompositeGE::dwgInFields" ) );

    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ComGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    m_geIds.setLogicalLength( 0 );

    int len;
    AcDbHardPointerId objId;
    pFiler->readInt32( ( long* )&len );
    for( int i = 0; i < len; i++ )
    {
        pFiler->readItem( &objId );
        m_geIds.append( objId );
    }

    return ( pFiler->filerStatus () ) ;
}

Adesk::Boolean ComGE::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    //acutPrintf(_T("\n����ComGE,����:%d"), m_geIds.length());
    ArxEntityHelper::DrawEntities( m_geIds, mode );

    // ����Ҫ���û����subWorldDraw����
    // ��ʼ�����ݶ���
    return MineGE::subWorldDraw( mode );
}

Acad::ErrorStatus ComGE::subTransformBy( const AcGeMatrix3d& xform )
{
    ArxEntityHelper::TransformEntities( m_geIds, xform );

    return MineGE::subTransformBy( xform );
}

Acad::ErrorStatus ComGE::subErase( Adesk::Boolean erasing )
{
    Acad::ErrorStatus retCode = MineGE::subErase( erasing );
    if( Acad::eOk != retCode ) return retCode;

    AcDbObjectIdArray objIds;
    findEntsNotInOtherComGEs( m_geIds, objIds );

    ArxEntityHelper::EraseObjects2( objIds, erasing );

    return Acad::eOk;
}

void ComGE::decomposeAllEnts()
{
    assertWriteEnabled();

    AcDbObjectIdArray objIds;
    findEntsNotInOtherComGEs( m_geIds, objIds );

    // ��ʾ���е���ͼԪ
    ArxEntityHelper::ShowEntities( objIds, true );

    // ���
    m_geIds.removeAll();
}

Acad::ErrorStatus ComGE::subGetGeomExtents( AcDbExtents& extents ) const
{
    assertReadEnabled () ;

    //Acad::ErrorStatus es = MineGE::subGetGeomExtents(extents);
    Acad::ErrorStatus es = Acad::eOk;
    if( es == Acad::eOk )
    {
        AcDbExtents ext;
        if( MergeExtents( m_geIds, ext ) )
        {
            //extents.addExt(ext); // ò����Ч???
            extents.addPoint( ext.minPoint() );
            extents.addPoint( ext.maxPoint() );
        }
    }
    return es;
}

void ComGE::findEntsNotInOtherComGEs( const AcDbObjectIdArray& geIds, AcDbObjectIdArray& objIds )
{
    AcDbObjectIdArray allSections;
    FindAllSections( this->objectId(), allSections );
    if( allSections.isEmpty() )
    {
        objIds.append( geIds );
        return;
    }

    // ������ѯ��ǰComGE�е�����section
    // ���Ҳ��غϵĲ���
    int len = geIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( !allSections.contains( geIds[i] ) )
        {
            objIds.append( geIds[i] );
        }
    }
}
