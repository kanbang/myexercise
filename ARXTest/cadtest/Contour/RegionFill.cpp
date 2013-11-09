// 停止等值线填充的实现!!!!
// 尚未实现,已有的实现存在问题,无法下去
// 参考文章及文献:
// 1) http://bbs.sciencenet.cn/home.php?mod=space&uid=244606&do=blog&id=303491
// 2) 等值线自动建立拓扑关系算法与快速填充应用
// 3) 不规则区域等值线拓扑关系的建立及充填算法
// 4) 基于等值线拓扑走向的快速区域填充算法
// 5) 一种基于三角网的等值线自动填充算法

// 目前尝试使用区域填充算法(闭合等值线->建立拓扑关系->填充)
// 但考虑到一些复杂的情况：闭合的等值线以及内部有一些孔洞(内部边界)
// 此时，定义填充区域就变得非常复杂
// 后续如果有时间，可以考虑使用1)提到的三角形填充
// 当前cpp文件不进行编译(从项目中排除编译)
#include "Contour.h"
#include "GeoTool.h"

#include <algorithm>
#include <iterator>
#include <cassert>

/* 使用lemon库提供的数据结构定义通用的图(或网络) */
/* ListDigraph、Arc、Node等表示通风网络 */
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>
using namespace lemon;

/* 有向图 */
typedef ListDigraph Digraph;
typedef std::vector<Digraph::Node> NodeArray;
typedef Digraph::NodeMap<int> IntNodeMap;

// 链接lib
#ifdef _DEBUG
#pragma comment(lib, "lemond.lib")
#else
#pragma comment(lib, "lemon.lib")
#endif

// 查找等值线的端点位于哪条边界，且在边界上的位置
static bool FindPosOnBoundary(
    const PointArray& bpts,
    const IntArray& bpos,
    const DT_Point& ps, const DT_Point& pt,
    int& psi, int& pti,
    int& bs, int& bt )
{
    psi = -1;
    pti = -1;
    bs = -1;
    bt = -1;
    for( int i = 0; i < ( int )bpos.size(); i++ )
    {
        // 始点位置
        int s = 0;
        for( int j = 0; j < i; i++ )
        {
            s += bpos[j];
        }
        // 终点位置
        int t = s + bpos[i];

        psi = -1;
        pti = -1;
        bs = -1;
        bt = -1;
        for( int j = s; j < t - 1; j++ )
        {
            DT_Point bp1 = bpts[j], bp2 = bpts[j + 1];
            if( IsPointOnline( bp1, bp2, ps ) )
            {
                psi = j;
            }
            else if( IsPointOnline( bp1, bp2, pt ) )
            {
                pti = j;
            }
        }
        if( psi != -1 && pti != -1 )
        {
            bs = s;
            bt = t;
            break;
        }
    }
    return ( psi != -1 && pti != -1 );
}

// 假设1:
// 当要追踪的z值按照从小到大的顺序排列时
// 等值线的终点始终沿着z减少的方向

// 假设2:
// 如果存在多个边界，开放等值线的始末点只能落在一个边界上
static void FindPointsOnBoundary(
    const PointArray& bpts, const IntArray& bpos,
    const DT_Point& ps, const DT_Point& pt,
    PointArray& z_bpts )
{
    int psi, pti, bs, bt;
    if( !FindPosOnBoundary( bpts, bpos, ps, pt, psi, pti, bs, bt ) ) return;

    // 判断方向(false -- 与边界方向相反)
    bool dir = ( bpts[pti].z < pt.z );

    if( dir )
    {
        if( psi < pti )
        {
            for( int i = pti; i > psi; i-- )
            {
                z_bpts.push_back( bpts[i] );
            }
        }
        else
        {
            for( int i = pti; i > bs; i-- )
            {
                z_bpts.push_back( bpts[i] );
            }
            for( int i = bt - 1; i > psi; i-- )
            {
                z_bpts.push_back( bpts[i] );
            }
        }
    }
    else
    {
        if( psi < pti )
        {
            // 边界是闭合的，bpts[bt-1]==bpts[bs]
            // 因此, 要注意避免重复的情况
            for( int i = pti + 1; i < bt - 1; i++ )
            {
                z_bpts.push_back( bpts[i] );
            }
            for( int i = bs; i <= psi; i++ )
            {
                z_bpts.push_back( bpts[i] );
            }
        }
        else
        {
            for( int i = pti + 1; i <= psi; i++ )
            {
                z_bpts.push_back( bpts[i] );
            }
        }
    }

    z_bpts.push_back( ps ); // 闭合
}

