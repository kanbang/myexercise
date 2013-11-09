#pragma once

#include "dlimexp.h"

// DataHelper提供了2个静态方法用于读写属性数据
// 它们的实现效率并不高，适用于读取少量字段的情况
// 如果需要连续读取一个图元的大量字段，可以考虑使用DataLink类
class MINEGE_DLLIMPEXP DataHelper
{
public:
	// 获取数据对象(DataObject)
	// 如果objId表示一个MineGE图元，则返回图元关联的数据对象(DataObject)
	static bool GetDataObject(const AcDbObjectId& objId, AcDbObjectId& dObjId);

	// 判断是否数据对象
	static bool IsDataObject(const AcDbObjectId& objId);

	// 获取类型名称
	// 注：objId可以是一个MineGE图元，数据对象(DataObject)
	static bool GetTypeName(const AcDbObjectId& objId, CString& type);

	// 获取属性数据
	// 如果字段不存在，返回false
	// 注：objId可以是一个MineGE图元，数据对象(DataObject)
	static bool GetPropertyData(const AcDbObjectId& objId, const CString& fieldName, CString& value);

	// 修改属性数据
	// 如果字段不存在，返回false
	// 注：objId可以是一个MineGE图元，数据对象(DataObject)
	static bool SetPropertyData(const AcDbObjectId& objId, const CString& fieldName, const CString& value);

	// 复制属性数据
	// 注：objId可以是一个MineGE图元，数据对象(DataObject)
	static bool CopyPropertyData(const AcDbObjectId& sourceObjId, const AcDbObjectId& targetObjId);
};
