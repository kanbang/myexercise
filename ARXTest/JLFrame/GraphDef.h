#pragma once

/* ʹ��lemon���ṩ�����ݽṹ����ͨ�õ�ͼ(������) */

/* ListDigraph��Arc��Node�ȱ�ʾͨ������ */
#include <lemon/list_graph.h>

using namespace lemon;

/* ����ͼ */
typedef ListDigraph Digraph;

// ����lib
#ifdef _DEBUG
	#pragma comment(lib, "lemond.lib")
#else
	#pragma comment(lib, "lemon.lib")
#endif