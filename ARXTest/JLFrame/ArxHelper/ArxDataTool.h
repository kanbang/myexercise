#pragma once

#include "dlimexp.h"

/* 
 * 扩展数据（XData）实质上就是一个链表（List）
 * 它的链表节点可以存放任何类型的数据（基本数据类型），
 * 例如整数、浮点数、字符串等。
 * 
 * 每个扩展数据可以包含很多的appName，这些appName都必须注册到ARX的RegApp Table中
 * 
 */
// 数据辅助工具类
class ARXHELPER_DLLIMPEXP ArxDataTool
{
public:
	/* 1) 扩展数据操作 */

	/* 注册appName到数据库的RegApp Table中 */
	static void RegAppName(AcDbDatabase* db, const CString& appName);
	/* 获取图元扩展数据appName组的第index个数据 */
	static bool GetDataFromXData(AcDbObject* pObj, const CString& appName, int index, CString& value);
	/* 设置图元扩展数据appName组的第index个数据 */
	static bool SetDataToXData(AcDbObject* pObj, const CString& appName, int index, const CString& value);
	/* 向图元扩展数据appName组中增加一个数据 */
	static void AddDataToXData(AcDbObject* pObj, const CString& appName);
	/* 从图元扩展数据appName组中删除第index个数据 */
	static void RemoveDataFromXData(AcDbObject* pObj, const CString& appName, int index);
	/* 向多个图元扩展数据appName组中增加数据 */
	static void AddDataToObjects(const AcDbObjectIdArray& objIds, const CString& appName);
	/* 从多个图元扩展数据appName组中删除第index个数据 */
	static void RemoveDataFromObjects(const AcDbObjectIdArray& objIds, const CString& appName, int index);

	/* 2) 词典操作 */

	/* 判断词典是否存在 */
	static bool IsDictExist(const CString& dictName);	
	/* 添加词典 */
	static void RegDict(const CString& dictName);

	/* 3) 图形操作 */

	/* 判断图元是否与类型type匹配 */
	static bool IsEqualType(const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent=true);
	/* 查找所有指定类型的图元(屏幕上可见的) */
	static void GetEntsByType(const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent=true);
	/* 
	 * 查找所有指定类型的图元(屏幕上不可见的，已被删除的)
	 * 在CAD中图形删除并不会马上收回图形对象占用的内存
	 * CAD只是将图形标记为删除，在关闭图形的时候，
	 * 标记为删除的图形不会保存到dwg文件中
	 */
	static void GetErasedEntsByType(const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent=true);
};
