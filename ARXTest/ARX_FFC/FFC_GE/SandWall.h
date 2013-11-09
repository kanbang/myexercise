#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 沙土密闭墙
class FFC_GE_EXPORT_API SandWall : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(SandWall) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	SandWall();
	SandWall(const AcGePoint3d& pt, double angle, double length);

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取方向角度
	double getDirectionAngle() const;

	// 设置方向角度
	void setDirectionAngle(double angle);

	// 获取墙的长度
	double getLength() const;

	// 设置墙的长度
	void setLength(double L);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	AcGePoint3d m_pt;        // 插入点坐标
	double m_angle;          // 方向角度
	double m_length;         // 墙的长度
};

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SandWall)
#endif
