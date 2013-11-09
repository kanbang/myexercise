#pragma once

#undef OUT
#undef IN

#include "../GraphDef.h"

typedef AcArray<Digraph::Arc> EdgeArray;
typedef AcArray<Digraph::Node> NodeArray;
typedef Digraph::ArcMap<AcDbObjectId> ObjectIdMap;

/* 根据指定的图形构建网络 */
extern bool BuildNetwork(const AcDbObjectIdArray& objIds, Digraph& dg, ObjectIdMap& om);
