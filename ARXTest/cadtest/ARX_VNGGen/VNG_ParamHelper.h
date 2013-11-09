#pragma once

class VNG_ParamHelper
{
public:
	/*
	 * ����ʵ䲻���ڣ��򴴽��ʵ䣬ͬʱ��ʼ������
	 */
	static void InitParams();

	/*
	 * ��ȡͨ��������Ʋ���
	 */
	static void ReadParams(double& m_nodeSep, double& m_rankSep, 
		                   double& m_graphRatio, double& m_graphWidth, double& m_graphHeight, bool& m_useDefWH,
						   double& m_nodeWidth, double& m_nodeHeight, double& m_nodeTextHeight,
                           double& m_arrowWidth, double& m_arrowLength, double& m_edgeTextHeight, bool& m_needEdge);
	
	/*
	 * д��ͨ��������Ʋ���
	 */
	static void WriteParams(double m_nodeSep, double m_rankSep, 
                            double m_graphRatio, double m_graphWidth, double m_graphHeight, bool m_useDefWH,
							double m_nodeWidth, double m_nodeHeight, double m_nodeTextHeight,
		                    double m_arrowWidth, double m_arrowLength, double m_edgeTextHeight, bool m_needEdge);
};
