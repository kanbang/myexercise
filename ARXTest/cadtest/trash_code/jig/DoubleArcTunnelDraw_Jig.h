#pragma once

#include "../MineGEDraw/MineGEDraw_Jig.h"
#include "DoubleArcTunnelDraw.h"

class DoubleArcTunnelDraw_Jig : public MineGEDraw_Jig
{
public:
	DoubleArcTunnelDraw_Jig(const CString& geType, const CString& drawName);

protected:
	virtual Adesk::Boolean doJig(MineGEDraw* pMineGEDraw);

	virtual DragStatus sampler() ;
	virtual Adesk::Boolean update() ;
	virtual AcDbEntity* entity() const ;

private:
	DragStatus getEndPoint();       // 动态获取点坐标
	DragStatus getSecondPoint();    // 动态获取第2点
	enum GET_STATE { EPOINT, SEPOINT }; // 当前动态获取的数据（末点坐标、第2点）
	GET_STATE gs;

	AcGePoint3d m_secondPt; // 第2点，临时数据
	DoubleArcTunnelDraw *m_pDraw;
};
