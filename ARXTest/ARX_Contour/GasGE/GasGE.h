#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

// ��˹��ص�ͼԪ����
class GASGE_EXPORT_API GasGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(GasGE) ;

protected:
	GasGE(void);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasGE)
#endif
