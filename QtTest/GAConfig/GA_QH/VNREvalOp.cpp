#include <ECF.h>
#include "VNREvalOp.h"
#include "Tool.h"

#include <cfloat>
#include <vector>
typedef std::vector<double>          DoubleArray;

// 计算一组数据绝对值的均方差
static double MeanSquare( DoubleArray& dvalues )
{
    int n = dvalues.size();

    // 1、平均值
    double a = 0;
    for( int i = 0; i < n; i++ )
    {
        a += abs( dvalues[i] );
    }
    a = a / n;

    // 2、均方差
    double s = 0;
    for( int i = 0; i < n; i++ )
    {
        double v = abs( dvalues[i] );
        s += ( a - v ) * ( a - v );
    }
    return sqrt( s / n );
}

// 计算一组数据平方和的平均值
static double QuadraticSum( DoubleArray& dvalues )
{
    int n = dvalues.size();

    // 平方和
    double s = 0;
    for( int i = 0; i < n; i++ )
    {
        s += pow( dvalues[i], 2.0 );
    }
    return sqrt( s / n );
}

// 评价风量数据
static double EvalEdgeQ( Digraph& dg, VNR_EdgeMap& vnr_edge_map )
{
    EdgeArray E;
    DoubleArray D;

    // 收集所有已测分支的数据(计算风量-测试风量)
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

// 评价总阻力数据
static double EvalNetworkH( Digraph& dg, VNR_Network_Data& vnr_network_data )
{
    double H = vnr_network_data.H;
    double CH = vnr_network_data.CH;
    return fabs( H - CH );
}

// 评价节点压力数据
static double EvalNodeP( Digraph& dg, VNR_NodeMap& vnr_node_map )
{
    NodeArray U;
    DoubleArray D;

    // 收集所有已测节点的数据(计算压力-测试压力)
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
归一化函数举例：
	1、线性函数转换，表达式如下：
		y=(x-MinValue)/(MaxValue-MinValue)
	说明：x、y分别为转换前、后的值，MaxValue、MinValue分别为样本的最大值和最小值。
	2、对数函数转换，表达式如下
		y=log10(x)
	说明：以10为底的对数函数转换。
	3、反正切函数转换，表达式如下
		y=atan(x)*2/PI
*/
static double Normalize( double x )
{
    return atan( x );
}

FitnessP VNREvalOp::evaluate( IndividualP individual )
{
    // 求最小问题
    FitnessP fitness = static_cast<FitnessP> ( new FitnessMin );

    // 浮点数-二进制编码
    Binary::Binary* gen = ( Binary::Binary* ) individual->at( 0 ).get();

    // 初始默认值为浮点数上限
    fitness->setValue( DBL_MAX );

    // 个体(染色体)转换成风阻值
    GenotypToR( dg, E, gen, vnr_edge_map );

    // 通风网络解算成功并得到数据，记录在vnr_edge_map和vnr_node_map中
    // 适应值计算思路：
    //	1) 将解算风量与测试风量的差值求平方和，最后开平方得到一个dq;
    //	2) 将解算总阻力与测试总阻力差值的绝对值作为一个dh
    //	3) h= r*q^2 ==> dh = 2*r*q ==> dh/h = (2*r*q^2/h)*(dq/q) ==> dh/h = 2dq/q;
    //	   两者比例关系为2:1
    //  4) 0< dq/q < 1，且0 < dh/h < 1
    //	5)
    if( VNR_VNC( dg, vnr_edge_map, vnr_node_map, vnr_network_data ) )
    {
        int n = gen->variables.size(); // 分支个数
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
