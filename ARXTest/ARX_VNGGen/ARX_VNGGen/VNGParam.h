#pragma once

// 图形参数
struct GraphParam
{
	double m_nodeSep;
	double m_rankSep;
	double m_graphRatio;
	double m_graphWidth;
	double m_graphHeight;
	bool m_useDefWH;
};

// 节点参数(同时也是图形参数)
struct NodeParam
{
	double m_nodeWidth;
	double m_nodeHeight;
	double m_nodeTextHeight;
};

// 分支参数
struct EdgeParam
{
	double m_arrowWidth;
	double m_arrowLength;
	bool m_needEdge;
	double m_edgeTextHeight; // 该参数也是图形参数
};

// 风门参数
struct GateParam
{
	double m_radius;
	double m_length;
};

// 风机参数
struct FanParam
{
	double m_radius;
	double m_distance;
};