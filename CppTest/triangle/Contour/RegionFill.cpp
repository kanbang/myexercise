// ֹͣ��ֵ������ʵ��!!!!
// ��δʵ��,���е�ʵ�ִ�������,�޷���ȥ
// �ο����¼�����:
// 1) http://bbs.sciencenet.cn/home.php?mod=space&uid=244606&do=blog&id=303491
// 2) ��ֵ���Զ��������˹�ϵ�㷨��������Ӧ��
// 3) �����������ֵ�����˹�ϵ�Ľ����������㷨
// 4) ���ڵ�ֵ����������Ŀ�����������㷨
// 5) һ�ֻ����������ĵ�ֵ���Զ�����㷨

// Ŀǰ����ʹ����������㷨(�պϵ�ֵ��->�������˹�ϵ->���)
// �����ǵ�һЩ���ӵ�������պϵĵ�ֵ���Լ��ڲ���һЩ�׶�(�ڲ��߽�)
// ��ʱ�������������ͱ�÷ǳ�����
// ���������ʱ�䣬���Կ���ʹ��1)�ᵽ�����������
// ��ǰcpp�ļ������б���(����Ŀ���ų�����)
#include "Contour.h"
#include "GeoAlgo.h"

#include <algorithm>
#include <iterator>
#include <cassert>

/* ʹ��lemon���ṩ�����ݽṹ����ͨ�õ�ͼ(������) */
/* ListDigraph��Arc��Node�ȱ�ʾͨ������ */
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>
using namespace lemon;

/* ����ͼ */
typedef ListDigraph Digraph;
typedef std::vector<Digraph::Node> NodeArray;
typedef Digraph::NodeMap<int> IntNodeMap;

// ����lib
#ifdef _DEBUG
#pragma comment(lib, "lemond.lib")
#else
#pragma comment(lib, "lemon.lib")
#endif

// ���ҵ�ֵ���ڱ߽��ϵ�λ��
static bool FindPosOnBoundary(
    const PointArray& bpts,
    const DT_Point& ps,
    const DT_Point& pt,
    int& psi, int& pti )
{
    psi = -1;
    pti = -1;
    for( int j = 0; j < ( int )bpts.size() - 1; j++ )
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
    return ( psi != -1 && pti != -1 );
}

