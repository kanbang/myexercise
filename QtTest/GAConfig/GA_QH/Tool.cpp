#include "Tool.h"
#include <ECF.h>

#include "../VNC/VNC.h"

void GenotypToR( Digraph& dg, EdgeArray& E, Binary::Binary* gen, VNR_EdgeMap& vnr_edge_map )
{
    assert( countArcs( dg ) == gen->variables.size() );
    assert( E.size() == gen->variables.size() );

    for( uint i = 0; i < E.size(); i++ )
    {
        Digraph::Arc e = E[i];
        vnr_edge_map[e].cr = gen->realValue.at( i );

        if( vnr_edge_map[e].bInitR )
        {
            vnr_edge_map[e].cr = vnr_edge_map[e].r;
        }
    }
}

bool VNR_VNC( Digraph& dg,
              VNR_EdgeMap& vnr_edge_map,
              VNR_NodeMap& vnr_node_map,
              VNR_Network_Data& vnr_network_data )
{

    // �����֧���ݺͷ������
    // Ŀǰ�����ǹ̶�����
    VNC_EdgeDataMap ed( dg, 0 );
    VNC_FanDataMap fd( dg, 0 );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNC_EdgeData* pData = new VNC_EdgeData;
        pData->r = vnr_edge_map[e].cr; // ʹ��cr
        ed[e] = pData;
    }

    // �����������
    if( !VNC( dg, ed, fd, vnr_network_data.Q ) ) return false;

    // �����Ҫ����Ӧ�ü�����������ڵ��ѹ������ѹ��
    // ����ʵ��...
    vnr_network_data.CH = VNC_H( dg, ed, fd );

    // ������õ��ķ����ͽڵ�ѹ�����䵽vnr_edge_map��vnr_node_map��
    // Ŀǰֻ���Ƿ�������ʱ�����ǽڵ�ѹ��
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNC_EdgeData* pData = ed[e];
        vnr_edge_map[e].cq = pData->q; // �������������vnr_edge_map�е�cq
    }

    // ����ڴ�
    ClearAllVNCDatas( dg, ed, fd );

    return true;
}
