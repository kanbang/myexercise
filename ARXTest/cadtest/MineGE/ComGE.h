#pragma once

#include "MineGE.h"

// Com ==> ��д Composite
// ComGE�Ļ���Ч��������������ͼԪ��ͼ��Ч����϶���
// ������Office�е�"ͼ�����"
class MINEGE_DLLIMPEXP ComGE : public MineGE {

public:
	ACRX_DECLARE_MEMBERS(ComGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	bool appendEnt(const AcDbObjectId& objId);           // ���ͼԪ
	void getAllEnts(AcDbObjectIdArray& objIds) const;    // ��ȡ�������ͼԪ��id����
	void decomposeAllEnts();                             // ������

	// AcDbObject�ຯ������ʵ��
public:
	Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);
	Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	// ����MineGE�麯��
protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

	// Ŀǰֻʵ����"����draw"��"�任transform"����
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	virtual Acad::ErrorStatus subErase(Adesk::Boolean erasing);
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

protected:
	ComGE(void);
	void showEnt(const AcDbObjectId& objId, bool isVisible = false); // ����ͼԪʵ��
	void removeAllEnts();                                         // ������е�ͼԪ
	void simpleAppendEnt(const AcDbObjectId& objId);              // ��ͼԪ��ӵ����ͼԪ�У�������������
	void findEntsNotInOtherComGEs(const AcDbObjectIdArray& geIds, AcDbObjectIdArray& objIds);        // ������ComGE�в����뵱ǰ����ͼԪ�в��غϵĲ���

private:
	AcDbObjectIdArray m_geIds;
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ComGE)
#endif