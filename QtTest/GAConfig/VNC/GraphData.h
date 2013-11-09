#pragma once

#include "NetworkData.h"

/* 使用lemon库提供的数据结构定义通用的图(或网络) */

/* ListDigraph、Arc、Node等表示通风网络 */
#include <lemon/list_graph.h>
using namespace lemon;

/* 有向图 */
typedef ListDigraph Digraph;

// 巷道分支数据
typedef Digraph::ArcMap<VNC_EdgeData*> VNC_EdgeDataMap;
// 巷道上的风机数据
typedef Digraph::ArcMap<VNC_FanData*> VNC_FanDataMap;
