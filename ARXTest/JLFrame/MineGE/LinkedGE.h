#pragma once

#include "MineGE.h"

/*
 * LinkedGE类图元包括：巷道、工作面、硐室、掘进工作面等
 * 它们在矿井系统中起连接管道的作用，用于通过风流、运输煤炭及物料、人行车辆通过等
 *
 * 主要几何特征：1)插入点坐标 2)方向角度
 *
 * 注：MINEGE_DLLIMPEXP宏表示DLL导入/导出指令
 */
class MINEGE_DLLIMPEXP LinkedGE : public MineGE
{
	/* arx向导生成的代码 */
	/* 请参见MineGE类的说明 */
public:
	ACRX_DECLARE_MEMBERS(LinkedGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

	/* 手动添加的代码 */
public:
	/* 设置始末点坐标 */
	void setSEPoint(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	/* 获取始末点坐标 */
	void getSEPoint(AcGePoint3d& startPt, AcGePoint3d& endPt) const;

	/* 反向(交换始末点坐标) */
	void reverse();

	/* 计算两坐标之间的角度(X轴逆时针旋转) */
	double getAngle() const;

	/* 手动添加的代码 */
	/* 
	 * 处理LinkedGE图元连接点闭合 
	 * 具体实现过程参见:
	 *		1) EdgeJunctionClosure.cpp
	 *		2) "JLFrame开发文档.doc"-->"LinkedGE图元连接闭合处理"章节
	 * 同MineGE类一样，具体的实现也是委托LinkedGEDraw派生的可视化对象处理
	 */
public:
	/*
	 * 获取始端的"向内"的延伸方向角(默认为始末点直线倾角)
	 * 返回的向量都是标准化向量(向量长度为1)
	 * 注："零向量"表示不处理闭合
	 */
	AcGeVector3d getStartPointInExtendAngle() const;

	/*
	 * 获取末端的"向内"的延伸方向角(默认为始末点直线倾角的反方向)
	 * 返回的向量都是标准化向量(向量长度为1)
	 * 注："零向量"表示不处理闭合
	 */
	AcGeVector3d getEndPointInExtendAngle() const;

	/* 处理始端与边界线（射线）的相交(默认不处理) */
	void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);

	/* 处理末端与边界线（射线）的相交(默认不处理) */
	void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

	/* 延长一定距离 */
	void extendByLength(double length);

	/* arx向导生成的代码 */
public:
	/* 
	 * 和MFC的序列化类似，CAD调用dwgOutFields()方法将图元的数据保存到DWG文件中
	 * 具体来说，保存LinkedGE图元的始末点坐标到DWG文件中
	 *
	 * 注1：调用CAD命令(移动、旋转、缩放)以及夹点编辑时，会产生一些动态效果
	 *       在这些动态效果产生的过程中会频繁的调用dwgOutFields()方法
	 * 注2：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * 和MFC的序列化类似，CAD调用dwgInFields()方法从DWG文件中读取数据到图元中
	 * 具体来说，从DWG文件中读取始末点坐标到LinkedGE图元
	 *
	 * 注1：调用CAD命令(移动、旋转、缩放)以及夹点编辑时，会产生一些动态效果
	 *       在这些动态效果产生的过程中会频繁的调用dwgInFields()方法
	 * 注2：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* 必须要重载实现的MineGE类纯虚函数 */
protected:
	/* 
	 * 将LinkedGE类图元的关键参数(始末点坐标)写入到writer中
	 */
	virtual void writeKeyParam(DrawParamWriter& writer) const;

	/*
	 * 从reader中读取2个数据保存到LinkedGE图元的关键参数(始末点坐标)中
	 */
	virtual void readKeyParam(DrawParamReader& reader);

	/* 通过arx工具栏添加的代码 */
	/* 
	 * LinkedGE图元需要实现一些特殊效果，因此需要重载实现部分AcDbEntity类虚函数 
	 * 
	 * 注：以下sub开头的方法都是由CAD内部调用(AcDbObject->AcDbEntity->MineGE)
	 */
protected:
	/*
	 * 对LinkedGE图元进行变换操作后自动处理连接点闭合
	 * 例如：
	 *	对巷道执行移动命令，移动巷道前所在的位置的连接点要闭合
	 *  移动巷道后所在位置的连接点闭合也要处理
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	/*
	 * 对LinkedGE图元进行夹点编辑操作后，自动处理连接点闭合
	 * 例如：
	 *	选中巷道的末点然后进行拖拽，同上，拖拽前后的位置都需要处理闭合
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

	/*
	 * 在CAD窗口中删除LinkedGE图元后，自动处理连接点闭合	
	 *
	 * 注：该方法由CAD内部调用!!!
	 */
	virtual Acad::ErrorStatus subErase(Adesk::Boolean erasing);

	/* 
	 * 构造函数
	 * 注：根据ARX技术要求，所有的自定义类都必须提供一个默认构造函数(无参构造函数)
	 *      默认构造函数的访问权限可以是public，也可以是protected
	 */
protected:
	/* 默认构造函数 */
	LinkedGE();

	/* 有参数的构造函数，需要传入始末点坐标 */
	LinkedGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

private:
	void doEdgeGEJunctionClosure();    // 处理闭合

protected:
	/* 关键参数(主要几何特征参数) */
	AcGePoint3d m_startPt, m_endPt;    // 始末点坐标
} ;

/* 
 * ARX向导生成的代码 
 * 具体请参见:MineGE.h文件末尾的说明
 */
#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LinkedGE)
#endif