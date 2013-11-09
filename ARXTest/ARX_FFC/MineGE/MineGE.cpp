#include "StdAfx.h"
#include "MineGE.h"

#include "config.h"
#include "DataHelperImpl.h"
#include "CurDrawTool.h"

#include "../MineGEDraw/MineGEDraw_Jig.h"
#include "../MineGEDraw/MineGEDrawSystem.h"
#include "../ArxDbgXdata/ArxDbgXdata.h"

static bool HasDataObject( const AcDbObjectId& dictId )
{
    AcDbDictionary* pDict;
    if( Acad::eOk != acdbOpenObject( pDict, dictId, AcDb::kForWrite ) ) return false;

    AcDbObjectId objId;
    bool ret = pDict->has( DATA_OBJECT_EXT_DICT_KEY );
    pDict->close();

    return ret;
}

static AcDbObjectId CreateDataObject( const AcDbObjectId& dictId,
                                      const CString& type,
                                      const AcDbObjectId& objId )
{
    AcDbDictionary* pDict;
    if( Acad::eOk != acdbOpenObject( pDict, dictId, AcDb::kForWrite ) ) return AcDbObjectId::kNull;

    // �������ݶ���
    DataObject* pDO = new DataObject();
    // ����ͼԪid
    pDO->setGE( objId );
    // ��¼��������
    pDO->setType( type );
    // ��ʼ������
    pDO->initData();

    AcDbObjectId dbObjId;
    if( Acad::eOk != pDict->setAt( DATA_OBJECT_EXT_DICT_KEY, pDO, dbObjId ) )
    {
        delete pDO;
    }
    else
    {
        pDO->close();
    }
    pDict->close();

    return dbObjId;
}

static AcDbObjectId GetDataObject( const AcDbObjectId& dictId )
{
    if( dictId.isNull() ) return AcDbObjectId::kNull;

    AcDbDictionary* pDict;
    if( Acad::eOk != acdbOpenObject( pDict, dictId, AcDb::kForRead ) ) return AcDbObjectId::kNull;

    AcDbObjectId objId;
    pDict->getAt( DATA_OBJECT_EXT_DICT_KEY, objId );

    pDict->close();

    return objId;
}

// ��ͼԪ���������б�ǩͼԪ���б任(�ɼ�ͼԪ����������ɾ������ͼԪ)
//static void TransformAllTagGE( const AcDbObjectId& objId, const AcGeMatrix3d & xform )
//{
//	AcDbObjectIdArray objIds;
//	DrawHelper::GetAllTagGEById(objId, objIds);
//	ArxEntityHelper::TransformEntities(objIds, xform);
//}

Adesk::UInt32 MineGE::kCurrentVersionNumber = 1 ;

// ���޸ģ�ʹ��MineGE��Ϊ������
ACRX_NO_CONS_DEFINE_MEMBERS ( MineGE, AcDbEntity )

MineGE::MineGE() : m_pCurrentGEDraw( 0 )
{
    //acutPrintf(_T("\nMineGE::MineGE()..."));
}

MineGE::~MineGE ()
{
    //acutPrintf(_T("\nMineGE::~MineGE()..."));
    m_pCurrentGEDraw = 0;
}

CString MineGE::getTypeName() const
{
    // ������������
    // ʹ�����麯������isA()
    return this->isA()->name();
}

void MineGE::initPropertyData()
{
    //assertWriteEnabled();
    if( !m_dataObjectId.isNull() ) return;

    Acad::ErrorStatus es = upgradeOpen();
    if( es == Acad::eOk || es == Acad::eWasOpenForWrite )
    {
        createExtensionDictionary();
        AcDbObjectId dictId = extensionDictionary();
        m_dataObjectId = CreateDataObject( dictId, getTypeName(), objectId() );
    }
    if( es == Acad::eOk )
    {
        downgradeOpen();
    }
}

AcDbObjectId MineGE::getDataObject() const
{
    assertReadEnabled();

    /*return GetDataObject(extensionDictionary());*/
    return m_dataObjectId;
}

void MineGE::initDraw()
{
    if( m_pCurrentGEDraw == 0 )
    {
        Acad::ErrorStatus es = upgradeOpen();
        if( es == Acad::eOk || es == Acad::eWasOpenForWrite )
        {
            updateCurrentDraw();
        }
        if( es == Acad::eOk )
        {
            downgradeOpen();
        }
    }
}

