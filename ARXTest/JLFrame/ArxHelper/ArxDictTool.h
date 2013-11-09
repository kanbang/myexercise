#pragma once

#include "dlimexp.h"

// 无效的entry索引
#define INVALID_ENTRY 0

/*
 * CAD词典管理辅助工具类
 * AcDbDictionay的key不是大小写敏感的
 * 例如NAME, name, Name, naMe等都是一样的
 * 
 * 一个key->AcDbXRecord(记录集，包含很多entry项)
 * AcDbXRecord实质是一个resbuf list, 可以保存一串数据，
 * 这里使用AcDbXRecord保存一些简单的文本串数据(可以只包含1个entry，也可以包含多个entry)
 *
 * 示例：
 * 使用词典保存字段
 *	   Tunnel       -->  (名称 风量 风阻  阻力 局部风阻 摩擦系数 ...)
 *	   Fan          -->  (名称 风机运行状态 a0 a1 a2 a3 a4 ...)
 *	   Gate         -->  (名称 漏风量 压差 ...)
 */

class ARXHELPER_DLLIMPEXP ArxDictTool
{
public:
	/* 简单的构造方法，传入词典的名称，创建一个ArxDictTool对象指针 
	 * 前提：该词典存在于数据库中，如果不存在，应该先使用
	 * ArxDataTool::RegDict()方法注册词典
	 *
	 * 该方法与直接使用new是一样的
	 * ArxDictTool* pDictTool = ArxDictTool::GetDictTool(_T("test"));
	 * 等价于
	 * ArxDictTool* pDictTool = new ArxDictTool(_T("test"));
	 */
	static ArxDictTool* GetDictTool(const CString& dictName);

	/* 判断key是否存在 */
	bool findKey(const CString& key);
	/* 删除key以及key对应的所有的数据 */
	void removeKey(const CString& key);
	/* 获取词典中的所有key */
	void getAllKeys(AcStringArray& keys);

	/* 添加entry,如果key不存在，则自动添加 */
	int addEntry(const CString& key, const CString& entry);
	/* 删除entry */
	int removeEntry(const CString& key, const CString& entry);
	/* 查找entry的索引位置 */
	int findEntry(const CString& key, const CString& entry);
	/* 获取key包含的的所有entry */
	void getAllEntries(const CString& key, AcStringArray& entries);
	/* 统计key包含的所有entry个数 */
	int countEntries(const CString& key);
	
	/* 修改key中第index项的值
	 * 注：index必须要大于0
	 */
	bool modifyEntry(const CString& key, int index, const CString& newEntry);
	/* 获取key中第index项的值
	 * 注：index必须要大于0
	 */
	bool getEntry(const CString& key, int index, CString& entry);

private:
	ArxDictTool(const CString& dictName);
	/* 要操作的词典名称 */
	CString m_dictName;
};

/* CAD词典管理工具
 * 一个key->AcDbObject(或者从AcDbObject直接派生，但不包含AcDbEntity图形实体基类)
 */
class ARXHELPER_DLLIMPEXP ArxDictTool2
{
public:
	static ArxDictTool2* GetDictTool(const CString& dictName);

	// 添加到词典中，同时自动关闭对象pObj->close();
	bool addEntry(const CString& key, AcDbObject* pObj);         // 添加
	bool removeEntry(const CString& key);                        // 删除
	bool findEntry(const CString& key, AcDbObjectId& objId);     // 查找
	void getAllKeys(AcStringArray& keys);
	void getAllEntries(AcDbObjectIdArray& objIds);               // 获取所有的对象

private:
	ArxDictTool2(const CString& dictName);
	CString m_dictName;
};

class ARXHELPER_DLLIMPEXP ArxDictHelper
{
public:
	static void GetAllKeys(const CString& dictName, AcStringArray& keys);
	static bool GetAllEntries(const CString& dictName, const CString& key, AcStringArray& entries);
	static bool AddEntry(const CString& dictName, const CString& key, const CString& entry);
	static void RemoveAllEntries(const CString& dictName, const CString& key);
	static bool RemoveEntry(const CString& dictName, const CString& key, const CString& entry);
};