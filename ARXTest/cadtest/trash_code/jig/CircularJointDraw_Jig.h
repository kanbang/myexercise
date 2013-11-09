#pragma once

#include "../MineGEDraw/MineGEDraw_Jig.h"
#include "CircularJointDraw.h"

class CircularJointDraw_Jig : public MineGEDraw_Jig 
{
public:
	CircularJointDraw_Jig (const CString& geType, const CString& drawName) ;

protected:
	virtual Adesk::Boolean doJig(MineGEDraw* pMineGEDraw);

	//- input sampler
	virtual DragStatus sampler() ;

	//- jigged entity update
	virtual Adesk::Boolean update() ;

	virtual AcDbEntity* entity() const ;

private:
	DragStatus getRadius(); // ¶¯Ì¬»ñÈ¡°ë¾¶

	CircularJointDraw *m_pDraw;
} ;
