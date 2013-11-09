#pragma once

#include "GeologyGE.h"

// ����߽�
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

	// �ָ�ɿ�������εĵ�pos����
	void splitLine(int pos, const AcGePoint3d& pt);

	// ɾ���ɿ�������ε�һ������
	void removePoint(int pos);

	// �޸ĵ�pos���߽��
	void setPoint(int pos, const AcGePoint3d& pt);

	// �ڵ�pos��λ�ò�������
	// ����bPrevOrNext��ʾ��pos֮ǰ����֮����������
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
