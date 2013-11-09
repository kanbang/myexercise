#pragma once

#include "GeologyGE.h"

// 井田边界
class GASGE_EXPORT_API MineBoundary : public GeologyGE {

public:
	ACRX_DECLARE_MEMBERS(MineBoundary) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	MineBoundary () ;
	virtual ~MineBoundary () ;
	
	void addControlPoint(const AcGePoint3d& pt);
	void getControlPoints(AcGePoint3dArray& pts);

	// 分割采空区多边形的第pos条边
	void splitLine(int pos, const AcGePoint3d& pt);

	// 删除采空区多边形的一个顶点
	void removePoint(int pos);

	// 修改第pos个边界点
	void setPoint(int pos, const AcGePoint3d& pt);

	// 在第pos个位置插入多个点
	// 参数bPrevOrNext表示在pos之前还是之后插入点坐标
	void insertPoints(int pos, const AcGePoint3dArray& pts, bool bPrevOrNext);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	AcGePoint3dArray m_pts;
} ;

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineBoundary)
#endif