static void FindOpenPointsOnBoundary( const PointArray& bpts, int psi, int pti, bool dir, PointArray& z_bpts )
{
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
            for( int i = pti; i >= 0; i-- )
            {
                z_bpts.push_back( bpts[i] );
            }
            for( int i = bpts.size() - 1; i > psi; i-- )
            {
                z_bpts.push_back( bpts[i] );
            }
        }
    }
    else
    {
        if( psi < pti )
        {
            // �߽��Ǳպϵģ�bpts[bt-1]==bpts[bs]
            // ���, Ҫע������ظ������
            for( int i = pti + 1; i < ( int )bpts.size() - 1; i++ )
            {
                z_bpts.push_back( bpts[i] );
            }
            for( int i = 0; i <= psi; i++ )
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
}

// ����1:
// ��Ҫ׷�ٵ�zֵ���մ�С�����˳������ʱ
// ��ֵ�ߵ��յ�ʼ������z���ٵķ���

// ����2:
// ������ڶ���߽磬���ŵ�ֵ�ߵ�ʼĩ��ֻ������һ���߽���
static void FindOpenPointsOnBoundary(
    const PointArray& bpts,
    const DT_Point& ps,
    const DT_Point& pt,
    /* ���zֵ����ı߽�� */
    PointArray& low_bpts,
    /* ���zֵ����ı߽�� */
    PointArray& high_bpts )
{
    int psi, pti;
    if( !FindPosOnBoundary( bpts, ps, pt, psi, pti ) ) return;

    FindOpenPointsOnBoundary( bpts, psi, pti, bpts[pti].z < pt.z, low_bpts );
    FindOpenPointsOnBoundary( bpts, psi, pti, bpts[pti].z > pt.z, high_bpts );

    low_bpts.push_back( ps );
    high_bpts.push_back( ps );
}

// �պϵ�ֵ��
static void EncloseContour(
    /* ׷�ٵõ��ĵ�ֵ�ߵ� */
    const PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    const ContourArray& cna,
    /* �߽����� */
    const PointArray& bpts,
    /* ��¼�ϲ���ĵ�ֵ�ߺ���Ϣ */
    PointArray& all_cnpts,
    ContourArray& all_cna )
{
    for( int i = 0; i < ( int )cna.size(); i++ )
    {
        // ��λ����i����ֵ��λ��
        int s = 0;
        for( int j = 0; j < i; j++ )
        {
            s += cna[j].cnpts_num;
        }
        int t = s + cna[i].cnpts_num;

        if( t > s )
        {
            // ��ֵ�����λ��s, �յ�λ��t-1
            if( !IsPointEqual( cnpts[s], cnpts[t - 1] ) )
            {
                // ��ֵ�߲��պ�,����Ҫ���ұ߽�
                PointArray low_bpts, high_bpts;           // ��¼�߽��
                int dir1 = cna[i].dir, dir2 = -1 * dir1;  // ��¼����
                int z1 = cna[i].zi, z2 = z1 + 1;          // ��¼zֵ����

                FindOpenPointsOnBoundary( bpts, cnpts[s], cnpts[t - 1], low_bpts, high_bpts );

                // 1) ����low_bpts
                // ���Ƶ�ֵ��
                std::copy( cnpts.begin() + s, cnpts.begin() + t, std::back_inserter( all_cnpts ) );
                // ���Ʊ߽��
                std::copy( low_bpts.begin(), low_bpts.end(), std::back_inserter( all_cnpts ) );

                // ���Ƶ�ֵ����Ϣ
                DT_Contour cnt = cna[i];
                cnt.bpts_num = low_bpts.size();
                cnt.dir = dir1;
                cnt.zi = z1;
                all_cna.push_back( cnt );

                // 2) ����high_bpts
                // ���Ƶ�ֵ��
                std::copy( cnpts.begin() + s, cnpts.begin() + t, std::back_inserter( all_cnpts ) );
                // ���Ʊ߽��
                std::copy( high_bpts.begin(), high_bpts.end(), std::back_inserter( all_cnpts ) );

                // ���Ƶ�ֵ����Ϣ
                /*DT_Contour */
                cnt = cna[i];
                cnt.bpts_num = high_bpts.size();
                cnt.dir = dir2;
                cnt.zi = z2;
                all_cna.push_back( cnt );
            }
            else
            {
                // ��ֵ�߱պ�
                std::copy( cnpts.begin() + s, cnpts.begin() + t, std::back_inserter( all_cnpts ) );

                // ���Ƶ�ֵ����Ϣ
                DT_Contour cnt = cna[i];
                if( cnt.dir < 0 )
                {
                    cnt.zi = cnt.zi + 1;
                    cnt.dir = 1;
                }
                all_cna.push_back( cnt );
            }
        }
        else
        {
            // �õ�ֵ��û������
            all_cna.push_back( cna[i] );
        }
    }
}

// �жϵ�p�Ƿ��ڵ�k����ֵ�ߵĶ����������?
static bool IsPointInContourPolygon(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    int k, const DT_Point& p )
{
    // ��k����ֵ�ߵ�ʼĩλ��(s, t)
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += all_cna[i].cnpts_num + all_cna[i].bpts_num;
    }
    int t = s + all_cna[k].cnpts_num + all_cna[k].bpts_num;

    // ������������:��ֵ�� + �߽�
    PointArray cntPolygon;
    std::copy( all_cnpts.begin() + s, all_cnpts.begin() + t, std::back_inserter( cntPolygon ) );

    // �ж��������Ƿ�λ�ڶ������
    return IsPointInPolygon( p, cntPolygon );;
}

// �жϵ�k����ֵ����������ֵ�ߵİ�����ϵ
static void BuildContourTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    int k, IntArray& next_contours )
{
    int ks = 0;
    for( int i = 0; i < k; i++ )
    {
        ks += all_cna[i].cnpts_num + all_cna[i].bpts_num;
    }
    int kt = ks + all_cna[k].cnpts_num + all_cna[k].bpts_num;

    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        if( i == k ) continue;

        bool ret = true;
        for( int j = ks; j < kt; j++ )
        {
            DT_Point p = all_cnpts[j];
            if( !IsPointInContourPolygon( all_cnpts, all_cna, i, p ) )
            {
                ret = false;
                break;
            }
        }
        if( ret )
        {
            // �߽��ϵ����е㶼�ڵ�i����ֵ�ߵĶ����������
            next_contours.push_back( i );
        }
    }
}

