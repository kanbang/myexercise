#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 指北针
class DEFGE_EXPORT_API Compass : public MineGE {

public:
	ACRX_DECLARE_MEMBERS(Compass) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Compass();
	Compass(const AcGePoint3d& insertPt, double angle);
	virtual ~Compass() ;

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取方向角度
	double getDirectionAngle() const;

	// 设置方向角度
	void setDirectionAngle(double angle);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 方向角度
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Compass)
#endif
