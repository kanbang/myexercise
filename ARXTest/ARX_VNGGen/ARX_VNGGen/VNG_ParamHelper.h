#pragma once

#include "VNGParam.h"

class VNG_ParamHelper
{
public:
	/*
	 * 如果词典不存在，则创建词典，同时初始化参数
	 */
	static void InitParams();

	static void ReadGraphParams(GraphParam& gp);
	static void ReadNodeParams(NodeParam& np);
	static void ReadEdgeParams(EdgeParam& ep);
	static void ReadGateParams(GateParam& gp);
	static void ReadFanParams(FanParam& fp);

	static void WriteGraphParams(const GraphParam& gp);
	static void WriteNodeParams(const NodeParam& np);
	static void WriteEdgeParams(const EdgeParam& ep);
	static void WriteGateParams(const GateParam& gp);
	static void WriteFanParams(const FanParam& fp);
};
