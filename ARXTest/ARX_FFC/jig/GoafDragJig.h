#pragma once

#include "../DefGE/Goaf.h"

class GoafDragJig : public AcEdJig  
{
public:
	GoafDragJig(const AcDbObjectId objId, int pos);
	virtual ~GoafDragJig();
	void doIt();

	virtual AcDbEntity * entity() const;
	virtual Adesk::Boolean update();
	virtual AcEdJig::DragStatus  sampler();

private:
	AcDbObjectId m_objId;
	Goaf* m_pGoaf;
	AcGePoint3dArray m_polygon;

	int m_pos;
	AcGePoint3d basePt, curPt;
};
