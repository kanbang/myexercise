#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 点类图元：注氮孔、漏风源汇
class FFC_GE_EXPORT_API PointGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(PointGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	PointGE();
	PointGE(const AcGePoint3d& insertPt);

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 始终等于0
};

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PointGE)
#endif
