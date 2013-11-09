#pragma once

#include "dlimexp.h"
#include "ConstData.h"

/*
 * 数据链接基类
 * 可以关联数据对象(DataObject)、MineGE派生
 *		1) 关联数据对象，可以关联所有的数据，但没有几何图形信息
 *		2) 关联图元(MineGE派生)，在关联数据的同时，也可以进行图形操作
 *			例如删除、着色、切换图层、修改/获取坐标等
 */
class MINEGE_DLLIMPEXP DataLink
{
public:
	virtual ~DataLink(void);

	/*
	 * 设置数据源
	 * 数据源可以是从MineGE派生的图元，数据对象DataObject
	 */
	void setDataSource(const AcDbObjectId& objId);

	/*
	 * 获取数据源
	 */
	AcDbObjectId getDataSource() const;

	/*
	 * 读取或更新数据
	 * 参数：
	 *    save -- false表示将数据更新到变量
	 *         -- true表示将变量更新到数据对象
	 */
	bool updateData(bool save=false);

protected:
	/* 构造函数 */
	DataLink();

	/*
	 * 注册要读取数据的字段 
	 * 派生类必须要重载实现该纯虚函数
	 */
	virtual void regDatas() = 0;

	/* 辅助方法
	 * 提供给派生类调用，用于将不同类型的变量映射到图元的相应字段数据
	 */
	/* 映射字符串变量 */
	void linkStringData(const CString& field, CString* pValue);
	/* 映射整数变量 */
	void linkIntData(const CString& field, int* pValue);
	/* 映射浮点数变量 */
	void linkDoubleData(const CString& field, double* pValue);
	/* 映射布尔变量 */
	void linkBoolData(const CString& field, bool* pValue);
	/* 映射时间日期变量 */
	void linkDateTimeData(const CString& field, COleDateTime* pValue);

private:
	/* 调用regDatas()方法注册字段 */
	void initDatas();
	/* 检查字段是否存在 */
	bool checkFields();
	// resbuf*实质就是一个链表，按照一定的格式保存数据
	// 数据格式：字段1->字段类型->映射的变量指针->
	//            字段2->字段类型->映射的变量指针->...
	resbuf* m_pDatasToFileds;
	
	// 数据源(可以是从MineGE派生的图元，数据对象DataObject)
	AcDbObjectId m_objId;
};
