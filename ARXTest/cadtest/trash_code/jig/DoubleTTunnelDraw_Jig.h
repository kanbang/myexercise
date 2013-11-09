#pragma once

#include "../MineGE/MineGEDraw_Jig.h"
#include "DoubleTTunnelDraw.h"

class DoubleTTunnelDraw_Jig : public MineGEDraw_Jig
{
public:
	DoubleTTunnelDraw_Jig(const CString& geType, const CString& drawName);

protected:
	virtual Adesk::Boolean doJig(MineGEDraw* pMineGEDraw);

	//- input sampler
	virtual DragStatus sampler() ;

	//- jigged entity update
	virtual Adesk::Boolean update() ;

	virtual AcDbEntity* entity() const ;

private:
	DragStatus getEndPoint(); // 动态获取点坐标
	DragStatus getWidth();    // 动态获取巷道宽度
	enum GET_STATE { EPOINT, WIDTH }; // 当前动态获取的数据（末点坐标、巷道宽度）
	GET_STATE gs;

	DoubleTTunnelDraw *m_pDraw;
};
