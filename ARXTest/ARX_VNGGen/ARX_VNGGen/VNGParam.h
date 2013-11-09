#pragma once

// ͼ�β���
struct GraphParam
{
	double m_nodeSep;
	double m_rankSep;
	double m_graphRatio;
	double m_graphWidth;
	double m_graphHeight;
	bool m_useDefWH;
};

// �ڵ����(ͬʱҲ��ͼ�β���)
struct NodeParam
{
	double m_nodeWidth;
	double m_nodeHeight;
	double m_nodeTextHeight;
};

// ��֧����
struct EdgeParam
{
	double m_arrowWidth;
	double m_arrowLength;
	bool m_needEdge;
	double m_edgeTextHeight; // �ò���Ҳ��ͼ�β���
};

// ���Ų���
struct GateParam
{
	double m_radius;
	double m_length;
};

// �������
struct FanParam
{
	double m_radius;
	double m_distance;
};