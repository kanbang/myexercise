#pragma once

#include "dlimexp.h"

// ARX工具类
class ARXHELPER_DLLIMPEXP ArxUtilHelper
{
public:
	// 将新建的图元添加到CAD图形数据库
	static bool PostToModelSpace(AcDbEntity* pEnt);

	// 命令交互选择一个实体
	// 参数：msg表示在cad命令行显示的提示消息
	static AcDbObjectId SelectObject(const CString& msg);

	// 获取当前已选择的图元(PickSet)
	static void GetPickSetEntity(AcDbObjectIdArray& objIds);

	// 交互选择一个点坐标
	// 参数：
	//		msg -- 表示在cad命令行显示的提示消息
	//      pt  --  返回用户选择的点坐标
	static bool PromptPt(const CString& msg, AcGePoint3d& pt);

	// 交互选择一个点坐标
	// 参数：
	//		msg    -- 在cad命令行显示的提示消息
	//		basePt -- 基点，用户移动鼠标时，
	//                自动绘制一条从basePt到鼠标当前点的橡皮线
	//      pt     -- 返回用户选择的点坐标
	static bool PromptPt2(const CString& msg, const AcGePoint3d& basePt, AcGePoint3d& pt);

	// 计算向量v与x轴逆时针旋转角度
	static double AngleToXAxis(const AcGeVector3d& v);

	// 暂停
	// CAD命令行会输出提示信息："请按回车或空格键结束..."
	static void Pause();

	// 是否指定类型的图元
	// 注：与ArxDataTool::IsEqualType()方法在功能上是一样的
	//      参见ArxDataTool.h
	static bool IsEqualType(const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent=true);

	/* 以下4个方法暂时没有实际的用途 */
	// 临时用一种颜色显示一个图元
	// 会话结束后，恢复原有颜色
	static void ShowEntityWithColor(const AcDbObjectId& objId, unsigned short colorIndex);
	// 临时用一种颜色显示多个图元
	// 会话结束后，恢复原有颜色
	static void ShowEntitiesWithColor(AcDbObjectIdArray& objIds, unsigned short colorIndex);
	// 临时用多种颜色显示多个图元
	// 会话结束后，恢复原有颜色
	static void ShowEntitiesWithColor2(AcDbObjectIdArray& objIds, const AcArray<Adesk::UInt16>& colors);
	// 命令行交互遍历多个图元
	static void BrowserEntities(const AcDbObjectIdArray& objIds);

	// 调整角度
	// 返回结果：正值，[0, 2*PI]
	// 可以输入任意大小的角度(不论正负，输入值可以超出[0, 2*PI]区间范围)
	static double AdjustAngle(double angle);

	/* string<-->int, double之间的转换 */

	// 字符串转成浮点数
	static bool StringToDouble(const CString& value, double& v);
	// 字符串转成整数
	static bool StringToInt(const CString& value, int& v);
	// 浮点数转换成字符串
	static void DoubleToString(double v, CString& value);
	// 整数转换成字符串
	static void IntToString(int v, CString& value);
	
	// 判断字符串是否是一个合法的整数串
	// 格式[+/-][0-9]
	static bool IsInteger(LPCTSTR pSrc);
	
	// 判断字符串是否是一个合法的浮点数串
	// 格式[+/-][0-9][.][0-9]	
	static bool IsNumeric(LPCTSTR pSrc);

	// 生成GUID字符串
	// GUID在空间上和时间上具有唯一性，保证同一时间不同地方产生的数字不同。 
	// 世界上的任何两台计算机都不会生成重复的 GUID 值
	// 例如：{9E3549BB-9144-42ea-8045-83BC250A2E8C}
	static bool NewGUID(CString& strGUID);
};
