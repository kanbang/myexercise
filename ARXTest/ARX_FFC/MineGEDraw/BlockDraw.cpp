#include "StdAfx.h"
#include "BlockDraw.h"
#include "BlockDraw_ConfigDlg.h"

// ���ݿ����ƻ�ȡ�鶨��id
static AcDbObjectId GetBlockDefinitionByName( const CString& blockName )
{
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable( pBlockTable, AcDb::kForRead );

    AcDbBlockTableRecord* pBlockTableRecord;
    Acad::ErrorStatus es = pBlockTable->getAt( blockName, pBlockTableRecord, AcDb::kForRead );
    pBlockTable->close();

    AcDbObjectId objId;
    if( es == Acad::eOk )
    {
        objId = pBlockTableRecord->objectId();
        pBlockTableRecord->close();
    }
    return objId;
}

static void FillAttribute( const AcStringArray& names, const AcStringArray& attValues, AcDbBlockReference& bRef )
{
    if( names.isEmpty() )
    {
        acutPrintf( _T( "\nû��ע��Ҫ��ȡ���ֶ�..." ) );
        return;
    }

    if( names.length() != attValues.length() )
    {
        acutPrintf( _T( "\nע��������������ȡ�����ݸ��������!" ) );
        return;
    }

    // ģ��cad�Ŀ�������
    // �������е����Զ��壬����֮Ϊģ�壬�������Զ���
    // ����ӵ�AcDbBlockReference��
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, bRef.blockTableRecord(), AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcDbBlockTableRecord* pBTR = AcDbBlockTableRecord::cast( pObj );
    // BUG�����ܵ���hasAttributeDefinitions()����
    // ����֮�����û���ڿ�༭���жԿ�����޸ģ�
    // ��ô�����ƶ����е�༭�Ȳ�����û�ж�̬��ʾЧ��
    //if(!pBTR->hasAttributeDefinitions())
    //{
    //	// û�����Զ���
    //	acutPrintf(_T("\nû�����Զ���"));
    //	actrTransactionManager->abortTransaction();
    //	return;
    //}

    AcDbBlockTableRecordIterator* pIterator;
    if( Acad::eOk != pBTR->newIterator( pIterator ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcGeMatrix3d blkXform = bRef.blockTransform();

    // �������е�ͼԪ������AcDbAttributeDefinition
    for( pIterator->start( true ); !pIterator->done(); pIterator->step( true ) )
    {
        AcDbObjectId objId;
        if( Acad::eOk != pIterator->getEntityId( objId ) ) continue;
        if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) ) continue;
        AcDbAttributeDefinition* pAttDef = AcDbAttributeDefinition::cast( pObj );
        if( pAttDef == 0 ) continue;

        // ��ȡ��ǩ����
        ACHAR* pTag = pAttDef->tag();
        int pos = names.find( pTag );
        if( pos != -1 )
        {
            // ��������
            AcDbAttribute* pAtt = new AcDbAttribute();
            pAtt->setAttributeFromBlock( pAttDef, blkXform );

            /*
             * ���Ե�ͼ�����������Զ��屣��һ�£�ʹ�� setPropertiesFrom����ʵ��
             * ���ԵĿɼ��������Զ���һ��
             * ���Եĸ߶ȺͽǶ������Զ���һ��
             * ���ԵĲ���㣺���Զ���Ĳ���������ղ�����ʸ����
             * ���Եı�����֡���ʾ���������Զ��屣��һ��
             */
            /*
            pAtt->setPropertiesFrom(pAttDef);
            pAtt->setInvisible(pAttDef->isInvisible());
            AcGePoint3d ptBase = pAttDef->position();
            ptBase += bRef.position().asVector();
            pAtt->setPosition(ptBase);
            pAtt->setHeight(pAttDef->height());
            pAtt->setRotation(pAttDef->rotation());
            ACHAR* pPrompt = pAttDef->prompt();
            acutDelString(pPrompt);
            */

            // ������������
            pAtt->setTextString( attValues[pos].kACharPtr() );
            acutPrintf( _T( "\n�������%s��ֵ:%s" ), pTag, attValues[pos].kACharPtr() );

            // ������Ե�AcDbBlockReference��
            bRef.appendAttribute( pAtt );
        }
        acutDelString( pTag );
    }
    delete pIterator;
    actrTransactionManager->endTransaction();
}

