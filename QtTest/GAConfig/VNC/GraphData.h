#pragma once

#include "NetworkData.h"

/* ʹ��lemon���ṩ�����ݽṹ����ͨ�õ�ͼ(������) */

/* ListDigraph��Arc��Node�ȱ�ʾͨ������ */
#include <lemon/list_graph.h>
using namespace lemon;

/* ����ͼ */
typedef ListDigraph Digraph;

// �����֧����
typedef Digraph::ArcMap<VNC_EdgeData*> VNC_EdgeDataMap;
// ����ϵķ������
typedef Digraph::ArcMap<VNC_FanData*> VNC_FanDataMap;
