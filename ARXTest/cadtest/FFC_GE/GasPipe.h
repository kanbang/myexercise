#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// 瓦斯钻孔(或瓦斯埋管)
class FFC_GE_EXPORT_API GasPipe : public MineGE 
{
public:
	ACRX_DECLARE_MEMBERS(GasPipe) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GasPipe();
	GasPipe(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~GasPipe();

	// 设置始末点坐标
	void setSEPoint(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	// 获取始末点坐标
	void getSEPoint(AcGePoint3d& startPt, AcGePoint3d& endPt) const;

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	AcGePoint3d m_startPt, m_endPt;    // 始末点坐标
} ;

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasPipe)
#endif