ACRX_NO_CONS_DEFINE_MEMBERS( BlockDraw, MineGEDraw )

BlockDraw::BlockDraw () : MineGEDraw ()
{
}

BlockDraw::~BlockDraw ()
{
}

void BlockDraw::setAllExtraParamsToDefault()
{

}

void BlockDraw::configExtraParams()
{
    // �л���Դ
    CAcModuleResourceOverride myResources;

    // ��ʾ���еĿ鶨���б�, ���û�ѡ�������
    // �Ӷ��޸Ŀ��ӻ�Ч��
    BlockDraw_ConfigDlg dlg;
    dlg.m_blockName = m_blockName;
    if( IDOK == dlg.DoModal() )
    {
        m_blockName = dlg.m_blockName;
    }
}

void BlockDraw::updateExtraParams()
{

}

void BlockDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void BlockDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void BlockDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readString( m_blockName );
}

void BlockDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeString( m_blockName );
}

void BlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_attValues.removeAll();
    m_attValues.append( values );
}

Adesk::Boolean BlockDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcDbObjectId objId = GetBlockDefinitionByName( m_blockName );
    if( objId.isNull() ) return Adesk::kTrue;

    //acutPrintf(_T("\nBlockDraw::subWorldDraw()..."));
    //acutPrintf(_T("\n(%.3f, %.3f, %.3f), %.3f"), m_insertPt.x, m_insertPt.y, m_insertPt.z, m_angle);
    AcDbBlockReference bRef( m_insertPt, objId );
    bRef.setRotation( m_angle );

    // �����������
    AcStringArray names;
    regPropertyDataNames( names );
    FillAttribute( names, m_attValues, bRef );

    return bRef.worldDraw( mode );
}

Acad::ErrorStatus BlockDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );

    // ����һ���������
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // �õ�ԭ�е��������

    // ִ�б任
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled();

    AcDbObjectId objId = GetBlockDefinitionByName( m_blockName );
    if( objId.isNull() ) return Acad::eOk;

    AcDbBlockReference bRef( m_insertPt, objId );
    bRef.setRotation( m_angle );
    return bRef.getOsnapPoints( osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds );
}

Acad::ErrorStatus BlockDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // Ŀǰ���Խ����������ֻ��һ���е㣺�����
    /*
    AcDbObjectId objId = GetBlockDefinitionByName(m_blockName);
    if(objId.isNull()) return Acad::eOk;

    AcDbBlockReference bRef(m_insertPt, objId);
    bRef.setRotation(m_angle);
    //fillAttribute(bRef);
    return bRef.getGripPoints(gripPoints, osnapModes, geomIds);
    */
    gripPoints.append( m_insertPt );
    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    /*AcDbObjectId objId = GetBlockDefinitionByName(m_blockName);
    if(objId.isNull()) return Acad::eOk;*/

    // ��ֻ��һ���е㣺���������
    // ��ֻ�ܽ����ƶ�����
    // ����Ĵ���Ŀǰ����Ч��
    /*AcDbBlockReference bRef(m_insertPt, objId);
    bRef.setRotation(m_angle);
    fillAttribute(bRef);
    acutPrintf(_T("\n�޸�ǰ(%.3f, %.3f, %.3f), %.3f"), m_insertPt.x, m_insertPt.y, m_insertPt.z, m_angle);
    Acad::ErrorStatus es = bRef.moveGripPointsAt(indices, offset);
    if(es == Acad::eOk)
    {
    	m_insertPt = bRef.position();
    	m_angle = bRef.rotation();
    	acutPrintf(_T("\n�޸ĺ�(%.3f, %.3f, %.3f), %.3f"), m_insertPt.x, m_insertPt.y, m_insertPt.z, m_angle);
    }
    return es;*/
    m_insertPt += offset;
    return Acad::eOk;
}

void BlockDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    /*AcDbObjectId objId = GetBlockDefinitionByName(m_blockName);
    if(objId.isNull()) return MineGEDraw::caclBackGroundMinPolygon(pts);

    AcDbBlockReference bRef(m_insertPt, objId);
    bRef.setRotation(m_angle);
    AcDbExtents ext;
    bRef.getGeomExtents(ext);
    AcGePoint3d pt = ext.minPoint();
    pts.append(pt);
    AcGePoint3d maxPt = ext.maxPoint();*/
    return MineGEDraw::caclBackGroundMinPolygon( pts );
}
