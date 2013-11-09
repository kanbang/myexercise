#pragma once

#include "GraphInfo.h"

namespace GraphParser
{
	/* 解析dot生成的plain格式文件，读取图形信息 */
	extern bool ReadGraphInfoFromPlainFile(const string& outFilePath, GraphInfo& gi, NodeInfoArray& nis, EdgeInfoArray& eis);

	extern void ClearNodeInfoArrayImpl(NodeInfoArray& nis);
	extern void ClearEdgeInfoArrayImpl(EdgeInfoArray& eis);
};