#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

#include "JournalHelper.h"

void GenDatas( PointArray& pts, EdgeArray& edges, IntArray& face )
{
    // 测试点
    CFD_Point test_points[] =
    {
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0}
    };
    int n = sizeof( test_points ) / sizeof( test_points[0] );
    std::copy( test_points, test_points + n, std::back_inserter( pts ) );

    // 测试分支
    CFD_Edge test_edges[] =
    {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 1}
    };
    int m = sizeof( test_edges ) / sizeof( test_edges[0] );
    std::copy( test_edges, test_edges + m, std::back_inserter( edges ) );

    // 测试面
    int test_face[] =
    {
        0, 1, 2, 3
    };
    int k = sizeof( test_face ) / sizeof( test_face[0] );
    std::copy( test_face, test_face + k, std::back_inserter( face ) );
}

int  main()
{
    // 生成测试数据
    PointArray pts;
    EdgeArray edges;
    IntArray face;
    GenDatas( pts, edges, face );

    JournalHelper jh;

    for( int i = 0; i < ( int )pts.size(); i++ )
    {
        jh.addPoint( pts[i] );
    }

    for( int i = 0; i < ( int )edges.size(); i++ )
    {
        jh.addEdge( edges[i] );
    }

    int face_id = jh.addFace();
    for( int i = 0; i < ( int )face.size(); i++ )
    {
        jh.addEdgeToFace( face[i], face_id );
    }

    const char filename[] = "d:\\test.jou";
    jh.write( filename );

    return 0;
}