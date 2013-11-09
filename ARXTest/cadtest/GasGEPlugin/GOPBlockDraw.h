#pragma once

#include "../MineGEDraw/BlockDraw.h"

class GOPBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(GOPBlockDraw) ;

public:
	virtual void regPropertyDataNames(AcStringArray& names) const;
	// ����������ĩβ��ӵ�λ��Ч
	// ԭ��:������Զ��岻��ʶ��ת���±�������ַ���
	//virtual void readPropertyDataFromGE(const AcStringArray& values);

protected:
	GOPBlockDraw();
};

class SmallGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(SmallGOPBlockDraw) ;

public:
	SmallGOPBlockDraw();
};

class MidGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(MidGOPBlockDraw) ;

public:
	MidGOPBlockDraw();
};

class LargeGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(LargeGOPBlockDraw) ;

public:
	LargeGOPBlockDraw();
};

class LargerGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(LargerGOPBlockDraw) ;

public:
	LargerGOPBlockDraw();
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SmallGOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MidGOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LargeGOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LargerGOPBlockDraw)
#endif