// �жϵ�k����ֵ��(��߽�)��������ֵ�ߵİ�����ϵ
static void BuildContourAndBoundaryTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    int k, IntArray& next_contours )
{
    if( all_cna[k].cnpts_num > 0 )
    {
        BuildContourTopolgy( all_cnpts, all_cna, k, next_contours );
    }
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

// ������ֵ������
static void BuildContourTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    IntArray& sortPos )
{
    // ����ͼ�۵��������������˹�ϵ
    Digraph dg;
    // ���ӽڵ�
    NodeArray nodes;
    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        nodes.push_back( dg.addNode() );
    }
    // �ڵ��������(��¼�ڵ��Ӧ�ĵ�ֵ�߱��)
    // ��ǰ���nodes����һ�𹹳���˫��ӳ��
    IntNodeMap idm( dg );
    for( int i = 0; i < ( int )nodes.size(); i++ )
    {
        idm[nodes[i]] = i; // �ڵ������(��Ӧ��ʵ�ʱ��)
    }

    // �жϵ�ֵ�߰�����ϵ, ������������
    // �����ֵ��c1������c2��,��c2->c1
    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        IntArray next_contours;
        BuildContourAndBoundaryTopolgy( all_cnpts, all_cna, i, next_contours );
        for( int j = 0; j < ( int )next_contours.size(); j++ )
        {
            dg.addArc( nodes[next_contours[j]], nodes[i] );
        }
    }

    // ��������
    IntNodeMap order( dg );
    topologicalSort( dg, order );

    // �Խڵ㰴�ռ���õ�������˳��ֵ����
    std::sort( nodes.begin(), nodes.end(), SortNode( order ) );

    for( int i = 0; i < ( int )nodes.size(); i++ )
    {
        Digraph::Node u = nodes[i];
        sortPos.push_back( idm[u] );
    }
}

static int SearchMaxZIndex(	const ContourArray& cna )
{
    int maxZi = -1;
    for( int i = 0; i < ( int )cna.size(); i++ )
    {
        // ����ıպϵ�ֵ��
        if( cna[i].dir == -1 && cna[i].zi > maxZi )
        {
            maxZi = cna[i].zi;
        }
    }
    return maxZi;
}

static void AddOutMostBoundary(
    int maxZi,
    const PointArray& bpts,
    PointArray& all_cnpts,
    ContourArray& all_cna )
{
    std::copy( bpts.begin(), bpts.end(), std::back_inserter( all_cnpts ) );

    DT_Contour cnt = {maxZi, 0, 0, bpts.size()};
    all_cna.push_back( cnt );
}

static void testxx()
{
    PointArray polygon;
    DT_Point p1 = {0, 0}, p2 = {1, 1}, p3 = {2, 0}, p4 = {0.5, 0.5};
    polygon.push_back( p1 );
    polygon.push_back( p2 );
    polygon.push_back( p3 );

    /*
    * ���޸�
    * ����pλ�ڶ�����ϵ�ʱ��Ҳ��Ϊ����
    * ���ڶ����Լ����ϵĵ㶼��Ϊ����
    * ��ˣ�����Ϊż�������ڶ������
    */
    bool ret = IsPointInPolygon( p1, polygon );
}

// �պϵ�ֵ�߲�������ֵ������
void EncloseContour_BuildTopolgy(
    /* ׷�ٵõ��ĵ�ֵ�ߵ� */
    const PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    const ContourArray& cna,
    /* �����õ��ı߽�� */
    const PointArray& bpts,
    /* ��¼�ϲ���ĵ�ֵ�ߺ���Ϣ */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    IntArray& sortPos )
{
    PointArray bounds;
    std::copy( bpts.begin(), bpts.end(), std::back_inserter( bounds ) );
    if( !IsPointEqual( bounds.front(), bounds.back() ) )
    {
        bounds.push_back( bounds.front() );
    }

    // �պϵ�ֵ��
    EncloseContour( cnpts, cna, bounds, all_cnpts, all_cna );

    // �������zֵ����
    int maxZi = SearchMaxZIndex( cna );
    if( maxZi != -1 )
    {
        // ���������Χ�߽�
        AddOutMostBoundary( maxZi, bounds, all_cnpts, all_cna );
    }

    // ��������
    BuildContourTopolgy( all_cnpts, all_cna, sortPos );

    //testxx();
}