#pragma once

#include "GraphInfo.h"

namespace GraphParser
{
	/* ����dot���ɵ�plain��ʽ�ļ�����ȡͼ����Ϣ */
	extern bool ReadGraphInfoFromPlainFile(const string& outFilePath, GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis);

	extern void ClearNodeInfoArrayImpl(NodeInfoArray& nis);
	extern void ClearEdgeInfoArrayImpl(EdgeInfoArray& eis);
};