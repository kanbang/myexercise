#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// �ɿ���(���ͼ��)
class FFC_GE_EXPORT_API Goaf : public MineGE 
{
public:
	ACRX_DECLARE_MEMBERS(Goaf) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Goaf();
	virtual ~Goaf();

	// ��ɿ�����ӱ߽��
	void addPoint(const AcGePoint3d& pt);
	
	// ��ȡ�ɿ��������
	void getPolygon(AcGePoint3dArray& polygon) const;

	// �ָ�ɿ�������εĵ�pos����
	void splitLine(int pos, const AcGePoint3d& pt);

	// ɾ���ɿ�������ε�һ������
	void removePoint(int pos);

	// �޸ĵ�pos���߽��
	void setPoint(int pos, const AcGePoint3d& pt);

	// �ڵ�pos��λ�ò�������
	// ����bPrevOrNext��ʾ��pos֮ǰ����֮����������
	void insertPoints(int pos, const AcGePoint3dArray& pts, bool bPrevOrNext);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

private:
	AcGePoint3dArray m_pts;
} ;

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Goaf)
#endif
