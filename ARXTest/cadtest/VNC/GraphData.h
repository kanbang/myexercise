#pragma once

#include "../GraphDef.h"

#include "NetworkData.h"

// 巷道分支数据
typedef Digraph::ArcMap<VNC_EdgeData*> VNC_EdgeDataMap;
// 巷道上的风机数据
typedef Digraph::ArcMap<VNC_FanData*> VNC_FanDataMap;
