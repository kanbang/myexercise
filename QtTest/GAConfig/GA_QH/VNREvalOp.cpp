#include <ECF.h>
#include "VNREvalOp.h"
#include "Tool.h"

#include <cfloat>
#include <vector>
typedef std::vector<double>          DoubleArray;

// ����һ�����ݾ���ֵ�ľ�����
static double MeanSquare( DoubleArray& dvalues )
{
    int n = dvalues.size();

    // 1��ƽ��ֵ
    double a = 0;
    for( int i = 0; i < n; i++ )
    {
        a += abs( dvalues[i] );
    }
    a = a / n;

    // 2��������
    double s = 0;
    for( int i = 0; i < n; i++ )
    {
        double v = abs( dvalues[i] );
        s += ( a - v ) * ( a - v );
    }
    return sqrt( s / n );
}

// ����һ������ƽ���͵�ƽ��ֵ
static double QuadraticSum( DoubleArray& dvalues )
{
    int n = dvalues.size();

    // ƽ����
    double s = 0;
    for( int i = 0; i < n; i++ )
    {
        s += pow( dvalues[i], 2.0 );
    }
    return sqrt( s / n );
}

// ���۷�������
static double EvalEdgeQ( Digraph& dg, VNR_EdgeMap& vnr_edge_map )
{
    EdgeArray E;
    DoubleArray D;

    // �ռ������Ѳ��֧������(�������-���Է���)
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNR_Edge_Data ed = vnr_edge_map[e];
        if( ed.bInitQ )
        {
            E.push_back( e );
            D.push_back( ed.cq - ed.q );
        }
    }

    //return MeanSquare(D);
    return QuadraticSum( D );
}

// ��������������
static double EvalNetworkH( Digraph& dg, VNR_Network_Data& vnr_network_data )
{
    double H = vnr_network_data.H;
    double CH = vnr_network_data.CH;
    return fabs( H - CH );
}

// ���۽ڵ�ѹ������
static double EvalNodeP( Digraph& dg, VNR_NodeMap& vnr_node_map )
{
    NodeArray U;
    DoubleArray D;

    // �ռ������Ѳ�ڵ������(����ѹ��-����ѹ��)
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        VNR_Node_Data nd = vnr_node_map[n];
        if( nd.bInitP )
        {
            U.push_back( n );
            D.push_back( nd.cp - nd.p );
        }
    }
    //return MeanSquare(D);
    return QuadraticSum( D );
}

VNREvalOp::VNREvalOp( Digraph& dg_, EdgeArray& E_,
                      VNR_Network_Data& vnr_network_data_,
                      VNR_EdgeMap& vnr_edge_map_,
                      VNR_NodeMap& vnr_node_map_ )
    : dg( dg_ ), E( E_ ),
      vnr_network_data( vnr_network_data_ ),
      vnr_edge_map( vnr_edge_map_ ),
      vnr_node_map( vnr_node_map_ )
{

}

/*
http://baike.baidu.com/view/829823.htm
��һ������������
	1�����Ժ���ת�������ʽ���£�
		y=(x-MinValue)/(MaxValue-MinValue)
	˵����x��y�ֱ�Ϊת��ǰ�����ֵ��MaxValue��MinValue�ֱ�Ϊ���������ֵ����Сֵ��
	2����������ת�������ʽ����
		y=log10(x)
	˵������10Ϊ�׵Ķ�������ת����
	3�������к���ת�������ʽ����
		y=atan(x)*2/PI
*/
static double Normalize( double x )
{
    return atan( x );
}

FitnessP VNREvalOp::evaluate( IndividualP individual )
{
    // ����С����
    FitnessP fitness = static_cast<FitnessP> ( new FitnessMin );

    // ������-�����Ʊ���
    Binary::Binary* gen = ( Binary::Binary* ) individual->at( 0 ).get();

    // ��ʼĬ��ֵΪ����������
    fitness->setValue( DBL_MAX );

    // ����(Ⱦɫ��)ת���ɷ���ֵ
    GenotypToR( dg, E, gen, vnr_edge_map );

    // ͨ���������ɹ����õ����ݣ���¼��vnr_edge_map��vnr_node_map��
    // ��Ӧֵ����˼·��
    //	1) �������������Է����Ĳ�ֵ��ƽ���ͣ����ƽ���õ�һ��dq;
    //	2) �������������������������ֵ�ľ���ֵ��Ϊһ��dh
    //	3) h= r*q^2 ==> dh = 2*r*q ==> dh/h = (2*r*q^2/h)*(dq/q) ==> dh/h = 2dq/q;
    //	   ���߱�����ϵΪ2:1
    //  4) 0< dq/q < 1����0 < dh/h < 1
    //	5)
    if( VNR_VNC( dg, vnr_edge_map, vnr_node_map, vnr_network_data ) )
    {
        int n = gen->variables.size(); // ��֧����
        double Q = vnr_network_data.Q;
        double H = vnr_network_data.H;
        double CH = vnr_network_data.CH;
        double CR = CH / ( Q * Q );

        double dq = EvalEdgeQ( dg, vnr_edge_map );
        double dh = EvalNetworkH( dg, vnr_network_data );

        double c = 0.667;
        //double f = c*f1 + (1-c)*f2;

        //double f = abs(CR*dq*dq - dh);
        //double f = abs(dh);
        //double f = abs(dq);
        double f = abs( c * n * n * CR * dq * dq + ( 1 - c ) * dh );
        fitness->setValue( f );
    }

    return fitness;
}
