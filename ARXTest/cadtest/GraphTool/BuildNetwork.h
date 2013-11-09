#pragma once

#include "dlimexp.h"

#undef OUT
#undef IN

#include "../GraphDef.h"

typedef AcArray<Digraph::Arc> EdgeArray;
typedef AcArray<Digraph::Node> NodeArray;

typedef Digraph::ArcMap<bool> ArcFilter;
typedef Digraph::NodeMap<bool> NodeFilter;

// ��֧��Ӧ��ʵ�����id
typedef Digraph::ArcMap<AcDbObjectId> ObjectIdMap;

// �ڵ�����
typedef Digraph::NodeMap<AcGePoint3d> NodeGeoDataMap;

/*
 *	��������
 *  ͬʱ���⴦����������(���������֧)
 */
extern "C" GRAPH_EXPORT_API bool BuildNetwork(Digraph& dg, ObjectIdMap& om);

/*
 * �����Դ���
 * ���һ������Դ���������
 * ������Դ���Դ��֮����������֧
 * ���Ҳ��ͬ������
 * �����֧��idΪAcDbObjectId::kNull
 */
extern "C" GRAPH_EXPORT_API bool AddVirtualSTNode(Digraph& dg, ObjectIdMap& om, Digraph::Node& sn, Digraph::Node& tn);

/*
 * ���˰�����Ϲ�����ķ�֧
 * block filter
 */
extern "C" GRAPH_EXPORT_API void FilterBlockEdges(Digraph& dg, ObjectIdMap& om, ArcFilter& bf);

/*
 * ɾ��������Ϲ�����ķ�֧
 * ע��ɾ����Ϸ�֧�����ܻ��������Դ�����
 */
extern "C" GRAPH_EXPORT_API void EraseBlockEdges(Digraph& dg, ArcFilter& bf);

/* 
 * ��ȡ�ڵ�ͷ�֧�ļ�������
 * ע��û�д��������֧
 *     ���������DealWithSourceTarget������
 *     ��ô����Դ��ͻ�������Ĭ��Ϊԭ��(0,0,0)
 */
extern "C" GRAPH_EXPORT_API bool GetNodeGeoData(Digraph& dg, ObjectIdMap& om, NodeGeoDataMap& ngm);