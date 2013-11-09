#include "StdAfx.h"
#include "PolyLineJig.h"

#include "../ArxHelper/HelperClass.h"

static AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

static AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

/*
 * Ŀǰ��jigʵ����˸ʮ�����أ�
 * �������Բμ�<<ObjectARX����ʵ���̳�-20090826>>�е�"5.2.2 ��̬���������"�Ĵ���
 */

/*
 * ��̬ѡ��һ�����������
 * �ö���ο����Ǻ����ģ�Ҳ���ܲ��Ǻ�����
 * ֻ��Ҫ�ܹ���ʾһ�����򼴿�
 */
class PolyLineJig : public AcEdJig
{
public:
    PolyLineJig () ;
    ~PolyLineJig () ;

    Adesk::Boolean doJig( AcGePoint3dArray& pts );

protected:
    virtual DragStatus sampler () ;
    virtual Adesk::Boolean update () ;
    virtual AcDbEntity* entity () const ;

private:
    void getAllPoints( AcGePoint3dArray& pts );
    void addPoint( const AcGePoint3d& pt );
    void setPoint( const AcGePoint3d& pt );
    void getLastPoint( AcGePoint3d& pt );
    AcEdJig::DragStatus getNextPoint();
    AcDbPolyline* pPolyLine;
    AcGePoint3d cpt;
    int num;
} ;

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
    setDispPrompt( _T( "\n�����ӱ߽��: " ) );
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
        setDispPrompt( _T( "\n�������ӱ߽��: " ) );
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

static AcDbObjectId CreatePolyLine()
{
    int index = 2;       // ��ǰ�����Ĵ���
    ads_point ptStart;   // ���
    if ( acedGetPoint( NULL, _T( "\n�����һ�㣺" ), ptStart ) != RTNORM )
        return AcDbObjectId::kNull;

    ads_point ptPrevious, ptCurrent; // ǰһ���ο��㣬��ǰʰȡ�ĵ�
    acdbPointSet( ptStart, ptPrevious );
    AcDbObjectId polyId;             // ����ߵ�ID
    while( RTNORM == acedGetPoint( ptPrevious, _T( "\n������һ�㣺" ), ptCurrent ) )
    {
        if ( index == 2 )
        {
            // ���������
            AcDbPolyline* pPoly = new AcDbPolyline( 2 );
            AcGePoint2d ptGe1, ptGe2; // �����ڵ�
            ptGe1[X] = ptPrevious[X];
            ptGe1[Y] = ptPrevious[Y];
            ptGe2[X] = ptCurrent[X];
            ptGe2[Y] = ptCurrent[Y];
            pPoly->addVertexAt( 0, ptGe1 );
            pPoly->addVertexAt( 1, ptGe2 );

            // ���ӵ�ģ�Ϳռ�
            ArxUtilHelper::PostToModelSpace( pPoly );

            polyId = pPoly->objectId();
        }
        else if( index > 2 )
        {
            // �޸Ķ���ߣ��������һ������
            AcDbPolyline* pPoly;
            acdbOpenObject( pPoly, polyId, AcDb::kForWrite );

            AcGePoint2d ptGe;    // ���ӵĽڵ�
            ptGe[X] = ptCurrent[X];
            ptGe[Y] = ptCurrent[Y];

            pPoly->addVertexAt( index - 1, ptGe );

            pPoly->close();
        }

        index++;

        acdbPointSet( ptCurrent, ptPrevious );
    }

    return polyId;
}

static void GetPolyPoints( const AcDbObjectId& objId, AcGePoint3dArray& pts )
{
    AcDbEntity* pEnt;
    acdbOpenObject( pEnt, objId, AcDb::kForRead );

    AcDbPolyline* pPolyLine = AcDbPolyline::cast( pEnt );
    int n = pPolyLine->numVerts();
    acutPrintf( _T( "\n����εĵ����:%d" ), n );
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3d pt;
        pPolyLine->getPointAt( i, pt );
        pts.append( pt );
    }
    pEnt->close();
}

bool GetPolygonByJig( AcGePoint3dArray& pts )
{
    PolyLineJig jig;
    return jig.doJig( pts );
}

bool GetPolygonByCmd( AcGePoint3dArray& pts )
{
    // �����н������������
    AcDbObjectId objId = CreatePolyLine();
    if( objId.isNull() ) return false;

    // ��ȡ����ߵ�
    GetPolyPoints( objId, pts );

    // ɾ�������
    ArxEntityHelper::EraseObject2( objId, true );

    return true;
}


/*

// ��PolyLineJig.h�е�GetPolygonByCmd()��ʵ������
class PolyLineJig : public AcEdJig
{
public:
	PolyLineJig () ;
	~PolyLineJig () ;

	Adesk::Boolean doJig(AcGePoint3dArray& polygon);
} ;

static AcGePoint2d Point3D_To_2D(const AcGePoint3d& pt)
{
	return AcGePoint2d(pt.x, pt.y);
}

static AcGePoint3d Point2D_To_3D(const AcGePoint2d& pt)
{
	return AcGePoint3d(pt.x, pt.y, 0);
}

PolyLineJig::PolyLineJig () : AcEdJig ()
{
}

PolyLineJig::~PolyLineJig () {
}

Adesk::Boolean PolyLineJig::doJig(AcGePoint3dArray& polygon)
{
	setUserInputControls((UserInputControls)(kNullResponseAccepted));

	setDispPrompt(_T("\n�����ӱ߽��: "));
	AcGePoint3d pt;
	if(kNormal != acquirePoint(pt)) return Adesk::kFalse;

	// ��ȡ��1����
	polygon.append(pt);

	Adesk::Boolean ret = Adesk::kTrue;
	AcDbObjectId objId;
	while(ret)
	{
		setUserInputControls((UserInputControls)(kNullResponseAccepted));

		setDispPrompt(_T("\n�����ӱ߽��[�򰴿ո�/ESC�˳�]: "));
		AcGePoint3d next_pt;
		if(kNormal != acquirePoint(next_pt, pt)) break;

		if(polygon.contains(next_pt))
		{
			setDispPrompt(_T("\n�����������ظ���"));
			continue;
		}

		polygon.append(next_pt);
		pt = next_pt;
		if(polygon.length()==2)
		{
			AcDbPolyline* pPolyLine = new AcDbPolyline(2);
			pPolyLine->addVertexAt(0, Point3D_To_2D(polygon[0]));
			pPolyLine->addVertexAt(1, Point3D_To_2D(polygon[1]));
			if(!ArxUtilHelper::PostToModelSpace(pPolyLine))
			{
				delete pPolyLine;
				ret = Adesk::kFalse;
			}
			else
			{
				objId = pPolyLine->objectId();
			}
		}
		else
		{
			AcDbEntity* pEnt;
			if(Acad::eOk != acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite))
			{
				ret = Adesk::kFalse;
			}
			else
			{
				AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt);
				pPolyLine->addVertexAt(pPolyLine->numVerts(), Point3D_To_2D(next_pt));
				pEnt->close();
			}
		}
	}

	if(!objId.isNull()) ArxEntityHelper::EraseObject2(objId, true);
	if(!ret) polygon.removeAll();
	if(polygon.length()<3) ret = false;

	return ret;
}
*/