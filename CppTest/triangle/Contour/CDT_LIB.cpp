#include "Contour.h"

extern "C" {
#include "triangle.h"
};

void CDT_LIB(
    const PointArray& pts,
    const EdgeArray& segments,
    const PointArray& holes,
    EdgeArray& ea,
    TriangleArray& ta )
{
    /* ��ʼ��������� */
    triangulateio in;

    // ��ʼ��pointlist
    in.pointlist = ( REAL* )trimalloc( pts.size() * 2 * sizeof( REAL ) );
    in.numberofpoints = pts.size();
    for( int i = 0; i < ( int )pts.size(); i++ )
    {
        in.pointlist[2 * i] = pts[i].x;
        in.pointlist[2 * i + 1] = pts[i].y;
    }
    in.pointmarkerlist = 0;
    in.pointattributelist = 0;
    in.numberofpointattributes = 0;

    // ��ʼ��segmentlist(ʹ��-p����)
    if( segments.empty() )
    {
        in.segmentlist = 0;
        in.numberofsegments = 0;
    }
    else
    {
        in.segmentlist = ( int* )trimalloc( segments.size() * 2 * sizeof( int ) );
        in.numberofsegments = segments.size();
        for( int i = 0; i < ( int )segments.size(); i++ )
        {
            in.segmentlist[2 * i] = segments[i].s;
            in.segmentlist[2 * i + 1] = segments[i].t;
        }
    }
    in.segmentmarkerlist = 0;

    // ��ʼ��holelist
    if( holes.empty() )
    {
        in.holelist = 0;
        in.numberofholes = 0;
    }
    else
    {
        in.holelist = ( REAL* )trimalloc( holes.size() * 2 * sizeof( REAL ) );
        in.numberofholes = holes.size();
        for( int i = 0; i < ( int )holes.size(); i++ )
        {
            in.holelist[2 * i] = holes[i].x;
            in.holelist[2 * i + 1] = holes[i].y;
        }
    }
    // ��ʼ��regionlist
    in.regionlist = 0;
    in.numberofregions = 0;

    /* ��ʼ��������� */
    triangulateio out;
    out.trianglelist = 0;
    out.edgelist = 0;

    // ����triangle
    triangulate( "pceBPNz_Q", &in, &out, 0 );

    // ��ȡ���صļ�����
    if( out.trianglelist != 0 )
    {
        for( int i = 0; i < out.numberoftriangles; i++ )
        {
            DT_Triangle triangle = {out.trianglelist[3 * i],
                                    out.trianglelist[3 * i + 1],
                                    out.trianglelist[3 * i + 2]
                                   };
            ta.push_back( triangle );
        }
    }
    if( out.edgelist != 0 )
    {
        for( int i = 0; i < out.numberofedges; i++ )
        {
            DT_Edge edge = {out.edgelist[2 * i], out.edgelist[2 * i + 1]};
            ea.push_back( edge );
        }
    }

    // �����ڴ�
    trifree( ( VOID* )in.pointlist );
    trifree( ( VOID* )in.segmentlist );
    trifree( ( VOID* )in.holelist );
    trifree( ( VOID* )out.trianglelist );
    trifree( ( VOID* )out.edgelist );
}