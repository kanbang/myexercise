#pragma once

class ContourParamHelper
{
public:
	/*
	 * ����ʵ䲻���ڣ��򴴽��ʵ䣬ͬʱ��ʼ������
	 */
	static void InitParams();

	/*
	 * ��ȡ��ֵ�߲���
	 */
	static void ReadParams(bool& bSmooth, double& textHeight);
	
	/*
	 * д���ֵ�߲���
	 */
	static void WriteParams(bool bSmooth, double textHeight);
};
