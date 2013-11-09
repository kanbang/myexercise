#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 采空区(封闭图形)
class FFC_GE_EXPORT_API Goaf : public MineGE 
{
public:
	ACRX_DECLARE_MEMBERS(Goaf) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Goaf();
	virtual ~Goaf();

	// 向采空区添加边界点
	void addPoint(const AcGePoint3d& pt);
	
	// 获取采空区多边形
	void getPolygon(AcGePoint3dArray& polygon) const;

	// 分割采空区多边形的第pos条边
	void splitLine(int pos, const AcGePoint3d& pt);

	// 删除采空区多边形的一个顶点
	void removePoint(int pos);

	// 修改第k个边界点
	void setPoint(int pos, const AcGePoint3d& pt);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	AcGePoint3dArray m_pts;
} ;

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Goaf)
#endif
