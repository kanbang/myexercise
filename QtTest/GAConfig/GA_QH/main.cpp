#include <ECF.h>
#include "VNREvalOp.h"
#include "BuildNetwork.h"
#include "Tool.h"

#include <iostream>
#include <iomanip>
using namespace std;

static void PrintVN( Digraph& dg, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data )
{
    cout << "初始风阻\t初始风量\t解算风阻\t解算风量\t解算阻力" << endl;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int id = vnr_edge_map[e].id;
        double r = vnr_edge_map[e].r;
        double cr = vnr_edge_map[e].cr;
        double q = vnr_edge_map[e].q;
        double cq = vnr_edge_map[e].cq;
        cout << setprecision( 4 ) << fixed
             << "r0(e" << id << ")=" << r << "\t"
             << "q0(e" << id << ")=" << q << "\t"
             << "r(e" << id << ")=" << cr << "\t"
             << "q(e" << id << ")=" << cq << "\t"
             << "h(e" << id << ")=" << cr* cq* cq << endl;
    }
    cout << "已知风阻的分支: ";
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int id = vnr_edge_map[e].id;
        if( vnr_edge_map[e].bInitR )
        {
            cout << "e" << id << "\t";
        }
    }
    cout << "\n";
    cout << "总风量：" << vnr_network_data.Q << "\n"
         << "初始总阻力：" << vnr_network_data.H << "\t"
         << "解算总阻力：" << vnr_network_data.CH << endl;
}

static bool InitNetwork( Digraph& dg, EdgeArray& E, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data )
{
    // 读取数据文件，构建通风网络，初始化数据
    if( !BuildNetwork( "data4.txt", "vnr_data4.txt", dg, vnr_edge_map, vnr_node_map, vnr_network_data ) ) return false;

    // 将所有分支排列成一个数组
    // 与个体(染色体)的顺序一致
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        E.push_back( e );
    }

    // 将初始的风阻r复制到cr中
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        vnr_edge_map[e].cr = vnr_edge_map[e].r;
    }

    cout << "根据假定的风阻进行一次通风网络解算" << endl;
    bool ret = VNR_VNC( dg, vnr_edge_map, vnr_node_map, vnr_network_data );
    if( ret )
    {
        cout << "解算成功" << endl;
        // 复制总阻力
        vnr_network_data.H = vnr_network_data.CH;

        // 将解算的风量cq复制到q中
        for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
        {
            vnr_edge_map[e].q = vnr_edge_map[e].cq;
        }
    }
    else
    {
        cout << "解算失败" << endl;
    }

    return ret;
}

static EvaluateOp* CreateEvalOp( Digraph& dg, EdgeArray& E, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data )
{
    return new VNREvalOp( dg, E, vnr_network_data, vnr_edge_map, vnr_node_map );
}

static Binary::Binary* RunGA( int argc, char** argv, EvaluateOp* pEvalOp )
{
    StateP state ( new State );

    // 设置评价函数
    state->setEvalOp( pEvalOp );

    // 初始化遗传参数
    state->initialize( argc, argv );

    Binary::Binary* pBest = 0;

    // 执行遗传算法过程
    if( state->run() )
    {
        // 获取最优个体
        IndividualP ind = state->getHoF()->getBest().at( 0 );
        Binary::Binary* gen = ( Binary::Binary* )ind->at( 0 ).get();

        pBest = gen->copy();
    }

    return pBest;
}

int main( int argc, char** argv )
{
    // 1) 通风网络
    Digraph dg;
    // 2) 网络数据--总风量、总阻力
    VNR_Network_Data vnr_network_data;
    // 3) 分支数据--编号、风阻、计算风量、测试风量
    VNR_EdgeMap vnr_edge_map( dg );
    // 4) 节点数据--编号、计算节点压力、测试节点压力
    VNR_NodeMap vnr_node_map( dg );
    // 5) 与编码串对应的分支排列顺序
    EdgeArray E;

    // 初始化网络，并执行一次网络解算
    if( !InitNetwork( dg, E, vnr_edge_map, vnr_node_map, vnr_network_data ) ) return 0;

    // 执行遗传算法，并返回最优个体
    Binary::Binary* pBest = RunGA( argc, argv,
                                   CreateEvalOp( dg, E, vnr_edge_map, vnr_node_map, vnr_network_data ) );

    if( pBest != 0 )
    {
        cout << "遗传算法计算成功" << endl;

        // 将个体(染色体)转换成风阻值
        GenotypToR( dg, E, pBest, vnr_edge_map );
        delete pBest;

        cout << "反算最优个体" << endl;
        if( VNR_VNC( dg, vnr_edge_map, vnr_node_map, vnr_network_data ) )
        {
            PrintVN( dg, vnr_edge_map, vnr_node_map, vnr_network_data );
        }
    }
    return 0;
}
