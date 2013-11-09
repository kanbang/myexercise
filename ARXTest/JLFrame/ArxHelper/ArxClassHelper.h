#pragma once

#include "dlimexp.h"

/* 
 * ARX内置的类以及自定义类都需要添加到ARX的类层次结构树(Class Hierarchy)
 * ARX的类按照派生关系构成层次结构树(Class Hierarchy) 
 * 命令：arx-->选项O-->选项CL，CAD命令行输出整个Class Hierarchy(按F2查看放大命令行窗口)
 *
 * ARX实现了一套RTTI(run time type infomation, 运行时类型信息)，
 * 每个ARX类都对应一个ArxClass对象
 * RRTI的一些比较简单的用途：
 *	  1) 给定一个对象，获取对象所属类的字符串名称
 *    2) 判断2个对象之间是否存在派生关系
 *    3) 获取对象的所有父类名称(在ARX中基本上不使用多重继承)
 *    4) 获取对象的所有子类名称
 *	  5) 给定字符串类型名称，动态创建一个对象指针(这个应用是最多的)
 *
 * ARX类层次结构树对应的ARX数据结构是一个词典(AcRxDictionary)
 * 可通过acrxClassDictionary宏来获取对象指针
 * 最典型的用法：通过acrxClassDictionary提供的方法查询类型对应的ArxClass对象
 *    AcRxClass* pClass = AcRxClass::cast(acrxClassDictionary->at(_T("Tunnel")));
 * 注意：方法只是要求传入一个字符串
 */
class ARXHELPER_DLLIMPEXP ArxClassHelper
{
public:
	/* 判断类型是否有效，即在arx类体系结构中是否已注册 */
	/* 从arx基类(AcDbObject/AcDbEntity)派生的类都会注册到arx类体系结构中 */
	static bool IsValidClass(const CString& type);
	
	/* 判断类型是否抽象类(抽象类不能实例化) */
	/* 抽象类通常是构造函数为protected或则类里包含纯虚函数 */
	static bool IsAbstractClass(const CString& type);
	
	/* 判断类型之间是否具有派生关系 */
	static bool IsDerivedFrom(const CString& type, const CString& parentType);
	
	/* 获取类型的dxf名称(从arx基类派生的类都有一个dxf名称，可以是中文名称) */
	/* 在cad中把鼠标停留在一个图形上面会显示图形的dxf名称，例如AcDbLine的dxf名称就是【直线】 */
	static bool GetDxfName(const CString& type, CString& dxfName);
	
	/* 查找类型的所有基类 */
	static void GetClassHierarchy(const CString& type, AcStringArray& types, bool findAbstractType);
	
	/* 查找类型的所有子类 */
	static bool GetChildClass(const CString& type, AcStringArray& types);	
	
	/* 查找类型的距离最近的非抽象基类 */
	static bool GetTopParentClass(const CString& type, CString& parentType);
	
	/* 查找类型的所有非抽象基类(包括基类的基类...) */
	static bool GetAllTopParentClass(const CString& root, AcStringArray& types);

	/* 查找基类root的所有子类 */
	static void GetArxClassTypes(const CString& root, AcStringArray& types, bool findAbstractType);		

	/* 给定类型名称，动态创建对象并返回指针 */
	static AcDbObject* CreateObjectByType(const CString& type);
};
