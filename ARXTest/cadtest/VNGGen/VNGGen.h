#pragma once

#include "dlimexp.h"
#include "GraphInfo.h"
#include "../GraphDef.h"

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
 * 目前采用样条曲线连接成一条分支
 * 样条曲线编辑不太方便，而分支应该是一个图元，用多条直线表示分支不可行
 *
 * 下一步的工作:
 *     1) 尝试根据已有的点数据，用圆弧绘制分支(未实现???)
 */

extern "C" VNC_GEN_EXPORT_API bool VNGGen(Digraph& dg, 
										  double m_nodeSep, double m_rankSep,
										  double m_graphRatio, double m_graphWidth, double m_graphHeight, bool m_useDefWH,
										  double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight, double m_edgeTextHeight, 
										  GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis);


extern "C" VNC_GEN_EXPORT_API void ClearNodeInfoArray(NodeInfoArray& nis);
extern "C" VNC_GEN_EXPORT_API void ClearEdgeInfoArray(EdgeInfoArray& eis);