// 闭合等值线
static void EncloseContour(
    /* 追踪得到的等值线点 */
    const PointArray& cnpts,
    /* 记录等值线信息(个数以及索引位置) */
    const ContourArray& cna,
    /* 搜索得到的边界点 */
    const PointArray& bpts,
    /* 记录边界信息(每条边界的点个数) */
    const IntArray& bpos,
    /* 记录合并后的等值线和信息 */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    ContourExtArray& all_cna_ext )
{
    int n = cna.size();
    for( int i = 0; i < n; i++ )
    {
        // 定位到第i条等值线位置
        int s = 0;
        for( int j = 0; j < i; j++ )
        {
            s += cna[j].cnpts_num;
        }
        int t = s + cna[i].cnpts_num;

        PointArray z_bpts; // 记录边界点
        if( t > s )
        {
            // 等值线起点位置s，终点位置t-1
            if( !IsPointEqual( cnpts[s], cnpts[t - 1] ) )
            {
                // 等值线不闭合,则需要查找边界
                FindPointsOnBoundary( bpts, bpos, cnpts[s], cnpts[t - 1], z_bpts );
            }
            //else if(cna[i].dir == -1)
            //{
            //	// 等值线闭合，且有外->内变大
            //	// 需要添加最大的边界(区域=最大边界"减去"闭合等值线)
            //	// 第0个边界表示最大边界
            //	std::copy(bpts.begin(), bpts.begin()+bpos[0], std::back_inserter(z_bpts));
            //}
        }

        // 复制等值点
        std::copy( cnpts.begin() + s, cnpts.begin() + t, std::back_inserter( all_cnpts ) );
        // 复制边界点
        std::copy( z_bpts.begin(), z_bpts.end(), std::back_inserter( all_cnpts ) );

        // 复制等值线信息
        all_cna.push_back( cna[i] );

        // 复制等值线的边界信息
        DT_ContourExt z_contour_ext = {z_bpts.size(), -1};
        all_cna_ext.push_back( z_contour_ext );
    }

    // 复制闭合边界,也视为等值线
    /*std::copy(bpts.begin(), bpts.end(), std::back_inserter(all_cnpts));
    for(int i=0;i<(int)bpos.size();i++)
    {
    	DT_Contour z_contour = {-1, 0};
    	all_cna.push_back(z_contour);

    	DT_ContourExt z_contour_ext = {bpos[i], -1};
    	all_cna_ext.push_back(z_contour_ext);
    }*/
}

// 判断点p是否在第k条等值线的多边形区域内?
static bool IsPointInContourPolygon(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, const DT_Point& p )
{
    // 第k条等值线的始末位置(s, t)
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += all_cna[i].cnpts_num + all_cna_ext[i].bpts_num;
    }
    int t = s + all_cna[k].cnpts_num + all_cna_ext[k].bpts_num;

    // 定义多边形区域:等值线闭合区域和边界区域
    PointArray cntPolygon;
    std::copy( all_cnpts.begin() + s, all_cnpts.begin() + t, std::back_inserter( cntPolygon ) );

    // 表示闭合等值线方向(1或-1),开放等值线为0
    int dir = all_cna[k].dir;
    if( dir == 0 ) dir = 1;

    // 判断特征点是否位于多边形内
    bool ret = IsPointInPolygon( p, cntPolygon );;
    if( dir == 1 )
        return ret;
    else
        return !ret;
}

// 判断第k条等值线与其它等值线的包含关系
static void BuildContourTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, IntArray& next_contours )
{
    // 查找一个特征点
    int ks = 0;
    for( int i = 0; i < k; i++ )
    {
        ks += all_cna[i].cnpts_num + all_cna_ext[i].bpts_num;
    }

    // 一条等值线至少有3个点(始点或末点可能会在边界上)
    // 且等值线不能发生交叉
    // 取等值线的第2个点作为特征点
    DT_Point p = all_cnpts[ks + 1];

    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        if( i == k ) continue;

        if( IsPointInContourPolygon( all_cnpts, all_cna, all_cna_ext, i, p ) )
        {
            next_contours.push_back( i );
        }
    }
}

// 判断第k个边界与其它等值线的包含关系
static void BuildBoundaryTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, IntArray& next_contours )
{
    int ks = 0;
    for( int i = 0; i < k; i++ )
    {
        ks += all_cna[i].cnpts_num + all_cna_ext[i].bpts_num;
    }
    int kt = ks + all_cna[k].cnpts_num + all_cna_ext[k].bpts_num;

    // 对边界中的所有点进行判断
    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        if( i == k ) continue;

        bool ret = true;
        for( int j = ks; j < kt; j++ )
        {
            DT_Point p = all_cnpts[j];
            if( !IsPointInContourPolygon( all_cnpts, all_cna, all_cna_ext, i, p ) )
            {
                ret = false;
                break;
            }
        }
        if( ret )
        {
            // 边界上的所有点都在第i条等值线的多边形区域内
            next_contours.push_back( i );
        }
    }
}

