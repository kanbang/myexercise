#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// ����ͼԪ�������ͼԪ����˹ͻ���㣬�����㣬ѹ���㣬��׵�
class GASGE_EXPORT_API PointGE : public MineGE
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

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
	double m_angle;          // ʼ�յ���0
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PointGE)
#endif
