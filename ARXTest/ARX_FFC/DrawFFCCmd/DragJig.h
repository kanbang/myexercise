#pragma once

#include "JunctionEdgeInfo.h"

#include "../DefGE/Tunnel.h"
#include "../DefGE/WorkSurface.h"

#include "../FFC_GE/Goaf.h"

class DumyJigEntity;

/*
 * ���ڵ�����
 * �ɿ���Goaf���϶������в���ʾ
 * ���ֻ��ʾ�ɿ����ı仯
 * virtual AcDbEntity * entity() const
 * { 
 *		return m_pGoaf;
 * }
 * ����ʾЧ���������ģ��ɿ�����������ƶ����仯
 * ԭ����....
 */
class DragJig : public AcEdJig  
{
public:
	DragJig(const AcDbObjectId ws_objId, 
		JunctionEdgeInfoArray& sptLines, 
		JunctionEdgeInfoArray& eptLines,
		const AcDbObjectId& goaf_objId,
		int p1, int p2);
	virtual ~DragJig();
	bool doIt();

	virtual AcDbEntity * entity() const;
	virtual Adesk::Boolean update();
	virtual AcEdJig::DragStatus  sampler();

private:
	// ������
	WorkSurface* m_pWS;
	// ʼ����������
	AcDbVoidPtrArray m_pSptLines;
	// ĩ����������
	AcDbVoidPtrArray m_pEptLines;
	// �����Ĳɿ���
	Goaf* m_pGoaf;

	// ���������Ϣ
	JunctionEdgeInfoArray& m_sptLinesInfo;
	JunctionEdgeInfoArray& m_eptLinesInfo;
	// �����Ĳɿ�����Ϣ
	AcGePoint3dArray m_polygon;
	int m_p1, m_p2;

	DumyJigEntity* m_pJigEnt;

	AcGePoint3d m_spt, m_ept;
	AcGeVector3d m_dir;
	AcGePoint3d m_basePt;
	double m_dist;
};
