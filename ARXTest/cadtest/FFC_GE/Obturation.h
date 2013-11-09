#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// √‹±’
class FFC_GE_EXPORT_API Obturation : public DirectionTagGE 
{
public:
	ACRX_DECLARE_MEMBERS(Obturation) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	Obturation();
	Obturation(const AcGePoint3d& pt, double angle);
	virtual ~Obturation();

private:
	AcGePoint3d m_pt;
	double m_angle;
} ;

#ifdef FFC_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Obturation)
#endif
