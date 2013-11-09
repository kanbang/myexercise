#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 巷道图元(不导出，内部使用)
class DEFGE_EXPORT_API Joint : public MineGE {

public:
	ACRX_DECLARE_MEMBERS(Joint) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Joint () ;
	Joint(const AcGePoint3d& insertPt);
	virtual ~Joint () ;

public:
	// 设置插入点坐标
	void setInsertPoint(const AcGePoint3d& insertPt);

	// 获取插入点坐标
	AcGePoint3d getInsertPoint() const;

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	AcGePoint3d m_insertPt;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Joint)
#endif