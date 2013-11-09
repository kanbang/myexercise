#pragma once

#undef OUT
#undef IN

#include "../GraphDef.h"

typedef AcArray<Digraph::Arc> EdgeArray;
typedef AcArray<Digraph::Node> NodeArray;
typedef Digraph::ArcMap<AcDbObjectId> ObjectIdMap;

/* ����ָ����ͼ�ι������� */
extern bool BuildNetwork(const AcDbObjectIdArray& objIds, Digraph& dg, ObjectIdMap& om);
