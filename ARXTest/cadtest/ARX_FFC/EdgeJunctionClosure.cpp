#include "stdafx.h"

#include "GeoDef.h"
#include "DoubleLine.h"

#include <algorithm>
#include <vector>

/* ȫ�ֺ���(DoubleLineHelper.cpp) */
extern void GetNodePoints( const AcDbVoidPtrArray& lines, AcGePoint3dArray& pts );
extern void FindLinesByPoint( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt, AcDbIntArray& linePos );

struct JunctionEdgeInfo
{
    int pos;               // ͼԪ��id
    bool startOrEnd;       // ���ӵ���ʼ�㻹��ĩ��
    AcGeVector3d angle;    // �������췽������
    double width;          // ���

    bool operator==( const JunctionEdgeInfo& info )
    {
        return ( pos == info.pos ) &&
               ( startOrEnd == info.startOrEnd ) &&
               ( angle == info.angle );
    }
};

typedef std::vector<JunctionEdgeInfo> EdgeInfo;
typedef EdgeInfo::iterator EdgeInfoIter;

static void BuildJunctionEdgeInfo(
    const AcDbVoidPtrArray& lines,
    const AcDbIntArray& linePos,
    const AcGePoint3d& junctionPt,
    EdgeInfo& ges )
{
    int n = linePos.length();
    for( int i = 0; i < n; i++ )
    {
        int pos = linePos.at( i );
        DoubleLine* pEdge = ( DoubleLine* )lines[pos];

        JunctionEdgeInfo info = {pos, true, AcGeVector3d::kIdentity, pEdge->getWidth()};

        AcGePoint3d startPt, endPt;
        pEdge->getSEPoint( startPt, endPt );
        if( startPt == junctionPt )
        {
            info.startOrEnd = true;
            info.angle = pEdge->getStartPointInExtendAngle().normalize();
        }
        else if( endPt == junctionPt )
        {
            info.startOrEnd = false;
            info.angle = pEdge->getEndPointInExtendAngle().normalize();
        }

        if( info.angle.isZeroLength() ) continue; // ��������ʾ������պ�
        //acutPrintf(_T("\nid:%d\tangle:%.3f\tdraw:%s"),
        //			info.pos,
        //			info.angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //			pEdge->getCurrentDraw());

        ges.push_back( info );
    }
}

struct SortEdgeByAngle
{
    bool operator()( const JunctionEdgeInfo& info1, const JunctionEdgeInfo& info2 )
    {
        double angle1 = info1.angle.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        double angle2 = info2.angle.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        return angle1 < angle2;
    }
};

static void SortJunctionEdge( EdgeInfo& ges )
{
    // ���սǶȴ�С������ʱ�룩
    std::sort( ges.begin(), ges.end(), SortEdgeByAngle() ); // ����ʹ��list��ֻ��ʹ���������
}

static AcGeVector3d CaclAverageVector2( const AcGeVector3d& v1, double w1, const AcGeVector3d& v2, double w2 )
{
    // ��v1->v2��ת��
    double angle = v2.angleTo( v1, -AcGeVector3d::kZAxis );

    // ��������v1�ĳ���
    double L1 = 0.5 * w1 / sin( angle );

    // ��������L2�ĳ���
    double L2 = 0.5 * w2 / sin( angle );

    // �������Ľ�����
    AcGeVector3d v3 = v1 * L2 + v2 * L1;

    // ������"�н�"����PIʱ�������Ƿ����
    //if(angle >= PI)
    //{
    //	//acutPrintf(_T("\n�н�:%.3f   ��������..."), angle);
    //	v3.negate();
    //}

    return v3;
}

static void DealWithBoundary2( const AcDbVoidPtrArray& lines,
                               const JunctionEdgeInfo& info,
                               const AcGePoint3d& junctionPt,
                               const AcGeVector3d& v )
{
    DoubleLine* pEdge = ( DoubleLine* )lines[info.pos];

    //acutPrintf(_T("\n���߽Ƕ�:%.3f"), v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));
    AcGeRay3d boundaryLine( junctionPt, v );
    if( info.startOrEnd )
    {
        pEdge->dealWithStartPointBoundary( boundaryLine );
    }
    else
    {
        pEdge->dealWithEndPointBoundary( boundaryLine );
    }
}

