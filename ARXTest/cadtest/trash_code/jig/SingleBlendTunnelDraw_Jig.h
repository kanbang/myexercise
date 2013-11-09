#pragma once

#include "../MineGE/MineGEDraw_Jig.h"
#include "SingleBlendTunnelDraw.h"

class SingleBlendTunnelDraw_Jig : public MineGEDraw_Jig
{
public:
	SingleBlendTunnelDraw_Jig(const CString& geType, const CString& drawName);
protected:
	virtual Adesk::Boolean doJig(MineGEDraw* pMineGEDraw);

	virtual DragStatus sampler() ;
	virtual Adesk::Boolean update() ;
	virtual AcDbEntity* entity() const ;

private:
	DragStatus getNextPoint();    // 动态获取下一点

	AcGePoint3d m_prevPt; // 上一次点

	SingleBlendTunnelDraw *m_pDraw;
};
