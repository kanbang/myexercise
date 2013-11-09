#pragma once

#include "dlimexp.h"

// 通风网络算法测试类
class VNALGO_EXPORT_API VNAlgoTest
{
public:
	static void FIPTest();
	static void SWDTest();
	static void ILTTest2();
	static void ILTTest3();	
	static void FWSTest();

	static void IntFlowTest();
	static void DFSTest();

	static void IsIDSTest();
	static void DoIDSTest(bool findNST);
};
