#pragma once

#include "dlimexp.h"

// 可视化效果基类
#include "../MineGEDraw/MineGEDraw.h"
// 扩展数据读写接口，用于MineGE和MineGEDraw之间交换数据
#include "../ArxDbgXdata/XDataStream.h"

/*
 * 矿井系统图元基类(抽象类)
 *
 * 注1：抽象类是准备被用做基类的，抽象类应该包含一个纯虚函数
 * 注2：MINEGE_DLLIMPEXP宏表示DLL导入/导出指令
 */
class MINEGE_DLLIMPEXP MineGE : public AcDbEntity 
{
	/* arx向导生成的代码 */
	/*
	 * 所有从AcDbObject/AcDbEntity派生的类必须使用ACRX_DECLARE_MEMBERS宏
	 * 给该类添加RTTI信息
	 * 类似于MFC中的DECLARE_DYNAMIC / IMPLEMENT_DYNAMIC
	 *
	 * 注：根据ARX技术要求，所有的自定义类都必须提供一个默认构造函数(无参构造函数)
	 *      默认构造函数的访问权限可以是public，也可以是protected
	 */
public:
	ACRX_DECLARE_MEMBERS(MineGE) ;

	/* arx向导生成的代码 */
protected:
	/* 用于序列化，表示类的版本号 */
	static Adesk::UInt32 kCurrentVersionNumber;

	/* 手动添加的代码 */
	/* 公共接口 */
public:
	/* 
	 * 虚析构函数是为了解决这样的一个问题：
	 *		基类的指针指向派生类对象，并用基类的指针删除派生类对象
	 */
	virtual ~MineGE ();	

	/* 
	 * 获取图元的类型名称
	 */
	CString getTypeName() const;

	/*
	 * 附加参数(Extra Param)被修改之后，更新图元可视化效果
	 * 例如：修改双线巷道的宽度之后，应该调用该方法更新可视化效果
	 */
	void configDraw(const CString& drawName);

	/*
	 * 关键参数(Key Param)被修改之后，更新图元可视化效果
	 * 例如：巷道的始末点坐标被修改，应该调用该方法更新可视化效果
	 */
	void updateDraw();

	/*
	 * 获取图元关联的数据对象ID
	 * AcDbObjectId是ARX提供的一种表示图元ID的辅助类
	 */
	AcDbObjectId getDataObject() const;

	/* 手动添加的代码 */
	/* 所有图元的派生类必须要重载实现的虚函数 */
protected:
	/* 
	 * 将关键参数写入到writer中
	 * DrawParamWriter实质就是一个链表数据结构
	 * 参见:ArxDbgXData/XDataStream.h/DrawParamWriter类
	 */
	virtual void writeKeyParam(DrawParamWriter& writer) const = 0;

	/*
	 * 从reader中读取关键参数
	 * DrawParamReader实质就是一个链表数据结构
	 * 参见:ArxDbgXData/XDataStream.h/DrawParamReader类
	 */
	virtual void readKeyParam(DrawParamReader& reader) = 0;

