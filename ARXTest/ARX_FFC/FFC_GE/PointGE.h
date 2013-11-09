#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// ����ͼԪ��ע���ס�©��Դ��
class FFC_GE_EXPORT_API PointGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(PointGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	PointGE();
	PointGE(const AcGePoint3d& insertPt);

	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // ʼ�յ���0
};

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PointGE)
#endif