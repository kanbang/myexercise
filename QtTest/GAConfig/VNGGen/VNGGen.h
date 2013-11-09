#pragma once

#include "dlimexp.h"
#include "GraphInfo.h"

/* 使用lemon库提供的数据结构定义通用的图(或网络) */

/* ListDigraph、Arc、Node等表示通风网络 */
#include <lemon/list_graph.h>
using namespace lemon;

/* 有向图 */
typedef ListDigraph Digraph;

/*
 * Ventilation Network Graph Generation ==> VNGGen
 * 通风网络图生成
 *
 * 输入参数:
 *		dg    --   图(或网络)

 * 输出参数:
 *		gi    --   图的几何信息
 *		nis   --   所有节点的几何信息
 *		eis   --   所有分支的几何信息
 * 注1: 网络分支Digraph::Arc都有一个id==>dg.id(e)
 * 注2: 分支信息EdgeInfo也有一个id ==> ei.id
 * 注3: 两者通过id进行关联
 * 
 * 缺点:
 *   目前采用样条曲线连接成一条分支,样条曲线编辑不太方便
 *	 在实际生成网络图的时候，还需要额外对分支进行处理(近似处理成3点弧)
 *
 */

// 通风网络图绘制参数
struct VNGParam
{
	double m_nodeSep;
	double m_rankSep;
	double m_graphRatio;
	double m_graphWidth;
	double m_graphHeight;
	bool m_useDefWH;

	double m_nodeWidth;
	double m_nodeHeight;

	double m_nodeTextHeight;
	double m_edgeTextHeight;
};

extern "C" VNC_GEN_EXPORT_API bool VNGGen(Digraph& dg, VNGParam& param, GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis);

extern "C" VNC_GEN_EXPORT_API void ClearNodeInfoArray(NodeInfoArray& nis);

extern "C" VNC_GEN_EXPORT_API void ClearEdgeInfoArray(EdgeInfoArray& eis);
