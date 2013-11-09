#include "StdAfx.h"
#include "PointMonitor.h"

PointMonitor::PointMonitor ( AcApDocument* pDoc ) : AcEdInputPointMonitor(), mpDocument( pDoc )
{
    if ( pDoc )
    {
        acutPrintf( _T( "\nPointMonitor : %ld" ), ( long )pDoc );
        pDoc->inputPointManager ()->addPointMonitor ( this ) ;
    }
}

PointMonitor::~PointMonitor ()
{
    Detach () ;
}

void PointMonitor::Attach ( AcApDocument* pDoc )
{
    Detach () ;
    if ( mpDocument == NULL )
    {
        if ( ( mpDocument = pDoc ) != NULL )
            pDoc->inputPointManager ()->addPointMonitor ( this ) ;
    }
}

void PointMonitor::Detach ()
{
    if ( mpDocument )
    {
        mpDocument->inputPointManager ()->removePointMonitor ( this ) ;
        mpDocument = NULL ;
    }
}

AcApDocument* PointMonitor::Subject () const
{
    return ( mpDocument ) ;
}

bool PointMonitor::IsAttached () const
{
    return ( mpDocument != NULL ) ;
}

bool PointMonitor::excludeFromOsnapCalculation (
    const AcArray<AcDbObjectId>& nestedEntity,
    Adesk::GsMarker gsSelectionMark )
{
    return ( AcEdInputPointMonitor::excludeFromOsnapCalculation ( nestedEntity, gsSelectionMark ) ) ;
}

Acad::ErrorStatus PointMonitor::monitorInputPoint (
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
    AcString toolTipStr;
    toolTipStr.format( _T( "\n²âÊÔPointMonitor" ) );

    // ×·¼Ótooltip
    appendToTooltipStr = true;
    additionalTooltipString = _tcsdup( toolTipStr.kACharPtr() );
    return ( Acad::eOk );
}
