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
#include "GeoTool.h"

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

// ���ҵ�ֵ�ߵĶ˵�λ�������߽磬���ڱ߽��ϵ�λ��
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
        // ʼ��λ��
        int s = 0;
        for( int j = 0; j < i; i++ )
        {
            s += bpos[j];
        }
        // �յ�λ��
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

// ����1:
// ��Ҫ׷�ٵ�zֵ���մ�С�����˳������ʱ
// ��ֵ�ߵ��յ�ʼ������z���ٵķ���

// ����2:
// ������ڶ���߽磬���ŵ�ֵ�ߵ�ʼĩ��ֻ������һ���߽���
static void FindPointsOnBoundary(
    const PointArray& bpts, const IntArray& bpos,
    const DT_Point& ps, const DT_Point& pt,
    PointArray& z_bpts )
{
    int psi, pti, bs, bt;
    if( !FindPosOnBoundary( bpts, bpos, ps, pt, psi, pti, bs, bt ) ) return;

    // �жϷ���(false -- ��߽緽���෴)
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
            // �߽��Ǳպϵģ�bpts[bt-1]==bpts[bs]
            // ���, Ҫע������ظ������
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

    z_bpts.push_back( ps ); // �պ�
}

// �պϵ�ֵ��
static void EncloseContour(
    /* ׷�ٵõ��ĵ�ֵ�ߵ� */
    const PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    const ContourArray& cna,
    /* �����õ��ı߽�� */
    const PointArray& bpts,
    /* ��¼�߽���Ϣ(ÿ���߽�ĵ����) */
    const IntArray& bpos,
    /* ��¼�ϲ���ĵ�ֵ�ߺ���Ϣ */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    ContourExtArray& all_cna_ext )
{
    int n = cna.size();
    for( int i = 0; i < n; i++ )
    {
        // ��λ����i����ֵ��λ��
        int s = 0;
        for( int j = 0; j < i; j++ )
        {
            s += cna[j].cnpts_num;
        }
        int t = s + cna[i].cnpts_num;

        PointArray z_bpts; // ��¼�߽��
        if( t > s )
        {
            // ��ֵ�����λ��s���յ�λ��t-1
            if( !IsPointEqual( cnpts[s], cnpts[t - 1] ) )
            {
                // ��ֵ�߲��պ�,����Ҫ���ұ߽�
                FindPointsOnBoundary( bpts, bpos, cnpts[s], cnpts[t - 1], z_bpts );
            }
            //else if(cna[i].dir == -1)
            //{
            //	// ��ֵ�߱պϣ�������->�ڱ��
            //	// ��Ҫ������ı߽�(����=���߽�"��ȥ"�պϵ�ֵ��)
            //	// ��0���߽��ʾ���߽�
            //	std::copy(bpts.begin(), bpts.begin()+bpos[0], std::back_inserter(z_bpts));
            //}
        }

        // ���Ƶ�ֵ��
        std::copy( cnpts.begin() + s, cnpts.begin() + t, std::back_inserter( all_cnpts ) );
        // ���Ʊ߽��
        std::copy( z_bpts.begin(), z_bpts.end(), std::back_inserter( all_cnpts ) );

        // ���Ƶ�ֵ����Ϣ
        all_cna.push_back( cna[i] );

        // ���Ƶ�ֵ�ߵı߽���Ϣ
        DT_ContourExt z_contour_ext = {z_bpts.size(), -1};
        all_cna_ext.push_back( z_contour_ext );
    }

    // ���Ʊպϱ߽�,Ҳ��Ϊ��ֵ��
    /*std::copy(bpts.begin(), bpts.end(), std::back_inserter(all_cnpts));
    for(int i=0;i<(int)bpos.size();i++)
    {
    	DT_Contour z_contour = {-1, 0};
    	all_cna.push_back(z_contour);

    	DT_ContourExt z_contour_ext = {bpos[i], -1};
    	all_cna_ext.push_back(z_contour_ext);
    }*/
}

