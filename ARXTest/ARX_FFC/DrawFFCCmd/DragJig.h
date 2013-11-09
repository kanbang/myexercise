#pragma once

#include "JunctionEdgeInfo.h"

#include "../DefGE/Tunnel.h"
#include "../DefGE/WorkSurface.h"

#include "../FFC_GE/Goaf.h"

class DumyJigEntity;

/*
 * 存在的问题
 * 采空区Goaf在拖动过程中不显示
 * 如果只显示采空区的变化
 * virtual AcDbEntity * entity() const
 * { 
 *		return m_pGoaf;
 * }
 * 则显示效果是正常的，采空区跟随鼠标移动而变化
 * 原因不明....
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
	// 工作面
	WorkSurface* m_pWS;
	// 始点关联的巷道
	AcDbVoidPtrArray m_pSptLines;
	// 末点关联的巷道
	AcDbVoidPtrArray m_pEptLines;
	// 关联的采空区
	Goaf* m_pGoaf;

	// 关联巷道信息
	JunctionEdgeInfoArray& m_sptLinesInfo;
	JunctionEdgeInfoArray& m_eptLinesInfo;
	// 关联的采空区信息
	AcGePoint3dArray m_polygon;
	int m_p1, m_p2;

	DumyJigEntity* m_pJigEnt;

	AcGePoint3d m_spt, m_ept;
	AcGeVector3d m_dir;
	AcGePoint3d m_basePt;
	double m_dist;
};
