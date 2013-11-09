#include "StdAfx.h"
#include "ArxUtilHelper.h"
#include "ArxDataTool.h"
#include "ArxEntityHelper.h"

static AcDbBlockTableRecord* openCurrentSpaceBlock( AcDb::OpenMode mode, AcDbDatabase* db )
{
    AcDbBlockTableRecord* blkRec;

    Acad::ErrorStatus es = acdbOpenObject( blkRec, db->currentSpaceId(), mode );
    if ( es != Acad::eOk )
        return NULL;
    else
        return blkRec;
}

bool ArxUtilHelper::PostToModelSpace( AcDbEntity* pEnt )
{
    AcDbBlockTableRecord* blkRec = openCurrentSpaceBlock(
                                       AcDb::kForWrite,
                                       acdbHostApplicationServices()->workingDatabase() );

    if ( blkRec == NULL ) return false;

    bool ret = ( Acad::eOk == blkRec->appendAcDbEntity( pEnt ) );
    blkRec->close();

    // ע��ر�˳��(����¼-->ʵ��ָ��)
    // ��յ���������֮�������ر�!!!
    if( ret )
    {
        pEnt->close();
    }
    return true;
}

AcDbObjectId ArxUtilHelper::SelectObject( const CString& msg )
{
    ads_name en;
    ads_point pt;
    CString fmsg;
    fmsg.Format( _T( "\n%s" ), msg ); // ���һ�����з�
    if( RTNORM != acedEntSel( fmsg, en, pt ) ) return AcDbObjectId::kNull;
    acutPrintf( _T( "\n" ) );

    AcDbObjectId eId;
    if ( acdbGetObjectId( eId, en ) != Acad::eOk ) return AcDbObjectId::kNull;

    return eId;
}

void ArxUtilHelper::GetPickSetEntity( AcDbObjectIdArray& objIds )
{
    ads_name ss;
    int ret = acedSSGet( _T( "I" ), NULL, NULL, NULL, ss );
    if( RTNORM != ret ) return;

    long n = 0;
    acedSSLength ( ss, &n );
    for( long i = 0; i < n; i++ )
    {
        ads_name ename;
        if( RTNORM != acedSSName ( ss, i, ename ) ) continue;

        AcDbObjectId objId;
        if( Acad::eOk != acdbGetObjectId( objId, ename ) ) continue;

        objIds.append( objId );
    }
    acedSSFree( ss );
}

void ArxUtilHelper::Pause()
{
    // �ж�
    ACHAR tt[100];
    acedGetString( 0, _T( "\n�밴�س���ո������..." ), tt );
}

bool ArxUtilHelper::IsEqualType( const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent/*=true*/ )
{
    return ArxDataTool::IsEqualType( type, objId, true );
}

void ArxUtilHelper::ShowEntityWithColor( const AcDbObjectId& objId, unsigned short colorIndex )
{
    AcDbObjectIdArray objIds;
    objIds.append( objId );
    ArxUtilHelper::ShowEntitiesWithColor( objIds, colorIndex );
}

void ArxUtilHelper::ShowEntitiesWithColor( AcDbObjectIdArray& objIds, unsigned short colorIndex )
{
    // ��¼ͼԪ��ԭ��ɫ
    AcArray<Adesk::UInt16> originColors;
    if( !ArxEntityHelper::GetEntitiesColor( objIds, originColors ) ) return;

    // ������ʾͼԪ
    ArxEntityHelper::SetEntitiesColor( objIds, colorIndex );

    // ��ͣ
    ArxUtilHelper::Pause();

    // �ָ�ͼԪԭ����ɫ
    ArxEntityHelper::SetEntitiesColor2( objIds, originColors );
}