	/* arx向导生成的代码 */
	/* 必须要重载实现的AcDbObject类虚函数 */
public:
	/* 
	 * 和MFC的序列化类似，CAD调用dwgOutFields()方法将图元的数据保存到DWG文件中
	 *
	 * 注1：调用CAD命令(移动、旋转、缩放)以及夹点编辑时，会产生一些动态效果
	 *       在这些动态效果产生的过程中会频繁的调用dwgOutFields()方法
	 * 注2：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * 和MFC的序列化类似，CAD调用dwgInFields()方法从DWG文件中读取数据到图元中
	 *
	 * 注1：调用CAD命令(移动、旋转、缩放)以及夹点编辑时，会产生一些动态效果
	 *       在这些动态效果产生的过程中会频繁的调用dwgInFields()方法
	 * 注2：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* 通过arx工具栏添加的代码 */
	/* 
	 * 必须要重载实现的AcDbEntity类虚函数 
	 * 注：以下sub开头的方法都是由CAD内部调用，且必须实现的虚函数
	 *      AcDbEntity派生自AcDbObject
	 */
protected:
	/* 
	 * 该方法是自定义实体技术中最重要的方法
	 * CAD内部调用subWorldDraw()方法来绘制实体图形
	 * 其中AcGiWorldDraw对象可以看作是MFC中的CDC类
	 * 两者都是用于在屏幕上绘制图形
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	/*
	 * 变换操作(移动、旋转、缩放)
	 * 当在CAD上选择一个实体，执行以下的命令："移动、旋转、缩放"
	 * CAD内部调用subTransformBy()方法，来执行实体的变换操作
	 *
	 * 在计算机图形学中，图形的移动、旋转、缩放统称为"变换(transform)"操作,
	 * 而变换操作则是对应一个矩阵(AcGeMatrix3d)
	 * 
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	/* 
	 * 当执行CAD命令的时候，如果鼠标移动到图元的附近，
	 * CAD会自动触发相应的捕捉点，并用一个“高亮显示的黄色小方框”显示捕捉点
	 *
	 * 注：该方法是由CAD内部来调用的!!!
	 */
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	/* 
	 * 当用户选择一个图元，CAD使用"蓝色实心小方框"高亮显示"夹点"
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds) const ;

	/*
	 * 鼠标拖拽不同的夹点可能会产生不同的行为：
	 *      移动(MOVE)、延伸(STRETCH)、旋转(ROTATE)
	 * 也可以说夹点编辑操作是"移动、拉伸、旋转"3个命令的另外一种操作方式，
	 * 使用夹点编辑可以极大地提高用户的人机交互操作体验，不需要频繁的输入命令
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	/*
	 * 确定图元的一个"紧凑"的3d包围盒(立方体)
	 * 如果是二维图形，那就是一个长方形
	 * 
	 * 该方法主要用于计算缩放的范围
	 * 参见:ArxHelper/ArxEntityHelper.h/ZoomToEntity和ZoomToEntities方法
	 * 
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

	/* 
	 * ==>前期知识准备：
	 * CAD中的图形实体仍然是一个C++对象，与常规C++对象不同的是
	 * 图形实体对象的生命周期是由CAD图形数据库系统来负责的
	 * 开发人员不允许直接操纵图元的对象指针(更不能直接删除对象指针)
	 * CAD向图元对象额外添加了一层"读写权限"包装
	 * 所有的读写操作都需要遵循"打开(Open)/关闭(Close)"机制
	 *
	 * 在CAD中，通常情况下是不能直接操作图元的对象指针的，必须通过Open/Close机制
	 * (1) 打开图元对象，得到对象指针
	 *     AcDbObjectId objId;      // 通过某种途径得到图元对象id(例如选择图形)
	 *     AcDbLine* pLine;         // 假设要打开的是一个直线对象
	 *     acdbOpenObject(pLine, objId, AcDb::kForRead); // 以read状态打开对象
	 * (2) 通过指针对象执行任务
	 *     使用read状态打开只能读取数据，如果修改图元对象的数据，
	 *     那么CAD会发生错误，然后终止退出
	 *     pLine->getXXX();         // ok
	 *     pLine->setStartPoint();  // error
	 * (3) 关闭对象
	 *     pLine->close();          // 要及时关闭图元对象指针，否则会出现一些莫名其妙的错误
	 *
	 * 当用户调用close()方法时，CAD内部会调用subClose()方法
	 * 由于C++的构造函数中不能调用虚函数，因此无法执行一些与派生类相关的初始化工作
	 * 当用户new一个新的图元，然后添加到CAD的图形数据库，添加成功之后，必须要关闭图元对象
	 * 例如： 
	 *     Tunnel* pTunnel = new Tunnel(p1, p2);     // 创建一条巷道,输入始末点坐标参数
	 *	   ArxUtilHelper::PostToModelSpace(pTunnel); // 把巷道添加到CAD图形数据库，如果添加成功，
	 *                                               // 则调用close()方法关闭图元对象，同时显示图形
	 * 我们可以把一些初始化动作放在subClose()方法中
	 *
	 * 注1：close()方法会被CAD在后台频繁调用，当鼠标即使只是在CAD屏幕上移动不做其它操作，CAD也会调用close()方法
	 * 注2：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subClose(void);

protected:
	/* 默认构造函数 */
	MineGE();

	/*
	 * 更新绘制参数(关键参数和附加参数)
	 * true  -- 将参数写入到当前可视化效果对象中
	 * false -- 将参数读取回来到ge中
	 * 与writeParamToGEDraw和readParamFromGEDraw方法在功能上重复(***)
	 */
	void updateDrawParam(bool readOrWrite = true) const;

	// 获取当前draw对象(方便子类使用，避免直接操纵私有数据)
	MineGEDraw* getCurrentDraw() const;

	/* 私有辅助方法 */
private:
	/*
	 * 从可视化效果对象中提取一个多边形，用CAD窗口的背景颜色绘制并填充多边形
	 * 相当于绘制了一个"背景消隐块"，位于下方的图元则会被"覆盖"一部分图形
	 */
	void drawBackground(MineGEDraw* pGEDraw, AcGiWorldDraw *mode);

	/*
	 * 从可视化效果对象pGEDraw中读取关键参数
	 * 然后调用readKeyParam()方法将关键参数更新到派生类
	 */
	void readKeyParamFromGEDraw(MineGEDraw* pGEDraw);

	/*
	 * 调用writeKeyParam()方法从派生类读取关键参数
	 * 然后将关键参数写入到可视化效果对象pGEDraw
	 */
	void writeKeyParamToGEDraw(MineGEDraw* pGEDraw) const;

	/*
	 * 从可视化效果对象pGEDraw中读取附加参数
	 * 然后调用readKeyParam()方法将附加参数更新到派生类
	 */
	void readExtraParamFromGEDraw(MineGEDraw* pGEDraw);

	/*
	 * 调用writeKeyParam()方法从派生类读取附加参数
	 * 然后将附加参数写入到可视化效果对象pGEDraw
	 */
	void writeExtraParamToGEDraw(MineGEDraw* pGEDraw) const;	

