#pragma once

// Object Record Pool ==> ORP
// 对象记录池
class ORPHelper
{
public:
	// 创建和销毁
	static void CreateORP();
	static void DeleteORP();

	// 设置开关状态
	static void SetORPState(bool flag);
	// 获取开关状态
	static bool getORPState();

	// 可用个数
	static int GetCount();
	// 清空
	static void ClearAll();
	
	// 添加
	static bool AddObject(const AcDbObjectId& objId);
	// 提取
	static bool GetObject(AcDbObjectId& objId);
};
