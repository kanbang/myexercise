#pragma once

#include "DataObject.h"

// 数据操纵辅助类(MineGE模块内部使用)
// 内部操作直接使用数据对象指针，连续操作大量数据效率较好
class DataHelperImpl
{
public:
	// 构造函数，传入数据对象指针作为参数
	DataHelperImpl(DataObject* pObj);

	// 获取属性数据
	// 如果字段不存在，返回false
	bool getPropertyData(const CString& fieldName, CString& value);

	// 修改属性数据
	// 如果字段不存在，返回false
	bool setPropertyData(const CString& fieldName, const CString& value);

private:
	DataObject* m_pObj;
};