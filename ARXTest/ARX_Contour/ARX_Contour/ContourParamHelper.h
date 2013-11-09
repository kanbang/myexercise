#pragma once

class ContourParamHelper
{
public:
	/*
	 * 如果词典不存在，则创建词典，同时初始化参数
	 */
	static void InitParams();

	/*
	 * 读取等值线参数
	 */
	static void ReadParams(bool& bSmooth, double& textHeight);
	
	/*
	 * 写入等值线参数
	 */
	static void WriteParams(bool bSmooth, double textHeight);
};
