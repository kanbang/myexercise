#pragma once

#include "BuildNetwork.h"
#include "NetworkDataLink.h"

typedef Digraph::ArcMap<EdgeDataLink*> NetworkEdgeDataMap;

typedef AcArray<short> EdgeDirCoeffArray;  // 回路中的分支方向系数