// 判断第k条等值线(或边界)与其它等值线的包含关系
static void BuildContourAndBoundaryTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, IntArray& next_contours )
{
    if( all_cna[k].cnpts_num > 0 )
    {
        // 等值线+边界线
        BuildContourTopolgy( all_cnpts, all_cna, all_cna_ext, k, next_contours );
    }
    //else if(all_cna_ext[k].bpts_num > 0)
    //{
    //	// 边界
    //	BuildBoundaryTopolgy(all_cnpts, all_cna, all_cna_ext, k, next_contours);
    //	if(next_contours.empty()) return;

    //	// 同时还需要对边界反向验证
    //	IntArray z_next_contours;
    //	for(int i=0;i<next_contours.size();i++)
    //	{
    //		IntArray t_next_contours;
    //		BuildContourTopolgy(all_cnpts, all_cna, all_cna_ext, next_contours[i], t_next_contours);
    //		if(t_next_contours.end == std::find(t_next_contours.begin(), t_next_contours.end(), k))
    //	}
    //}
}

struct SortNode
{
    SortNode( IntNodeMap& order_ ) : order( order_ ) {}

    bool operator()( const Digraph::Node& u, const Digraph::Node& v )
    {
        return ( order[u] < order[v] );
    }

    IntNodeMap& order;
};

static bool FindOutArcNode( Digraph& dg, Digraph::Node u, Digraph::Node v )
{
    bool ret = false;
    for( Digraph::OutArcIt e( dg, u ); e != INVALID; ++e )
    {
        if( v == dg.target( e ) )
        {
            ret = true;
            break;
        }
    }
    return ret;
}

// 构建等值线拓扑
static void BuildContourTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    ContourExtArray& all_cna_ext )
{
    assert( all_cna.size() == all_cna_ext.size() );

    // 采用图论的拓扑排序建立拓扑关系
    Digraph dg;
    // 添加节点
    NodeArray nodes;
    for( int i = 0; i < ( int )all_cna_ext.size(); i++ )
    {
        nodes.push_back( dg.addNode() );
    }
    // 节点关联属性(记录节点对应的等值线编号)
    // 与前面的nodes数组一起构成了双向映射
    IntNodeMap idm( dg );
    for( int i = 0; i < ( int )nodes.size(); i++ )
    {
        idm[nodes[i]] = i; // 节点的属性(对应的实际编号)
    }

    // 判断等值线包含关系, 建立拓扑网络
    // 例如等值线c1包含在c2中,则c1->c2
    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        IntArray next_contours;
        BuildContourAndBoundaryTopolgy( all_cnpts, all_cna, all_cna_ext, i, next_contours );
        for( int j = 0; j < ( int )next_contours.size(); j++ )
        {
            dg.addArc( nodes[i], nodes[next_contours[j]] );
        }
    }

    // 拓扑排序
    IntNodeMap order( dg );
    topologicalSort( dg, order );

    // 对节点按照计算得到的拓扑顺序值排序
    std::sort( nodes.begin(), nodes.end(), SortNode( order ) );

    for( int i = 0; i < ( int )nodes.size() - 1; i++ )
    {
        Digraph::Node u = nodes[i];
        for( int j = i + 1; j < ( int )nodes.size(); j++ )
        {
            Digraph::Node v = nodes[j];
            if( FindOutArcNode( dg, u, v ) )
            {
                all_cna_ext[idm[u]].next_contour = idm[v];
                break;
            }
        }
    }
}

// 闭合等值线并构建等值线拓扑
void EncloseContour_BuildTopolgy(
    /* 追踪得到的等值线点 */
    const PointArray& cnpts,
    /* 记录等值线信息(个数以及索引位置) */
    const ContourArray& cna,
    /* 搜索得到的边界点 */
    const PointArray& bpts,
    /* 记录边界信息(每条边界的点个数) */
    const IntArray& bpos,
    /* 记录合并后的等值线和信息 */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    ContourExtArray& all_cna_ext )
{
    // 闭合等值线
    EncloseContour( cnpts, cna, bpts, bpos, all_cnpts, all_cna, all_cna_ext );
    // 构建拓扑
    BuildContourTopolgy( all_cnpts, all_cna, all_cna_ext );
}