void MineGE::updateDrawParams( MineGEDraw* pGEDraw )
{
    if( pGEDraw != 0 )
    {
        writeKeyParamToGEDraw( pGEDraw );
        pGEDraw->updateExtraParams();
        readExtraParamFromGEDraw( pGEDraw );
    }
}

void MineGE::configDraw( const CString& drawName )
{
    MineGEDraw* pGEDraw = MineGEDrawSystem::GetInstance()->getGEDraw( getTypeName(), drawName );
    updateDrawParams( pGEDraw );
}

void MineGE::extractExistedDraw( AcStringArray& existedDraw )
{
    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.getXdata( this );          // ��ȡ��չ���ݵ�xdata������

    if( xdata.isEmpty() ) return;    // û������

    int len = 0;
    xdata.getInteger( 1, len );      // ����Ч������
    for( int i = 1; i <= len; i++ )
    {
        CString drawName;
        xdata.getString( 2 * i, drawName ); // ����Ч������
        existedDraw.append( drawName );
    }
}

// ���������Ƚϸ���
// ����Ҫ��֤xdata���draw��ȫ�ֵ�draw��ͬ����
// ���ܴ������¼��������
// 1��xdata���draw ���� ȫ�ֵ�draw
// 2��xdata���draw ���� ȫ�ֵ�draw(������draw���)
// 3��xdata���draw С�� ȫ�ֵ�draw(ɾ����draw���)
// 4��xata��Ĳ���draw����ȫ�ֵ�draw��û�У�ȫ�ֵĲ���draw��xdata��û��
// Ŀǰ������򵥵İ취������draw�������ȫ�����ϲ���xdata��
// Ҳ����˵xdata�п��ܴ�������ġ�����ʹ�õ�draw����
// ���ڿ��Կ���ר�ű�дһ������draw�ĳ���
void MineGE::initAllExtraParamsToXData()
{
    // ͨ��assertWriteEnabled�޶�ֻ����write״̬�²���
    assertWriteEnabled();

    // ��ȡ�Ѵ��ڵ�draw
    AcStringArray existedDraw;
    extractExistedDraw( existedDraw );

    MineGEDrawSystem* pGEService = MineGEDrawSystem::GetInstance();
    AcStringArray drawList;
    pGEService->getAllGEDrawsByGEType( getTypeName(), drawList );

    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.setString( 0, _T( "" ) );                           // ��ǰ����Ч������
    xdata.setInteger( 1, 0 );                                 // ����Ч������(��ʱ����0)

    bool foundNewDraw = false;                                // �Ƿ����µ�draw
    int i = existedDraw.length() + 1;
    int len = drawList.length();
    for( int j = 0; j < len; j++ )
    {
        if( existedDraw.contains( drawList.at( j ) ) ) continue;

        foundNewDraw = true;                                   // �������µ�draw

        MineGEDraw* pGEDraw = pGEService->getGEDraw( getTypeName(), drawList.at( j ).kACharPtr() );
        writeKeyParamToGEDraw( pGEDraw );                          // д����Ҫ������draw��
        // ��Щextra param����Ҫ����ģ�
        // �ҿ�����key param�й�
        //pGEDraw->setAllExtraParamsToDefault();                   // ������ΪĬ��ֵ
        pGEDraw->updateExtraParams();                              // ���㲢���²���
        xdata.setString( 2 * i, pGEDraw->isA()->name() );          // ����Ч������

        ArxDbgXdataList dataList;
        DrawParamWriter writer( &dataList );
        pGEDraw->writeExtraParam( writer );
        xdata.setList( 2 * i + 1, dataList );                      // д�븽�Ӳ���
        i++;
    }
    xdata.setInteger( 1, i - 1 );                                  // ����draw��ʵ�ʸ���

    if( foundNewDraw ) xdata.setXdata( this );                     // ֻ�з����µ�draw�Ż����xdata
}

static MineGEDraw* GetCurrentDrawPointer( const CString& type )
{
    MineGEDraw* pDraw = 0;

    CString draw;
    if( GetCurDraw( type, draw ) )
    {
        MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();
        if( pDrawSystem != 0 )
        {
            pDraw = pDrawSystem->getGEDraw( type, draw );
        }
    }
    return pDraw;
}

