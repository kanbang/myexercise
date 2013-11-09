#pragma once

#include "dlimexp.h"

/*
 * 数据对象(DataObject)观察者(参见：观察者模式)
 * 例如:
 *		绘制通风网络图时，巷道、工作面图元都被映射成一条弧线分支
 *      且分支只使用它们的一部分数据(通风数据)
 */
class MINEGE_DLLIMPEXP ModelGE : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ModelGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// 返回图元类型名称
	CString getTypeName() const;

	// 获取数据源
	AcDbObjectId getDataObject() const;

	// 设置数据源
	void setDataObject(const AcDbObjectId& objId);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	ModelGE();

private:
	AcDbObjectId m_objId;  // 数据源(从MineGE派生的图元或者DataObject类的数据对象)
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ModelGE)
#endif
