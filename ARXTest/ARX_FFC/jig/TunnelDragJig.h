#pragma once

#include "../DefGE/Tunnel.h"

class TunnelDragJig : public AcEdJig  
{
public:
	TunnelDragJig(const AcDbObjectId objId);
	virtual ~TunnelDragJig();
	void doIt();

	virtual AcDbEntity * entity() const;
	virtual Adesk::Boolean update();
	virtual AcEdJig::DragStatus  sampler();

private:
	AcDbObjectId m_objId;
	Tunnel* m_pWS;
	AcGePoint3d spt, ept;
	AcGePoint3d basePt, curPt;
};
