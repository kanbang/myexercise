#include "GeoDef.h"
#include <fstream>
using namespace std;

static void ReadPoints( ifstream& inFile, PointArray& pts )
{
    // 1) 读取第1行
    // v -节点个数
    // n - 维数
    // a - 属性个数
    // b - 边界标记
    int nv, n, a, b;
    inFile >> nv >> n >> a >> b;

    for( int i = 0; i < nv; i++ )
    {
        int ni;
        double x, y;
        inFile >> ni >> x >> y;

        Point pt = {x, y, 0};
        pts.push_back( pt );

        for( int j = 0; j < a; j++ )
        {
            double aa;
            inFile >> aa;
        }

        if( b != 0 )
        {
            double bb;
            inFile >> bb;
        }
    }
}

static void ReadSegments( ifstream& inFile, EdgeArray& segments )
{
    int ns, b;
    inFile >> ns >> b;

    for( int i = 0; i < ns; i++ )
    {
        int ei;
        inFile >> ei;

        int s, t;
        inFile >> s >> t;

        Edge seg = {s, t};
        segments.push_back( seg );

        if( b != 0 )
        {
            int bb;
            inFile >> bb;
        }
    }
}

static void ReadHoles( ifstream& inFile, PointArray& holes )
{
    int nh;
    inFile >> nh;

    for( int i = 0; i < nh; i++ )
    {
        int hi;
        inFile >> hi;

        double x, y;
        inFile >> x >> y;

        Point pt = {x, y, 0};
        holes.push_back( pt );
    }
}

void ParseInPolyFile( const string& path, PointArray& pts, EdgeArray& segments, PointArray& holes )
{
    ifstream inFile( path.c_str() );
    if( !inFile ) return;

    ReadPoints( inFile, pts );
    ReadSegments( inFile, segments );
    ReadHoles( inFile, holes );

    inFile.close();
}