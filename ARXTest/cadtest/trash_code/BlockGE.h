#pragma once

#include "MineGE.h"

class MINEGE_DLLIMPEXP BlockGE : public MineGE {

public:
	ACRX_DECLARE_MEMBERS(BlockGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

	// ��ȡ����Ƕ�
	double getDirectionAngle() const;

	// ���÷���Ƕ�
	void setDirectionAngle(double angle);

	// ��ȡ������
	CString getBlockName() const;

	// ���ÿ�����
	void setBlockName(const CString& blockName);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	BlockGE();
	BlockGE(const AcGePoint3d& insertPt, double angle, const CString& blockName);

	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // ����Ƕ�
	CString m_blockName;     // �鶨������
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BlockGE)
#endif