#pragma once

#include "dlimexp.h"

// 标签操作辅助类
// 可以给任何图元实体(从AcDbObject派生)添加标签
class MINEGE_DLLIMPEXP LabelHelper
{
public:
	// 添加一个标签
	static void AddLabel(const AcDbObjectId& objId, const CString& label);

	// 添加多个标签
	static void AddMoreLabels(const AcDbObjectId& objId, const AcStringArray& labels);

	// 删除一个标签
	static void RemoveLabel(const AcDbObjectId& objId, const CString& label);
	
	// 删除多个标签
	static void RemoveMoreLabels(const AcDbObjectId& objId, const AcStringArray& labels);

	// 删除所有标签
	static void RemoveAllLabels(const AcDbObjectId& objId);

	// 获取所有的标签
	static void GetAllLabels(const AcDbObjectId& objId, AcStringArray& labels);

	// 是否具有标签
	static bool HasLabel(const AcDbObjectId& objId, const CString& label);
};
