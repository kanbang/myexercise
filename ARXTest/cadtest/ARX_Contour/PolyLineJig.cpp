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

    // 获取第1个点
    setDispPrompt( _T( "\n请添加边界点: " ) );
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

    // 按空格结束(返回kNull)
    while( 1 )
    {
        setDispPrompt( _T( "\n继续添加边界点: " ) );
        //acutPrintf(_T("\n调用drag()前...."));

        addPoint( cpt );
        num++;

        stat = drag();
        //acutPrintf(_T("\n调用drag()后****"));
        //acutPrintf(_T("status:%d"), stat);
        if( stat != kNormal ) break;
    }
    pPolyLine->removeVertexAt( num ); // 删除最后一个节点(注意!!!)
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
    //acutPrintf(_T("\n调用sampler()"));
    return getNextPoint();
}

Adesk::Boolean PolyLineJig::update()
{
    //num == pPolyLine->numVerts();
    //acutPrintf(_T("\n调用update()\t当点节点个数:%d"), num+1);
    setPoint( cpt );
    return Adesk::kTrue;
}

AcDbEntity* PolyLineJig::entity () const
{
    //acutPrintf(_T("\n调用entity()"));
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

