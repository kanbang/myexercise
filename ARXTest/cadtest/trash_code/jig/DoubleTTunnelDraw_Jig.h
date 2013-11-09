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
	DragStatus getEndPoint(); // ��̬��ȡ������
	DragStatus getWidth();    // ��̬��ȡ������
	enum GET_STATE { EPOINT, WIDTH }; // ��ǰ��̬��ȡ�����ݣ�ĩ�����ꡢ�����ȣ�
	GET_STATE gs;

	DoubleTTunnelDraw *m_pDraw;
};