// �жϵ�p�Ƿ��ڵ�k����ֵ�ߵĶ����������?
static bool IsPointInContourPolygon(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, const DT_Point& p )
{
    // ��k����ֵ�ߵ�ʼĩλ��(s, t)
    int s = 0;
    for( int i = 0; i < k; i++ )
    {
        s += all_cna[i].cnpts_num + all_cna_ext[i].bpts_num;
    }
    int t = s + all_cna[k].cnpts_num + all_cna_ext[k].bpts_num;

    // ������������:��ֵ�߱պ�����ͱ߽�����
    PointArray cntPolygon;
    std::copy( all_cnpts.begin() + s, all_cnpts.begin() + t, std::back_inserter( cntPolygon ) );

    // ��ʾ�պϵ�ֵ�߷���(1��-1),���ŵ�ֵ��Ϊ0
    int dir = all_cna[k].dir;
    if( dir == 0 ) dir = 1;

    // �ж��������Ƿ�λ�ڶ������
    bool ret = IsPointInPolygon( p, cntPolygon );;
    if( dir == 1 )
        return ret;
    else
        return !ret;
}

// �жϵ�k����ֵ����������ֵ�ߵİ�����ϵ
static void BuildContourTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, IntArray& next_contours )
{
    // ����һ��������
    int ks = 0;
    for( int i = 0; i < k; i++ )
    {
        ks += all_cna[i].cnpts_num + all_cna_ext[i].bpts_num;
    }

    // һ����ֵ��������3����(ʼ���ĩ����ܻ��ڱ߽���)
    // �ҵ�ֵ�߲��ܷ�������
    // ȡ��ֵ�ߵĵ�2������Ϊ������
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

// �жϵ�k���߽���������ֵ�ߵİ�����ϵ
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

    // �Ա߽��е����е�����ж�
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
            // �߽��ϵ����е㶼�ڵ�i����ֵ�ߵĶ����������
            next_contours.push_back( i );
        }
    }
}

// �жϵ�k����ֵ��(��߽�)��������ֵ�ߵİ�����ϵ
static void BuildContourAndBoundaryTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    const ContourExtArray& all_cna_ext,
    int k, IntArray& next_contours )
{
    if( all_cna[k].cnpts_num > 0 )
    {
        // ��ֵ��+�߽���
        BuildContourTopolgy( all_cnpts, all_cna, all_cna_ext, k, next_contours );
    }
    //else if(all_cna_ext[k].bpts_num > 0)
    //{
    //	// �߽�
    //	BuildBoundaryTopolgy(all_cnpts, all_cna, all_cna_ext, k, next_contours);
    //	if(next_contours.empty()) return;

    //	// ͬʱ����Ҫ�Ա߽練����֤
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

// ������ֵ������
static void BuildContourTopolgy(
    const PointArray& all_cnpts,
    const ContourArray& all_cna,
    ContourExtArray& all_cna_ext )
{
    assert( all_cna.size() == all_cna_ext.size() );

    // ����ͼ�۵��������������˹�ϵ
    Digraph dg;
    // ��ӽڵ�
    NodeArray nodes;
    for( int i = 0; i < ( int )all_cna_ext.size(); i++ )
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
    // �����ֵ��c1������c2��,��c1->c2
    for( int i = 0; i < ( int )all_cna.size(); i++ )
    {
        IntArray next_contours;
        BuildContourAndBoundaryTopolgy( all_cnpts, all_cna, all_cna_ext, i, next_contours );
        for( int j = 0; j < ( int )next_contours.size(); j++ )
        {
            dg.addArc( nodes[i], nodes[next_contours[j]] );
        }
    }

    // ��������
    IntNodeMap order( dg );
    topologicalSort( dg, order );

    // �Խڵ㰴�ռ���õ�������˳��ֵ����
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

// �պϵ�ֵ�߲�������ֵ������
void EncloseContour_BuildTopolgy(
    /* ׷�ٵõ��ĵ�ֵ�ߵ� */
    const PointArray& cnpts,
    /* ��¼��ֵ����Ϣ(�����Լ�����λ��) */
    const ContourArray& cna,
    /* �����õ��ı߽�� */
    const PointArray& bpts,
    /* ��¼�߽���Ϣ(ÿ���߽�ĵ����) */
    const IntArray& bpos,
    /* ��¼�ϲ���ĵ�ֵ�ߺ���Ϣ */
    PointArray& all_cnpts,
    ContourArray& all_cna,
    ContourExtArray& all_cna_ext )
{
    // �պϵ�ֵ��
    EncloseContour( cnpts, cna, bpts, bpos, all_cnpts, all_cna, all_cna_ext );
    // ��������
    BuildContourTopolgy( all_cnpts, all_cna, all_cna_ext );
}