#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风筒
// 关联的图元必须是掘进工作面!!!
class DEFGE_EXPORT_API Chimney : public TagGE {

public:
	ACRX_DECLARE_MEMBERS(Chimney) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Chimney () ;
	virtual ~Chimney () ;

	void addControlPoint(const AcGePoint3d& pt);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	AcGePoint3dArray m_pts;         // 风筒控制点
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Chimney)
#endif