	/*
	 * 从可视化效果对象pGEDraw中读取关键参数和附加参数
	 * 然后调用readKeyParam()方法将参数更新到派生类
	 * 等价于同时调用
	 *		readKeyParamFromGEDraw(pGEDraw);
	 * 和
	 *		readExtraParamFromGEDraw(pGEDraw);
	 */
	void readParamFromGEDraw(MineGEDraw* pGEDraw);

	/*
	 * 调用writeKeyParam()方法从派生类读取关键参数和附加参数
	 * 然后将参数写入到可视化效果对象pGEDraw
	 * 等价于同时调用
	 *		writeKeyParamToGEDraw(pGEDraw);
	 * 和
	 *		writeExtraParamToGEDraw(pGEDraw);
	 */
	void writeParamToGEDraw(MineGEDraw* pGEDraw) const;

	/*
	 * 当添加一个图元到数据库，MineGE读取系统中已经注册的所有可视化效果
	 * 依次将图元的关键参数传递给可视化效果对象，可视化效果对象计算附加参数
	 * 然后从可视化效果对象中读取附加参数并保存到图元的扩展数据中
	 * 格式：
	 *		(可视化效果1 附加参数1 附加参数2 ...) (可视化效果2 附加参数1 附加参数2 ...) ...
	 * 例如，巷道的可视化效果，包括双线巷道可视化效果、单线巷道可视化效果、宽多段线巷道可视化效果
	 *		(DoubleTunnelDraw 20 14.5 12.6 20.1 29.1) --> (SingleTunnelDraw 2) ... 
	 */
	void initAllExtraParamsToXData();

	/*
	 * 从图元的扩展数据中读取所有可视化效果名称
	 * 注：AcStringArray是ARX提供的字符串数组类
	 */
	void extractExistedDraw(AcStringArray& existedDraw);

	/* 
	 * 初始化可视化效果 
	 * 读取注册的可视化效果附加参数并保存到扩展数据(XData)
	 * 与initAllExtraParamsToXData()方法是等价的
	 */
	void initDraw();

	/*
	 * 初始化属性数据 
	 * 创建一个数据对象(DataObject)并保存到图元的扩展词典中
	 */
	void initPropertyData();

	// 写入属性数据到MineGEDraw中
	/*
	 * 有些图元的可视化效果可能与属性数据有关联
	 * 例如风流方向，新风和乏风绘制出来的效果是不一样的，但它们的图元都只有一个
	 * 即不再额外派生"新风图元类"和"乏风图元类"，只有一个"风流方向图元类(Direction)"
	 * 在绘制图形的时候，可视化效果对象(pGEdraw)向图元发送字段名称，请求要读取的属性数据
	 * 图元则从数据对象中读取数据，然后再传递给可视化效果对象
	 */
	void writePropertyDataToGEDraw(MineGEDraw* pGEDraw) const;

	/*
	 * 当图元的关键参数发生变化时，图元的附加参数也可能要随之变化
	 * 例如巷道的始末点坐标变化，则双线巷道的两侧轮廓点坐标也需要重新计算
	 * MineGE从派生类中读取关键参数，传递给可视化效果对象，由它计算附加参数
	 * 然后再从可视化效果对象中将更新后的附加参数读取到MineGE图元的扩展数据中
	 */
	void updateDrawParams(MineGEDraw* pGEDraw);

	/*
	 * 移动、旋转、缩放、夹点编辑命令执行过程中
	 * 图形随着鼠标移动不停的变化
	 * 在动态变化的过程会不断的产生新的图元对象(new object)
	 * 这些临时图元没有提交到数据库(称为new object)，且仅用于显示图形
	 * 因此这些图元对象不会调用subClose()方法，临时图元没有附加扩展数据
	 * 从而导致动态变化的过程中，没有图形显示(或者显示不正确)
	 * 该方法保证临时图元(new object)的扩展数据能够正确地被初始化
	 */
	void initNewObjectExtraParams();

private:
	// 当前可视化效果对象指针
	// 图元的可视化效果都是通过该指针委托实现
	// 例如：绘制、变换(移动、旋转、缩放)、捕捉点、夹点编辑
	MineGEDraw* m_pCurrentGEDraw;
	// 图元关联的数据对象
	AcDbObjectId m_dataObjectId;
};

/* 
 * ARX向导生成的代码 
 * 在老版本的ARX中，开发人员从AcDbObject/AcDbEntity派生自定义类
 * 必须要手动的调用ARX提供的方法将自定义类注册到CAD类层次结构树中(Class Hierarchy)
 * 参见：ArxHelper/ArxClassHelper.h
 *
 * 如果开发人员由于疏忽忘记了这样做，则会出现一些莫名其妙的错误
 * 例如：添加图元到数据库后在CAD屏幕上看不到图形
 *
 * 新版本的ARX提供了辅助宏(ACDB_REGISTER_OBJECT_ENTRY_AUTO)，
 * 自动注册自定义类到CAD类层次结构树中，减少了开发人员犯错的机会
 *
 * MINEGE_MODULE宏在MineGE项目中定义
 * 项目属性 --> 配置属性 --> C/C++ --> 预处理器 --> 预处理器定义
 */
#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGE)
#endif