#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// ָ����
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
	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

	// ��ȡ����Ƕ�
	double getDirectionAngle() const;

	// ���÷���Ƕ�
	void setDirectionAngle(double angle);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // ����Ƕ�
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Compass)
#endif
