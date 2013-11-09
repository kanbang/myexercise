#include "StdAfx.h"
#include "MineGETooltipMonitor.h"

#include "FieldHelper.h"
#include "DataHelper.h"

#include "FieldInfoHelper.h"
//#include "TypeNameMapHelper.h"

#include "../ArxHelper/HelperClass.h"

// ��ȡҪ��tooltip��ʾ���ֶ�
static void GetFieldsShowInToolTip( const CString& type, AcStringArray& fields )
{
    AcStringArray allFields;
    FieldHelper::GetAllFields( type, allFields );
    if( allFields.isEmpty() ) return;

    int len = allFields.length();
    for( int i = 0; i < len; i++ )
    {
        FieldInfo info;
        FieldInfoHelper::ReadFieldInfo( type, allFields[i].kACharPtr(), info );
        if( info.m_showInTooltip )
        {
            fields.append( allFields[i] );
        }
    }
}

// ��ʽ��tooltip
static void FormatToolTip( const CString& type, const AcDbObjectId& objId, const AcStringArray& fields, AcString& toolTipStr )
{
    CString type2( type );
    //TypeNameMapHelper::GetTypeNameMap(type, type2); // �û��ڴʵ���ָ��������
    ArxClassHelper::GetDxfName( type, type2 ); // ��ȡdxf����(���ඨ���ʱ��ͨ��ARX��ָ��)

    // ͼԪ��������
    AcString str;
    str.format( _T( "\nͼԪ����: %s\n" ), type2 );

    toolTipStr.append( str );
    str.format( _T( "��������\t�ֶθ���:%d\n" ), fields.length() );
    toolTipStr.append( str );

    if( fields.isEmpty() )
    {
        toolTipStr.format( _T( "\nû�п�����ʾ������!" ) );
        //toolTipStr.append(_T("\n  û�п�����ʾ������\n  ����\"�ֶι���Ի���\"�������Ƿ���ʾ"));
        return;
    }

    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        CString name = fields[i].kACharPtr();
        CString value;
        DataHelper::GetPropertyData( objId, name, value );

        str.format( _T( "%s: \t%s\n" ), name, value );
        toolTipStr.append( str );
    }
}

MineGETooltipMonitor::MineGETooltipMonitor ( AcApDocument* pDoc ) : AcEdInputPointMonitor(), mpDocument( pDoc )
{
    if ( pDoc )
    {
        //acutPrintf(_T("\nMineGETooltipMonitor : %ld"), (long)pDoc);
        pDoc->inputPointManager ()->addPointMonitor ( this ) ;
    }
}

MineGETooltipMonitor::~MineGETooltipMonitor ()
{
    Detach () ;
}

void MineGETooltipMonitor::Attach ( AcApDocument* pDoc )
{
    Detach () ;
    if ( mpDocument == NULL )
    {
        if ( ( mpDocument = pDoc ) != NULL )
            pDoc->inputPointManager ()->addPointMonitor ( this ) ;
    }
}

void MineGETooltipMonitor::Detach ()
{
    if ( mpDocument )
    {
        mpDocument->inputPointManager ()->removePointMonitor ( this ) ;
        mpDocument = NULL ;
    }
}

AcApDocument* MineGETooltipMonitor::Subject () const
{
    return ( mpDocument ) ;
}

bool MineGETooltipMonitor::IsAttached () const
{
    return ( mpDocument != NULL ) ;
}

bool MineGETooltipMonitor::excludeFromOsnapCalculation (
    const AcArray<AcDbObjectId>& nestedEntity,
    Adesk::GsMarker gsSelectionMark )
{
    return ( AcEdInputPointMonitor::excludeFromOsnapCalculation ( nestedEntity, gsSelectionMark ) ) ;
}

Acad::ErrorStatus MineGETooltipMonitor::monitorInputPoint (
    bool& appendToTooltipStr,
    ACHAR*& additionalTooltipString,
    AcGiViewportDraw* drawContext,
    AcApDocument* document,
    bool pointComputed,
    int history,
    const AcGePoint3d& lastPoint,
    const AcGePoint3d& rawPoint,
    const AcGePoint3d& grippedPoint,
    const AcGePoint3d& cartesianSnappedPoint,
    const AcGePoint3d& osnappedPoint,
    AcDb::OsnapMask osnapMask,
    const AcArray<AcDbCustomOsnapMode*>& customOsnapModes,
    AcDb::OsnapMask osnapOverrides,
    const AcArray<AcDbCustomOsnapMode*>& customOsnapOverrides,
    const AcArray<AcDbObjectId>& apertureEntities,
    const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> >& nestedApertureEntities,
    const AcArray<Adesk::GsMarker>& gsSelectionMark,
    const AcArray<AcDbObjectId>& keyPointEntities,
    const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> >& nestedKeyPointEntities,
    const AcArray<Adesk::GsMarker>& keyPointGsSelectionMark,
    const AcArray<AcGeCurve3d*>& alignmentPaths,
    const AcGePoint3d& computedPoint,
    const ACHAR* tooltipString
)
{
    // !!!��Ҫ���ĵ�������������!!!
    // ����������ʵ���صĲ������᷵�ش���Ľ��
    if( !apertureEntities.isEmpty() )
    {
        // �����ĵ�(document������ʾ�ĵ�����)
        acDocManager->lockDocument( document );

        // ��ȡ���ݶ���
        AcDbObjectId dObjId;
        if( !DataHelper::GetDataObject( apertureEntities[0], dObjId ) ) return Acad::eOk;

        // ��ȡ��������
        CString type;
        if( !DataHelper::GetTypeName( apertureEntities[0], type ) ) return Acad::eOk;

        // �����ֶ�
        AcStringArray fields;
        GetFieldsShowInToolTip( type, fields );

        // ��ʽ��tooltip
        AcString toolTipStr;
        FormatToolTip( type, dObjId, fields, toolTipStr );

        // ������ǰ�ĵ�
        acDocManager->unlockDocument( document );

        // ׷��tooltip
        appendToTooltipStr = true;
        additionalTooltipString = _tcsdup( toolTipStr.kACharPtr() );
    }
    return ( Acad::eOk );
}
