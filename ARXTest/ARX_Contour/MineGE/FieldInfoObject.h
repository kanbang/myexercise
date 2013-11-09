#pragma once

#include "FieldInfo.h"

// �ֶ���Ϣ������(�洢�ڴʵ�PROPERTY_DATA_FIELD_INFO_DICT��)
class FieldInfoObject : public AcDbObject 
{
public:
	ACRX_DECLARE_MEMBERS(FieldInfoObject) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	FieldInfoObject () ;
	virtual ~FieldInfoObject () ;

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

public:
	FieldInfo m_info;
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(FieldInfoObject)
#endif
