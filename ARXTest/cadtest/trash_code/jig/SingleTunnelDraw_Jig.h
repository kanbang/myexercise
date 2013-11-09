#pragma once

#include "../MineGEDraw/MineGEDraw_Jig.h"
#include "SingleTunnelDraw.h"

class SingleTunnelDraw_Jig : public MineGEDraw_Jig
{
public:
	SingleTunnelDraw_Jig(const CString& geType, const CString& drawName);

protected:
	virtual Adesk::Boolean doJig(MineGEDraw* pMineGEDraw);

	virtual DragStatus sampler() ;
	virtual Adesk::Boolean update() ;
	virtual AcDbEntity* entity() const ;

private:
	DragStatus getEndPoint(); // 动态获取点坐标

	SingleTunnelDraw *m_pDraw;
};
