#pragma once

#include "dlimexp.h"

// 视图管理辅助类
class MINEGE_DLLIMPEXP ViewHelper
{
public:
	static bool AddView(const CString& view);                                   // 新建视图
	static bool RemoveView(const CString& view);                                // 切换视图

	static bool AddElement(const CString& view, const CString& element);        // 添加元素到视图中
	static bool RemoveElement(const CString& view, const CString& element);     // 从视图中删除元素
	static int FindElement(const CString& view, const CString& element);        // 查找视图中元素的索引位置
	static void GetAllElements(const CString& view, AcStringArray& elements);   // 获取视图的所有元素
	static int CountElements(const CString& view);                              // 统计视图的元素个数	

	static void SwitchView(const CString& view);                                // 切换视图
};
