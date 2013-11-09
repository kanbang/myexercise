#include "StdAfx.h"
#include "PolyLineJig.h"

static AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

static AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

PolyLineJig::PolyLineJig () : AcEdJig (), pPolyLine( 0 )
{
}

PolyLineJig::~PolyLineJig ()
{
}

void PolyLineJig::addPoint( const AcGePoint3d& pt )
{
    pPolyLine->addVertexAt( num, Point3D_To_2D( pt ) );
}

void PolyLineJig::setPoint( const AcGePoint3d& pt )
{
    pPolyLine->setPointAt( num, Point3D_To_2D( pt ) );
}

void PolyLineJig::getLastPoint( AcGePoint3d& pt )
{
    //if(pPolyLine->numVerts() == 1)
    //{
    //	pPolyLine->getStartPoint(pt);
    //}
    //else
    //{
    pPolyLine->getEndPoint( pt );
    //}
}

void PolyLineJig::getAllPoints( AcGePoint3dArray& pts )
{
    int num = pPolyLine->numVerts();
    AcGePoint2dArray V;
    for( int i = 0; i < num; i++ )
    {
        AcGePoint2d pt;
        pPolyLine->getPointAt( i, pt );
        pts.append( Point2D_To_3D( pt ) );
    }
}

Adesk::Boolean PolyLineJig::doJig( AcGePoint3dArray& pts )
{
    pPolyLine = new AcDbPolyline();

    setUserInputControls( ( UserInputControls )( kDontUpdateLastPoint | kNullResponseAccepted ) );

    // ��ȡ��1����
    setDispPrompt( _T( "\n����ӱ߽��: " ) );
    AcGePoint3d pt;
    DragStatus stat = acquirePoint( pt );
    if ( stat != kNormal )
    {
        delete pPolyLine;
        pPolyLine = 0;
        return Adesk::kFalse;
    }

    num = 0;
    addPoint( pt );
    cpt = pt;

    // ���ո����(����kNull)
    while( 1 )
    {
        setDispPrompt( _T( "\n������ӱ߽��: " ) );
        //acutPrintf(_T("\n����drag()ǰ...."));

        addPoint( cpt );
        num++;

        stat = drag();
        //acutPrintf(_T("\n����drag()��****"));
        //acutPrintf(_T("status:%d"), stat);
        if( stat != kNormal ) break;
    }
    pPolyLine->removeVertexAt( num ); // ɾ�����һ���ڵ�(ע��!!!)
    //acutPrintf(_T("status:%d"), stat);
    bool ret = false;
    if( stat == kNull )
    {
        ret = true;
        getAllPoints( pts );
    }
    delete pPolyLine;
    pPolyLine = 0;

    return ret;
}

AcEdJig::DragStatus PolyLineJig::sampler()
{
    //acutPrintf(_T("\n����sampler()"));
    return getNextPoint();
}

Adesk::Boolean PolyLineJig::update()
{
    //num == pPolyLine->numVerts();
    //acutPrintf(_T("\n����update()\t����ڵ����:%d"), num+1);
    setPoint( cpt );
    return Adesk::kTrue;
}

AcDbEntity* PolyLineJig::entity () const
{
    //acutPrintf(_T("\n����entity()"));
    return pPolyLine;
}

AcEdJig::DragStatus PolyLineJig::getNextPoint()
{
    setUserInputControls( ( UserInputControls )( kDontUpdateLastPoint | kNullResponseAccepted ) );

    AcGePoint3d lastPt;
    getLastPoint( lastPt );

    AcGePoint3d pt;
    AcEdJig::DragStatus stat = acquirePoint( pt, lastPt );
    //acutPrintf(_T("\nstatus:%d"), stat);

    if( stat != kNormal ) return stat;

    if( pt == lastPt )
    {
        stat = kNoChange;
    }
    else
    {
        cpt = pt;
    }
    return stat;
}

