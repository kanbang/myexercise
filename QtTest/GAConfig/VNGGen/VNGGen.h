#pragma once

#include "dlimexp.h"
#include "GraphInfo.h"

/* ʹ��lemon���ṩ�����ݽṹ����ͨ�õ�ͼ(������) */

/* ListDigraph��Arc��Node�ȱ�ʾͨ������ */
#include <lemon/list_graph.h>
using namespace lemon;

/* ����ͼ */
typedef ListDigraph Digraph;

/*
 * Ventilation Network Graph Generation ==> VNGGen
 * ͨ������ͼ����
 *
 * �������:
 *		dg    --   ͼ(������)

 * �������:
 *		gi    --   ͼ�ļ�����Ϣ
 *		nis   --   ���нڵ�ļ�����Ϣ
 *		eis   --   ���з�֧�ļ�����Ϣ
 * ע1: �����֧Digraph::Arc����һ��id==>dg.id(e)
 * ע2: ��֧��ϢEdgeInfoҲ��һ��id ==> ei.id
 * ע3: ����ͨ��id���й���
 * 
 * ȱ��:
 *   Ŀǰ���������������ӳ�һ����֧,�������߱༭��̫����
 *	 ��ʵ����������ͼ��ʱ�򣬻���Ҫ����Է�֧���д���(���ƴ����3�㻡)
 *
 */

// ͨ������ͼ���Ʋ���
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
