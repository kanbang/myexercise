#pragma once

#include "DoubleTunnelDraw.h"

// �򵥵�˫�߾��������
class DoubleTTunnelDraw : public DoubleTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleTTunnelDraw);

public:
	DoubleTTunnelDraw(void);

protected:
	void drawText(AcGiWorldDraw *mode);
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleTTunnelDraw)
#endif