// ������Ҫ2��Ԫ�ز�����ȷ�ıպ�
static void EdgeJunctionClosureImpl( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt, EdgeInfo& ges )
{
    //acutPrintf(_T("\n�����е�Ԫ�ظ���:%d"), ges.size());
    if( ges.size() == 1 )
    {
        ges.push_back( ges.front() );
    }

    // �ѵ�1����ӵ�ĩβ������ѭ��
    ges.push_back( ges.front() );

    // ��¼ÿ�δ���պϵĵ�ǰ����
    AcGeVector3d v3 = ges.front().angle;
    v3.rotateBy( PI / 2, AcGeVector3d::kZAxis );

    for( EdgeInfoIter itr = ges.begin(); itr != ges.end(); itr++ )
    {
        EdgeInfoIter itr2 = itr + 1;
        if( itr2 == ges.end() ) break;

        //acutPrintf(_T("\n���1�Ƕ�:%.3f, ���2�Ƕ�:%.3f"),
        //	itr->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //	itr2->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));

        AcGeVector3d cv = itr->angle.crossProduct( itr2->angle ); // ���(����нǵ�=0��PI��������=0)
        //if(cv.length() > 0.001)
        //acutPrintf(_T("\n��������:%.5f"), cv.length());
        if( !cv.isZeroLength() )
        {
            //acutPrintf(_T("\n��˳���=%.3f"), cv.length());
            v3 = CaclAverageVector2( itr->angle, itr->width, itr2->angle, itr2->width );;
        }
        else
        {
            // ƽ��(�н�=0��PI)
            //acutPrintf(_T("\n���=0"));
            v3.negate();
        }

        DealWithBoundary2( lines, *itr, junctionPt, v3 );
        DealWithBoundary2( lines, *itr2, junctionPt, v3 );
    }
}

static void EdgeJunctionClosure( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt )
{
    //acutPrintf(_T("\n�պϵ�:(%.3f, %.3f)"), junctionPt.x, junctionPt.y);
    AcDbIntArray linePos;
    FindLinesByPoint( lines, junctionPt, linePos );

    int len = linePos.length();
    //acutPrintf(_T("\n�ҵ�Ҫ����պϵķ�֧����:%d"), len);
    if( len < 1 ) return;

    EdgeInfo ges;
    BuildJunctionEdgeInfo( lines, linePos, junctionPt, ges );
    if( ges.size() > 0 )
    {
        SortJunctionEdge( ges );
        EdgeJunctionClosureImpl( lines, junctionPt, ges );
    }
}

// flag��ʾ����DoubleLine�ཻ��ȡ��λ��
static AcGePoint3d GetPolygonPoint( DoubleLine* pLine, bool startOrEnd, bool flag )
{
    AcGePoint3d pt;
    if( startOrEnd )
    {
        AcGePoint3d ls, rs;
        pLine->getStartPoints( ls, rs );
        pt = ( flag ? rs : ls );
    }
    else
    {
        AcGePoint3d le, re;
        pLine->getEndPoints( le, re );

        pt = ( flag ? le : re );
    }
    return pt;
}

static void AddPolygonPoints( const AcDbVoidPtrArray& lines, EdgeInfo& ges, int s, int t, AcGePoint3dArray& pts )
{
    int n = ges.size();
    for( int j = s; j < t; j++ )
    {
        DoubleLine* pLine = ( DoubleLine* )lines[ges[j].pos];
        pts.append( GetPolygonPoint( pLine, ges[j].startOrEnd, true ) );
    }
}

static void BuildPolygonPoints( const AcDbVoidPtrArray& lines, EdgeInfo& ges )
{
    int n = ges.size();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint3dArray pts;
        AddPolygonPoints( lines, ges, i + 1, n, pts );
        AddPolygonPoints( lines, ges, 0, i + 1, pts );

        DoubleLine* pLine = ( DoubleLine* )lines[ges[i].pos];
        if( ges[i].startOrEnd )
        {
            pLine->appendStartPolygon( pts );
        }
        else
        {
            pLine->appendEndPolygon( pts );
        }
    }
}

static void BuildPolygonPoints( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt )
{
    AcDbIntArray linePos;
    FindLinesByPoint( lines, junctionPt, linePos );

    int len = linePos.length();
    if( len < 1 ) return;

    EdgeInfo ges;
    BuildJunctionEdgeInfo( lines, linePos, junctionPt, ges );
    if( ges.size() > 2 )
    {
        SortJunctionEdge( ges );
        BuildPolygonPoints( lines, ges );
    }
}

void EdgeJunctionClourse( const AcDbVoidPtrArray& lines )
{
    AcGePoint3dArray pts;
    GetNodePoints( lines, pts );

    for( int i = 0; i < pts.length(); i++ )
    {
        EdgeJunctionClosure( lines, pts[i] );
    }

    for( int i = 0; i < pts.length(); i++ )
    {
        BuildPolygonPoints( lines, pts[i] );
    }
}