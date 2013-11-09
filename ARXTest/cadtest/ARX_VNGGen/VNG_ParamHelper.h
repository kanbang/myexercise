#pragma once

class VNG_ParamHelper
{
public:
	/*
	 * 如果词典不存在，则创建词典，同时初始化参数
	 */
	static void InitParams();

	/*
	 * 读取通风网络绘制参数
	 */
	static void ReadParams(double& m_nodeSep, double& m_rankSep, 
		                   double& m_graphRatio, double& m_graphWidth, double& m_graphHeight, bool& m_useDefWH,
						   double& m_nodeWidth, double& m_nodeHeight, double& m_nodeTextHeight,
                           double& m_arrowWidth, double& m_arrowLength, double& m_edgeTextHeight, bool& m_needEdge);
	
	/*
	 * 写入通风网络绘制参数
	 */
	static void WriteParams(double m_nodeSep, double m_rankSep, 
                            double m_graphRatio, double m_graphWidth, double m_graphHeight, bool m_useDefWH,
							double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight,
		                    double m_arrowWidth, double m_arrowLength, double m_edgeTextHeight, bool m_needEdge);
};