void MineGE::updateDraw()
{
    assertWriteEnabled();

    //m_pCurrentGEDraw = GetCurrentDrawPointer(getTypeName());
    if( m_pCurrentGEDraw != 0 )
    {
        updateDrawParams( m_pCurrentGEDraw );
    }
}

void MineGE::updateCurrentDraw()
{
    assertWriteEnabled();

    m_pCurrentGEDraw = GetCurrentDrawPointer( getTypeName() );
    if( !isNewObject() )
    {
        //acutPrintf(_T("\nnot new object"));
        // clone����(���綯̬�༭Ч����ʾ�����Ƶ�)
        // �õ���ͼԪû���ύ�����ݿ�
        // ��ʱ���ӻ�������δ��ʼ��������Ҫ���¿��ӻ�����
        //updateDrawParams(m_pCurrentGEDraw);
    }
    else
    {
        //acutPrintf(_T("\nnew object"));
        initAllExtraParamsToXData();
    }
}

MineGEDraw* MineGE::getCurrentDraw() const
{
    return m_pCurrentGEDraw;
}

void MineGE::writeExtraParamToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.getXdata( ( MineGE* )this ); // ��ȡ��չ���ݵ�xdata������(ǿ��ȥ��const����)

    int len = 0;
    xdata.getInteger( 1, len );    // ����Ч������
    for( int i = 1; i <= len; i++ )
    {
        CString drawName;
        xdata.getString( 2 * i, drawName ); // ����Ч������

        if( drawName.CompareNoCase( pGEDraw->isA()->name() ) == 0 )
        {
            ArxDbgXdataList dataList;
            xdata.getList( 2 * i + 1, dataList ); // ����Ч���Ĳ���

            ArxDbgXdataListIterator paramList( dataList );
            DrawParamReader reader( &paramList );
            pGEDraw->readExtraParam( reader ); // ����չ�����ж�ȡ����
        }
    }
}

// ������write״̬�²���
void MineGE::readExtraParamFromGEDraw( MineGEDraw* pGEDraw )
{
    assertWriteEnabled();

    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.getXdata( this ); // ��ȡ��չ���ݵ�xdata������

    // �����滻����չ����
    // ˼·����ԭ�е���չ���ݸ���һ�ݣ������޸ĵ�draw�������Ƚ����޸ģ�Ȼ���ٱ��浽�µ���չ������
    ArxDbgAppXdata xdata2( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata2.setString( 0, _T( "" ) ); // ��ǰ����Ч������

    int len = 0;
    xdata.getInteger( 1, len );     // ����Ч������
    xdata2.setInteger( 1, len );

    for( int i = 1; i <= len; i++ )
    {
        CString drawName;
        xdata.getString( 2 * i, drawName ); // ����Ч������
        xdata2.setString( 2 * i, drawName );

        if( drawName.CompareNoCase( pGEDraw->isA()->name() ) == 0 )
        {
            ArxDbgXdataList dataList;
            DrawParamWriter writer( &dataList );
            pGEDraw->writeExtraParam( writer );

            xdata2.setList( 2 * i + 1, dataList ); // �޸Ĳ��������Ч���Ĳ���
        }
        else
        {
            ArxDbgXdataList dataList;
            xdata.getList( 2 * i + 1, dataList );
            xdata2.setList( 2 * i + 1, dataList );
        }
    }
    xdata2.setXdata( this );
}

void MineGE::readKeyParamFromGEDraw( MineGEDraw* pGEDraw )
{
    assertWriteEnabled();

    // ��MineGEDraw�ж�ȡ����֮��Ĺؼ�����
    ArxDbgXdataList dataList;
    DrawParamWriter writer( &dataList );
    pGEDraw->writeKeyParam( writer );

    ArxDbgXdataListIterator dataListIter( dataList );
    DrawParamReader reader( &dataListIter );
    pullKeyParamFromReader( reader );
}

void MineGE::writeKeyParamToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    // ���ؼ��������µ�MineGEDraw��
    ArxDbgXdataList dataList;
    DrawParamWriter writer( &dataList );
    pushKeyParamToWriter( writer );

    ArxDbgXdataListIterator dataListIter( dataList );
    DrawParamReader reader( &dataListIter );
    pGEDraw->readKeyParam( reader );
}

