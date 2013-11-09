#pragma once

// 一些临时测试，后续可能需要删除
class SomeTest
{
public:
	/* 测试数据操作 */
	static void TestData(void);
	static void TestData2(void);
	static void PrintData(void);
	static void ChangeData(void);

	/* 测试图层 */
	static void AddLayerTest(void);
	static void ChangeLayerTest(void);

	/* 打印图元类型 */
	static void DisplayAllGETypes(void);
	static void DisplayAllGETypes2(void);

	/* 可视化效果配置测试 */
	static void ConfigDrawTest(void);

	/* 数据导入和导出测试 */
	static void DoExportData();
	static void DoImportData();
};
