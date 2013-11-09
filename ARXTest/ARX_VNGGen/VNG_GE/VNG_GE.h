#pragma once

#include "dlimexp.h"
#include "dbmain.h"

// ����ͼͼԪ����
class VNG_GE_DLLIMPEXP VNG_GE : public AcDbEntity 
{
public:
	ACRX_DECLARE_MEMBERS(VNG_GE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	virtual ~VNG_GE () ;

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	VNG_GE () ;

public:
	int m_id;  // ���(���ں��ڰ������õ�)
} ;

#ifdef VNG_GE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(VNG_GE)
#endif
