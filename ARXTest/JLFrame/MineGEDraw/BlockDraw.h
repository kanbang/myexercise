#pragma once

#include "MineGEDraw.h"

/*
 * 有些图形的绘制非常复杂，例如指北针
 * 如果采用完全从头绘制的方法，可能需要考虑非常多的附加参数
 *
 * 利用CAD强大的"块定义"(Block)功能，可以预先定义复杂图块
 * 然后BlockDraw根据块名称读取块定义(块中包含的图形和数据)，并显示图形
 * BlockDraw已经实现了大部分功能，开发人员只需要派生并重载实现几个虚函数就行了
 *
 * 可视化开发人员使用BlockDraw需要按照如下步骤操作:
 *		1) 在CAD中定义块
 *		2) 实现一个可视化插件(ARX模块)，从BlockDraw类派生
 *		3) 在构造函数中修改成员变量m_blockName的值
 *		4) 如果块定义中包含数据(块属性)，则重载实现regPropertyDataNames()方法，同时
 *		   如果需要，重载readPropertyDataFromGE()方法，修改从图元读取到的属性数据m_attValues
 *		   注：必须保证块定义中添加的"块属性"名称与属性数据的字段名称是相同的
 *
 * 【关于块的几点说明】
 *	    1) 使用block命令定义的块称为"块定义"(Block Definition)
 *		2) 通过insert命令插入的块称为"块引用"(Block Reference)
 *		3) 双击"块引用"，进入"块编辑界面"添加属性，称为"属性定义"(Attribute Definition)
 *		4) 每个"属性定义"包含{标签名称(tag)、文本内容(text)}
 *		5) 每个块定义都是一个"块表记录"(Block Table Record, 这个是ARX里的一种数据结构，类似于数组)
 *		6) 每个块引用都是一个图形实体，保存在"模型空间块表记录"(所有屏幕上显示的图形都保存在这里)
 *  【提示】
 *	    1) 使用ARXDBG查看(编译ARX官方自带的例子ARXDBG得到)
 */
class MINEGE_DRAW_DLLIMPEXP BlockDraw : public MineGEDraw 
{
public:
	ACRX_DECLARE_MEMBERS(BlockDraw) ;

public:
	/* 析构函数 */
	virtual ~BlockDraw();

	/* 手动添加的代码 */
	/* 具体说明请参见MineGEDraw */
public:
	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void readPropertyDataFromGE(const AcStringArray& values);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // 实现为空

	/* 通过arx工具栏添加的代码 */
	/* 
	 * BlockDraw已经实现了大部分从AcDbEntity->MineGEDraw定义的以sub开头的虚函数
	 * 
	 * 注：以下sub开头的方法都是由CAD内部调用
	 */
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;

	virtual Acad::ErrorStatus subGetGripPoints( 
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

protected:
	/* 默认构造函数 */
	BlockDraw();

public:
	// 关键参数
	AcGePoint3d m_insertPt; // 插入点坐标
	double m_angle;         // 角度

	// 附加参数
	CString m_blockName;    // 块名称

	// 从图元提取的属性数据
	// 用户也可根据实际需求修改值
	AcStringArray m_attValues; // 属性数据
};

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BlockDraw)
#endif