void ArxUtilHelper::ShowEntitiesWithColor2( AcDbObjectIdArray& objIds, const AcArray<Adesk::UInt16>& colors )
{
    // ��¼ͼԪ��ԭ��ɫ
    AcArray<Adesk::UInt16> originColors;
    if( !ArxEntityHelper::GetEntitiesColor( objIds, originColors ) ) return;

    // ������ʾͼԪ
    ArxEntityHelper::SetEntitiesColor2( objIds, colors );

    // ��ͣ
    ArxUtilHelper::Pause();

    // �ָ�ͼԪԭ����ɫ
    ArxEntityHelper::SetEntitiesColor2( objIds, originColors );
}

double ArxUtilHelper::AdjustAngle( double angle )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    return v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

// Ҳ����ʹ�������2����������ת��
//double d = _tstof(strText);
//int nItem = _ttoi(strText);
bool ArxUtilHelper::StringToDouble( const CString& value, double& v )
{
    if( value.GetLength() == 0 )
    {
        v = 0;
        return true;
    }
    else
    {
        return ( RTNORM == acdbDisToF( value, 2, &v ) );
    }
}

bool ArxUtilHelper::StringToInt( const CString& value, int& v )
{
    double vv;
    if( !StringToDouble( value, vv ) ) return false;

    v = ( int )vv;
    return true;
}

void ArxUtilHelper::DoubleToString( double v, CString& value )
{
    ACHAR fmtval[50];
    if( RTNORM == acdbRToS( v, 2, -1, fmtval ) ) value = fmtval;
}

void ArxUtilHelper::IntToString( int v, CString& value )
{
    value.Format( _T( "%d" ), v );
}


// �ж��ַ����Ƿ���һ���Ϸ���������
// ��ʽ[+/-][0-9]
bool ArxUtilHelper::IsInteger( LPCTSTR pSrc )
{
    if( *pSrc == _T( '+' ) || *pSrc == _T( '-' ) ) pSrc++;
    for( ; *pSrc != _T( '\0' ) && _istdigit( *pSrc ); pSrc++ );
    return ( *pSrc == _T( '\0' ) );
}

// �ж��ַ����Ƿ���һ���Ϸ��ĸ�������
// ��ʽ[+/-][0-9][.][0-9]
bool ArxUtilHelper::IsNumeric( LPCTSTR pSrc )
{
    if( *pSrc == _T( '+' ) || *pSrc == _T( '-' ) ) pSrc++;
    for( bool bp = false; _istdigit( *pSrc ) || ( *pSrc == _T( '.' ) && bp == false ); )
    {
        if( *pSrc++ == _T( '.' ) ) bp = true;
    }
    return ( *pSrc == _T( '\0' ) );
}

// GUID�ַ�����ʽ
#define GUID_FORMAT _T("{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")

bool ArxUtilHelper::NewGUID( CString& strGUID )
{
    GUID m_guid;
    bool ret = ( S_OK == ::CoCreateGuid( &m_guid ) );
    if( ret )
    {
        strGUID.Format( GUID_FORMAT,
                        m_guid.Data1, m_guid.Data2, m_guid.Data3,
                        m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2],
                        m_guid.Data4[3], m_guid.Data4[4], m_guid.Data4[5],
                        m_guid.Data4[6], m_guid.Data4[7] );
    }
    return ret;
}

bool ArxUtilHelper::PromptPt( const CString& msg, AcGePoint3d& pt )
{
    ads_point _pt;
    if( acedGetPoint( NULL, msg, _pt ) != RTNORM ) return false;

    pt = asPnt3d( _pt );

    return true;
}

bool ArxUtilHelper::PromptPt2( const CString& msg, const AcGePoint3d& basePt, AcGePoint3d& pt )
{
    ads_point _pt;
    if( acedGetPoint( asDblArray( basePt ), msg, _pt ) != RTNORM ) return false;

    pt = asPnt3d( _pt );

    return true;
}

double ArxUtilHelper::AngleToXAxis( const AcGeVector3d& v )
{
    return v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

void ArxUtilHelper::BrowserEntities( const AcDbObjectIdArray& objIds )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        ArxEntityHelper::ZoomToEntity( objIds[i] );
        ArxUtilHelper::Pause(); // ��ͣ
    }
}