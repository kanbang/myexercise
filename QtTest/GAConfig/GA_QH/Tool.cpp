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

    // 构造分支数据和风机数据
    // 目前不考虑固定风量
    VNC_EdgeDataMap ed( dg, 0 );
    VNC_FanDataMap fd( dg, 0 );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNC_EdgeData* pData = new VNC_EdgeData;
        pData->r = vnr_edge_map[e].cr; // 使用cr
        ed[e] = pData;
    }

    // 进行网络解算
    if( !VNC( dg, ed, fd, vnr_network_data.Q ) ) return false;

    // 如果需要，还应该计算所有网络节点的压力和总压力
    // 后续实现...
    vnr_network_data.CH = VNC_H( dg, ed, fd );

    // 将计算得到的风量和节点压力分配到vnr_edge_map和vnr_node_map中
    // 目前只考虑风量，暂时不考虑节点压力
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        VNC_EdgeData* pData = ed[e];
        vnr_edge_map[e].cq = pData->q; // 将解算风量赋给vnr_edge_map中的cq
    }

    // 清除内存
    ClearAllVNCDatas( dg, ed, fd );

    return true;
}
