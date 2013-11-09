#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// ���ͼԪ(���������ڲ�ʹ��)
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
	// ���ò��������
	void setInsertPoint(const AcGePoint3d& insertPt);

	// ��ȡ���������
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