static bool GetPropertyDataFromDataObject( const AcDbObjectId& objId, const AcStringArray& names, AcStringArray& values )
{
    DataObject* pDO;
    if( Acad::eOk != acdbOpenObject( pDO, objId, AcDb::kForRead ) ) return false;

    DataHelperImpl dh( pDO );
    int len = names.length();
    for( int i = 0; i < len; i++ )
    {
        CString value;
        bool ret = dh.getPropertyData( names[i].kACharPtr(), value );
        values.append( value );
    }
    pDO->close();

    return true;
}

void MineGE::writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    // ��ȡҪ��ѯ���ֶ����Ƽ���
    AcStringArray names;
    pGEDraw->regPropertyDataNames( names );
    if( names.isEmpty() ) return;

    // ��ѯ���ݣ���д�뵽values��
    AcStringArray values;
    if( !GetPropertyDataFromDataObject( getDataObject(), names, values ) )
    {
        int len = names.length();
        for( int i = 0; i < len; i++ )
        {
            // �����ȡ����ʧ�ܣ������names�ȳ��Ŀ��ַ���
            values.append( _T( "" ) );
        }
    }

    // ����ѯ�������ݷ��ص�pGEDraw
    pGEDraw->readPropertyDataFromGE( values );
}

void MineGE::writeParamToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    // 1�����ؼ��������µ�MineGEDraw��
    writeKeyParamToGEDraw( pGEDraw );

    // 2������չ��������ȡ����
    writeExtraParamToGEDraw( pGEDraw );

    // 3����ȡ�������ݣ������ݸ�MineGEDraw
    writePropertyDataToGEDraw( pGEDraw );
}

void MineGE::readParamFromGEDraw( MineGEDraw* pGEDraw )
{
    assertWriteEnabled();

    // 1����MineGEDraw�ж�ȡ���º�Ĺؼ�����
    readKeyParamFromGEDraw( pGEDraw );

    // 2����draw�Ĳ������浽��չ������
    readExtraParamFromGEDraw( pGEDraw );
}

void MineGE::updateDrawParam( bool readOrWrite ) const
{
    if( readOrWrite )
    {
        ( ( MineGE* )this )->readParamFromGEDraw( m_pCurrentGEDraw ); // ǿ��ȥ��const����
    }
    else
    {
        writeParamToGEDraw( m_pCurrentGEDraw );
    }
}

