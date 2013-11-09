#include <ECF.h>
#include "VNREvalOp.h"
#include "BuildNetwork.h"
#include "Tool.h"

#include <iostream>
#include <iomanip>
using namespace std;

static void PrintVN( Digraph& dg, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data )
{
    cout << "��ʼ����\t��ʼ����\t�������\t�������\t��������" << endl;
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
    cout << "��֪����ķ�֧: ";
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        int id = vnr_edge_map[e].id;
        if( vnr_edge_map[e].bInitR )
        {
            cout << "e" << id << "\t";
        }
    }
    cout << "\n";
    cout << "�ܷ�����" << vnr_network_data.Q << "\n"
         << "��ʼ��������" << vnr_network_data.H << "\t"
         << "������������" << vnr_network_data.CH << endl;
}

static bool InitNetwork( Digraph& dg, EdgeArray& E, VNR_EdgeMap& vnr_edge_map, VNR_NodeMap& vnr_node_map, VNR_Network_Data& vnr_network_data )
{
    // ��ȡ�����ļ�������ͨ�����磬��ʼ������
    if( !BuildNetwork( "data4.txt", "vnr_data4.txt", dg, vnr_edge_map, vnr_node_map, vnr_network_data ) ) return false;

    // �����з�֧���г�һ������
    // �����(Ⱦɫ��)��˳��һ��
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        E.push_back( e );
    }

    // ����ʼ�ķ���r���Ƶ�cr��
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        vnr_edge_map[e].cr = vnr_edge_map[e].r;
    }

    cout << "���ݼٶ��ķ������һ��ͨ���������" << endl;
    bool ret = VNR_VNC( dg, vnr_edge_map, vnr_node_map, vnr_network_data );
    if( ret )
    {
        cout << "����ɹ�" << endl;
        // ����������
        vnr_network_data.H = vnr_network_data.CH;

        // ������ķ���cq���Ƶ�q��
        for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
        {
            vnr_edge_map[e].q = vnr_edge_map[e].cq;
        }
    }
    else
    {
        cout << "����ʧ��" << endl;
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

    // �������ۺ���
    state->setEvalOp( pEvalOp );

    // ��ʼ���Ŵ�����
    state->initialize( argc, argv );

    Binary::Binary* pBest = 0;

    // ִ���Ŵ��㷨����
    if( state->run() )
    {
        // ��ȡ���Ÿ���
        IndividualP ind = state->getHoF()->getBest().at( 0 );
        Binary::Binary* gen = ( Binary::Binary* )ind->at( 0 ).get();

        pBest = gen->copy();
    }

    return pBest;
}

int main( int argc, char** argv )
{
    // 1) ͨ������
    Digraph dg;
    // 2) ��������--�ܷ�����������
    VNR_Network_Data vnr_network_data;
    // 3) ��֧����--��š����衢������������Է���
    VNR_EdgeMap vnr_edge_map( dg );
    // 4) �ڵ�����--��š�����ڵ�ѹ�������Խڵ�ѹ��
    VNR_NodeMap vnr_node_map( dg );
    // 5) ����봮��Ӧ�ķ�֧����˳��
    EdgeArray E;

    // ��ʼ�����磬��ִ��һ���������
    if( !InitNetwork( dg, E, vnr_edge_map, vnr_node_map, vnr_network_data ) ) return 0;

    // ִ���Ŵ��㷨�����������Ÿ���
    Binary::Binary* pBest = RunGA( argc, argv,
                                   CreateEvalOp( dg, E, vnr_edge_map, vnr_node_map, vnr_network_data ) );

    if( pBest != 0 )
    {
        cout << "�Ŵ��㷨����ɹ�" << endl;

        // ������(Ⱦɫ��)ת���ɷ���ֵ
        GenotypToR( dg, E, pBest, vnr_edge_map );
        delete pBest;

        cout << "�������Ÿ���" << endl;
        if( VNR_VNC( dg, vnr_edge_map, vnr_node_map, vnr_network_data ) )
        {
            PrintVN( dg, vnr_edge_map, vnr_node_map, vnr_network_data );
        }
    }
    return 0;
}
