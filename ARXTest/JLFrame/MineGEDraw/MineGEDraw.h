#pragma once

#include "dbmain.h"
#include "dlimexp.h"
#include "../ArxDbgXdata/XDataStream.h"

// 圆周率
#define PI 3.1415926535897932384626433832795

// 矿井图元可视化效果抽象基类
class MINEGE_DRAW_DLLIMPEXP MineGEDraw : public AcDbEntity 
{
	/* ARX向导生成的代码 */
public:
	ACRX_DECLARE_MEMBERS(MineGEDraw) ;

public:
	/* 析构函数 */
	virtual ~MineGEDraw () {}

	/* 设置所有附加参数的默认值 */
	virtual void setAllExtraParamsToDefault() = 0;

	/*
	 * 弹出一个对话框进行附加参数的修改
	 * 例如，1）修改双线巷道的宽度
	 */
	virtual void configExtraParams() = 0;

	/*
	 * 当附加参数发生变化的时候，更新其它的附加参数
	 * 例如双线巷道，当巷道始末点坐标、宽度变化时，需要重新计算轮廓线坐标
	 */
	virtual void updateExtraParams() = 0;

	/*
	 * 读写关键参数
	 * 纯虚函数，派生类必须要实现!!!
	 */
	virtual void readKeyParam(DrawParamReader& reader) = 0;
	virtual void writeKeyParam(DrawParamWriter& writer) = 0;

	/*
	 * 读写附加参数
	 * 纯虚函数，派生类必须要实现!!!
	 */
	virtual void readExtraParam(DrawParamReader& reader) = 0;
	virtual void writeExtraParam(DrawParamWriter& writer) = 0;

	/*
	 * 派生类重载该虚函数，填充names数组
	 * 注：names数组保存的都是"字段"
	 */
	virtual void regPropertyDataNames(AcStringArray& names) const {}

	/*
	 * 图元根据可视化效果对象请求的字段数组(names)
	 * 从数据对象中读取相应的数据，并填充到数据数组(values)
	 * 注：派生类可以根据可视化的需要对数据进行修改
	 * 
	 * 例如：向测风站图元(WindStation)请求2个数据:
	 *		  names = {"风量"、"风速"}
	 
	 *	      返回数据数组:
	 *		  vlaues = {"25"、"3.1"}
	 *
	 *        如果要将风量改变单位显示，则可以修改：
	 *		  Q = 0;  // 风量
	 *		  ArxUtilHelper::StringToDouble(values[0], Q);
	 *		  Q = Q*60;      // (m3/s --> m3/min)
	 *		  V = 0;  // 风速
	 *	      ArxUtilHelper::StringToDouble(values[1], V);
	 *			
	 *		 其中:Q、V是测风站可视化效果派生类中定义的2个成员变量(double类型)
	 *		  
	 */
	virtual void readPropertyDataFromGE(const AcStringArray& values) {}

	/*
	 * 计算可视化效果的最小多边形
	 * 用于MineGE图元绘制"背景消隐块"，实现"消隐"效果
	 * 
	 * 注1：BlockDraw采用块定义作为模板，无法定义最小多边形
	 *      因此，使用BlockDraw派生类定义的可视化效果的图元没有消隐效果
	 * 注2：消隐效果指的是位于上方的图元会覆盖位于下方图元的部分图形
	 */
	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // 默认实现，什么也不做

protected:
	/* 默认构造函数 */
	MineGEDraw();
} ;

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
 * MINEGEDRAW_MODULE宏在MineGEDraw项目中定义
 * 项目属性 --> 配置属性 --> C/C++ --> 预处理器 --> 预处理器定义
 */
#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGEDraw)
#endif
