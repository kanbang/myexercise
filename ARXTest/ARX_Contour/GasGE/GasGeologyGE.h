#pragma once

#include "GasGE.h"

// 瓦斯地质类图元基类
class GASGE_EXPORT_API GasGeologyGE : public GasGE
{
public:
	ACRX_DECLARE_MEMBERS(GasGeologyGE) ;

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
	GasGeologyGE();
	GasGeologyGE(const AcGePoint3d& insertPt);

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 始终等于0
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasGeologyGE)
#endif
