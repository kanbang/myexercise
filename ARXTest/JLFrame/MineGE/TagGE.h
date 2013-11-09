#pragma once

#include "MineGE.h"

/*
 * 标签类图元
 * 例如: 风流方向、风门、风机
 * 这类图元必须要要关联一个宿主图元(host)，它们是无法独立存在的
 * 注：被关联的宿主图元(Host)可以有很多的"标签图元"
 *
 * 注：MINEGE_DLLIMPEXP宏表示DLL导入/导出指令
 */
class MINEGE_DLLIMPEXP TagGE : public MineGE 
{
	/* arx向导生成的代码 */
	/* 请参见MineGE类的说明 */
public:
	ACRX_DECLARE_MEMBERS(TagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

	/* 手动添加的代码 */
public:
	/* 获取关联的宿主图元(host) */
	AcDbObjectId getHost() const;

	/*
	 * 设置关联的宿主图元(host)
	 * 注：只能关联MineGE派生的图元
	 */
	void setHost(const AcDbObjectId& objId);

	/* arx向导生成的代码 */
public:
	/* 
	 * 和MFC的序列化类似，CAD调用dwgOutFields()方法将图元的数据保存到DWG文件中
	 * 具体来说，保存TagGE图元的"宿主图元ID"(m_objId)到DWG文件中
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * 和MFC的序列化类似，CAD调用dwgInFields()方法从DWG文件中读取数据到图元中
	 * 具体来说，从DWG文件中读取ID到TagGE图元的"宿主图元"(m_objId)
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* 通过arx工具栏添加的代码 */
	/* 
	 * TagGE图元是依赖于宿主图元(host)存在的
	 * 如果宿主图元不存在(m_objId.isNull()==true)，
	 * 那么TagGE图元也不应该存在图形效果(绘制图形)
	 * 
	 * 注：以下sub开头的方法都是由CAD内部调用(AcDbObject->AcDbEntity->MineGE)
	 */
protected:
	/* 
	 * 如果宿主图元不存在(m_objId.isNull()==true)，
	 * 那么TagGE图元不存在图形效果(绘制图形)
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

	/* 
	 * 构造函数
	 * 注：根据ARX技术要求，所有的自定义类都必须提供一个默认构造函数(无参构造函数)
	 *      默认构造函数的访问权限可以是public，也可以是protected
	 */
protected:
	/* 默认构造函数 */
	TagGE();

private:
	/* 标签图元关联的宿主图元(host) */
	AcDbObjectId m_objId;
};

/*
 * 方向标签类图元，例如风流方向、风机、风门
 * 
 * 主要几何特征：1)插入点坐标 2)方向角度
 *
 * 注：MINEGE_DLLIMPEXP宏表示DLL导入/导出指令
 */
class MINEGE_DLLIMPEXP DirectionTagGE : public TagGE 
{
	/* arx向导生成的代码 */
	/* 请参见MineGE类的说明 */
public:
	ACRX_DECLARE_MEMBERS(DirectionTagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

	/* 手动添加的代码 */
public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取方向角度
	double getDirectionAngle() const;

	// 设置方向角度
	void setDirectionAngle(double angle);

	/* arx向导生成的代码 */
public:
	/* 
	 * 和MFC的序列化类似，CAD调用dwgOutFields()方法将图元的数据保存到DWG文件中
	 * 具体来说，保存DirectionTagGE图元的"插入点坐标和方向角度"到DWG文件中
	 *
	 * 注1：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * 和MFC的序列化类似，CAD调用dwgInFields()方法从DWG文件中读取数据到图元中
	 * 具体来说，从DWG文件中读取"插入点坐标和方向角度"到LinkedGE图元
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* 
	 * 构造函数
	 * 注：根据ARX技术要求，所有的自定义类都必须提供一个默认构造函数(无参构造函数)
	 *      默认构造函数的访问权限可以是public，也可以是protected
	 */
protected:
	/* 默认构造函数 */
	DirectionTagGE();

	/* 构造函数，传入插入点坐标和方向角度参数 */
	DirectionTagGE(const AcGePoint3d& insertPt, double angle);

	/* 必须要重载实现的MineGE类纯虚函数 */
protected:
	/* 
	 * 将DirectionTagGE类图元的关键参数(插入点坐标和方向角度)写入到writer中
	 */
	virtual void writeKeyParam(DrawParamWriter& writer) const;

	/*
	 * 从reader中读取2个数据保存到DirectionTagGE图元的关键参数(插入点坐标和方向角度)中
	 */
	virtual void readKeyParam(DrawParamReader& reader);

	/* 关键参数(主要几何特征参数) */
private:	
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 方向角度
};

/* 
 * ARX向导生成的代码 
 * 具体请参见:MineGE.h文件末尾的说明
 */
#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TagGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DirectionTagGE)
#endif