Acad::ErrorStatus MineGE::dwgOutFields( AcDbDwgFiler* pFiler ) const
{
    //acutPrintf(_T("\nid:%d call MineGE::dwgOutFields()..."), objectId());

    assertReadEnabled () ;
    Acad::ErrorStatus es = AcDbEntity::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    if ( ( es = pFiler->writeUInt32 ( MineGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeSoftPointerId( m_dataObjectId );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus MineGE::dwgInFields( AcDbDwgFiler* pFiler )
{
    //acutPrintf(_T("\nid:%d call MineGE::dwgInFields()..."), objectId());

    assertWriteEnabled () ;
    Acad::ErrorStatus es = AcDbEntity::dwgInFields ( pFiler );
    if ( es != Acad::eOk )
        return ( es ) ;

    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > MineGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    /* 199֮ǰ(����199)�İ汾ʹ�� */
    //AcString str;
    //pFiler->readString(str);
    //pFiler->readString(str);

    /* 199֮��(������199)�İ汾ʹ�� */
    AcDbSoftPointerId id;
    pFiler->readSoftPointerId( &id );
    m_dataObjectId = id;

    initDraw();

    return ( pFiler->filerStatus () ) ;
}

// DWORD <==> Adesk::UInt32 <==> unsigned long
// Adesk::UInt8 <==> unsigned char
static void LONG2RGB( Adesk::UInt32 rgbColor, Adesk::UInt8& r, Adesk::UInt8& g, Adesk::UInt8& b )
{
    // ����2�δ����ǵȼ۵�

    /*r = ( rgbColor & 0xffL );
    g = ( rgbColor & 0xff00L ) >> 8;
    b = rgbColor >> 16;*/

    r = GetRValue( rgbColor );
    g = GetGValue( rgbColor );
    b = GetBValue( rgbColor );
}

// ��ȡcad��ͼ���ڱ���ɫ
static bool GetBackgroundColor( Adesk::UInt8& r, Adesk::UInt8& g, Adesk::UInt8& b )
{
    // ��ȡcad��ǰ��������ɫ����
    // �Ҽ�"ѡ��"->"��ʾ"->"��ɫ"
    AcColorSettings cs;
    if( !acedGetCurrentColors( &cs ) ) return false;

    // ��ȡ������ɫ
    DWORD rgbColor = cs.dwGfxModelBkColor;
    LONG2RGB( rgbColor, r, g, b );
    return true;
}

// ��������
/*
Adesk::UInt32 rgbColor = AcCmEntityColor::lookUpRGB(cl.colorIndex()); // ת����rgb��ɫ

AcCmEntityColor bgColor(255-r, 255-g, 255-b); // RGB��ɫ

AcCmEntityColor bgColor;
bgColor.setRGB(0, 0, 0);
if(layerColor.colorIndex() == 7) bgColor.setRGB(r, g, b); // RGB��ɫ
else bgColor.setRGB(255-r, 255-g, 255-b);
 */

/*
 * �ڻ��Ʊպ�ͼ��ʱ��AcGiFillTypeĬ��ΪkAcGiFillAlways (ʼ�����)
 * �պ�ͼ�ΰ�����Բ������Ρ������
 * �μ���AcGiSubEntityTraits::fillType()����˵��
 * ���磬����һ��Բ����ǰ��ɫ�Ǻڵװ�ɫ����ô�����Զ���ʵ����Ƶ�Բ��2�����:
 *	    1) arx������ص������-- �ױ�+�ڵ����(����Ч������cad��Բ��һ����)
 *		2) arx����ж�أ�cad���ô���ʵ��ģʽ��ʾͼԪ -- �ױ�+�׵����
 * ����μ����������Բ��һЩ˵��.doc
 */

// ����AcCmColor::colorIndex()��������Ϊcolor index�ܹ�ֻ��256�֣��Ұ�/�ڶ�ʹ��7��ʾ���޷�����
// ���Ҫʹ��rgb��ɫ��Ӧʹ��AcCmEntityColor��AcCmColor����
void MineGE::drawBackground( MineGEDraw* pGEDraw, AcGiWorldDraw* mode )
{
    if( isNewObject() ) return;

    AcGePoint3dArray pts;
    m_pCurrentGEDraw->caclBackGroundMinPolygon( pts );

    // �û�û�ж���߽�
    if( pts.isEmpty() ) return;

    // ��ȡcad��ͼ���ڱ���ɫ
    Adesk::UInt8 r, g, b;
    if( !GetBackgroundColor( r, g, b ) ) return;

    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    // ����ԭ�е�����
    Adesk::UInt16 cl = traits.color();;
    AcGiFillType ft = traits.fillType();

    AcCmEntityColor bgColor( r, g, b );
    traits.setTrueColor( bgColor );
    traits.setFillType( kAcGiFillAlways ); // ���
    //acutPrintf(_T("\n��ɫ������%d"), bgColor.colorIndex());
    mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

    // �ָ�����
    traits.setFillType( ft );
    traits.setColor( cl );
}

Adesk::Boolean MineGE::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Adesk::kTrue;

    //acutPrintf(_T("\ncall id:%d MineGE::subWorldDraw()..."), objectId());
    //acutPrintf(_T("\ncall drawname:%s..."), m_pCurrentGEDraw->isA()->name());

    // 1�����²�����MineGEDraw��
    updateDrawParam( false );

    // 2�����Ʊ�����
    // �÷����ڶ�̬Ч���п��ܻ���һЩ����,�ر���jig
    // �²�ԭ��:�������Ķ���ο��ܱȽ���
    drawBackground( m_pCurrentGEDraw, mode );

    // 3��draw���������ʵ��ͼ��
    m_pCurrentGEDraw->worldDraw( mode );

    return Adesk::kTrue;
}

Acad::ErrorStatus MineGE::subTransformBy( const AcGeMatrix3d& xform )
{
    if( m_pCurrentGEDraw == 0 )
    {
        //acutPrintf(_T("\nsubTransformBy��draw=null"));
        return Acad::eOk;
    }

    //acutPrintf(_T("\ncall id:%d MineGE::subTransformBy()..."), objectId());

    // 1�����²�����MineGEDraw��
    updateDrawParam( false );

    // 2��ִ�б任
    m_pCurrentGEDraw->transformBy( xform );

    // 3����MineGEDraw�ж�ȡ���º�Ĳ���
    updateDrawParam( true );

    // 4�����������е�TagGE
    //TransformAllTagGE(objectId(), xform);

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Acad::eOk;

    // 1�����²�����MineGEDraw��
    updateDrawParam( false );

    // 2��draw���������ʵ��ͼ��
    m_pCurrentGEDraw->getOsnapPoints( osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds );

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subGetGripPoints(
    AcGePoint3dArray& gripPoints,
    AcDbIntArray& osnapModes,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Acad::eOk;

    // 1�����²�����MineGEDraw��
    updateDrawParam( false );

    // 2��draw���������ʵ��ͼ��
    m_pCurrentGEDraw->getGripPoints( gripPoints, osnapModes, geomIds );

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Acad::eOk;

    //acutPrintf(_T("\ncall id:%d MineGE::subMoveGripPointsAt()..."), objectId());

    // 1�����²�����MineGEDraw��
    updateDrawParam( false );

    // 2��ִ�б任
    m_pCurrentGEDraw->moveGripPointsAt( indices, offset );

    // 3����MineGEDraw�ж�ȡ���º�Ĳ���
    updateDrawParam( true );

    // ���еı�ǩͼԪҲ���б任
    // �任���ƫ��ϴ󣬲������ڳ�����ʹ��
    //TransformAllTagGE(objectId(), AcGeMatrix3d::translation(offset));

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subGetGeomExtents( AcDbExtents& extents ) const
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return AcDbEntity::subGetGeomExtents( extents );

    // 1�����²�����MineGEDraw��
    updateDrawParam( false );

    Acad::ErrorStatus es = m_pCurrentGEDraw->getGeomExtents( extents );
    // Drawû������ʵ��subGetGeomExtents
    if( Acad::eOk != es )
    {
        //acutPrintf(_T("\nʹ�ñ�����������μ�����������...\n"));
        // ʹ��caclBackGroundMinPolygon()��������Ķ���δ���
        AcGePoint3dArray pts;
        m_pCurrentGEDraw->caclBackGroundMinPolygon( pts );
        if( pts.isEmpty() )
        {
            es = Acad::eInvalidExtents;
        }
        else
        {
            int len = pts.length();
            for( int i = 0; i < len; i++ )
            {
                extents.addPoint( pts[i] );
            }
            es = Acad::eOk;
        }
    }

    return es;
}

bool MineGE::drawByJig()
{
    if( m_pCurrentGEDraw == 0 )
    {
        updateCurrentDraw();
        if( m_pCurrentGEDraw == 0 )
        {
            //acutPrintf(_T("\njig ����drawʧ��..."));
            return false;
        }
    }

    MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();
    if( pDrawSystem == 0 ) return false;

    MineGEDraw_Jig* pJig = pDrawSystem->getGEDraw_Jig( getTypeName(), m_pCurrentGEDraw->isA()->name() );
    if( pJig == 0 ) return false;

    initAllExtraParamsToXData(); // д��extra param��xdata��

    MineGEDraw* pDraw = MineGEDraw::cast( m_pCurrentGEDraw->clone() );
    writeExtraParamToGEDraw( pDraw ); // ��ǰdraw�е�һЩ������extra paramд�뵽pDraw
    // ����currentDraw���������£�����Ҫ���µ�����д�뵽pDraw��

    Adesk::Boolean ret = pJig->startJig( pDraw );
    if( ret )
    {
        readParamFromGEDraw( pDraw );
    }
    delete pDraw;

    return ret;
}

// cad��Ƶ���ĵ���subClose
Acad::ErrorStatus MineGE::subClose( void )
{
    //acutPrintf(_T("\nid:%d call MineGE::subClose()...\n"), objectId());

    Acad::ErrorStatus es = AcDbEntity::subClose () ;

    // new���󲢳ɹ��ύ�����ݿ�֮��
    // ��ʼ�����ӻ�Ч������(��չ����)
    // �������ݶ���(��չ�ʵ�)
    if( es == Acad::eOk )
    {
        initDraw();
        initPropertyData();
    }

    return es;
}