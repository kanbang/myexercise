#include "stdafx.h"

#include "GeoDef.h"
#include "DoubleLine.h"

#include <algorithm>
#include <vector>

/* 全局函数(DoubleLineHelper.cpp) */
extern void GetNodePoints( const AcDbVoidPtrArray& lines, AcGePoint3dArray& pts );
extern void FindLinesByPoint( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt, AcDbIntArray& linePos );

struct JunctionEdgeInfo
{
    int pos;               // 图元的id
    bool startOrEnd;       // 连接点是始点还是末点
    AcGeVector3d angle;    // 向内延伸方向向量
    double width;          // 宽度

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

        if( info.angle.isZeroLength() ) continue; // 零向量表示不处理闭合
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
    // 按照角度大小排序（逆时针）
    std::sort( ges.begin(), ges.end(), SortEdgeByAngle() ); // 不能使用list，只能使用随机容器
}

static AcGeVector3d CaclAverageVector2( const AcGeVector3d& v1, double w1, const AcGeVector3d& v2, double w2 )
{
    // 从v1->v2的转角
    double angle = v2.angleTo( v1, -AcGeVector3d::kZAxis );

    // 计算向量v1的长度
    double L1 = 0.5 * w1 / sin( angle );

    // 计算向量L2的长度
    double L2 = 0.5 * w2 / sin( angle );

    // 计算中心角向量
    AcGeVector3d v3 = v1 * L2 + v2 * L1;

    // 向量的"夹角"大于PI时，向量是反向的
    //if(angle >= PI)
    //{
    //	//acutPrintf(_T("\n夹角:%.3f   向量反向..."), angle);
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

    //acutPrintf(_T("\n射线角度:%.3f"), v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));
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

// 至少需要2个元素才能正确的闭合
static void EdgeJunctionClosureImpl( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt, EdgeInfo& ges )
{
    //acutPrintf(_T("\n队列中的元素个数:%d"), ges.size());
    if( ges.size() == 1 )
    {
        ges.push_back( ges.front() );
    }

    // 把第1个添加到末尾，构成循环
    ges.push_back( ges.front() );

    // 记录每次处理闭合的当前向量
    AcGeVector3d v3 = ges.front().angle;
    v3.rotateBy( PI / 2, AcGeVector3d::kZAxis );

    for( EdgeInfoIter itr = ges.begin(); itr != ges.end(); itr++ )
    {
        EdgeInfoIter itr2 = itr + 1;
        if( itr2 == ges.end() ) break;

        //acutPrintf(_T("\n巷道1角度:%.3f, 巷道2角度:%.3f"),
        //	itr->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //	itr2->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));

        AcGeVector3d cv = itr->angle.crossProduct( itr2->angle ); // 叉乘(如果夹角等=0或PI，则向量=0)
        //if(cv.length() > 0.001)
        //acutPrintf(_T("\n向量长度:%.5f"), cv.length());
        if( !cv.isZeroLength() )
        {
            //acutPrintf(_T("\n叉乘长度=%.3f"), cv.length());
            v3 = CaclAverageVector2( itr->angle, itr->width, itr2->angle, itr2->width );;
        }
        else
        {
            // 平行(夹角=0或PI)
            //acutPrintf(_T("\n叉乘=0"));
            v3.negate();
        }

        DealWithBoundary2( lines, *itr, junctionPt, v3 );
        DealWithBoundary2( lines, *itr2, junctionPt, v3 );
    }
}

static void EdgeJunctionClosure( const AcDbVoidPtrArray& lines, const AcGePoint3d& junctionPt )
{
    //acutPrintf(_T("\n闭合点:(%.3f, %.3f)"), junctionPt.x, junctionPt.y);
    AcDbIntArray linePos;
    FindLinesByPoint( lines, junctionPt, linePos );

    int len = linePos.length();
    //acutPrintf(_T("\n找到要处理闭合的分支个数:%d"), len);
    if( len < 1 ) return;

    EdgeInfo ges;
    BuildJunctionEdgeInfo( lines, linePos, junctionPt, ges );
    if( ges.size() > 0 )
    {
        SortJunctionEdge( ges );
        EdgeJunctionClosureImpl( lines, junctionPt, ges );
    }
}

// flag表示两条DoubleLine相交，取点位置
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