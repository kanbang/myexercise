#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// ɳ���ܱ�ǽ
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
	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

	// ��ȡ����Ƕ�
	double getDirectionAngle() const;

	// ���÷���Ƕ�
	void setDirectionAngle(double angle);

	// ��ȡǽ�ĳ���
	double getLength() const;

	// ����ǽ�ĳ���
	void setLength(double L);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	AcGePoint3d m_pt;        // ���������
	double m_angle;          // ����Ƕ�
	double m_length;         // ǽ�ĳ���
};

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SandWall)
#endif
