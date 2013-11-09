#pragma once

#include "PropertyData_DockBarChildDlg.h"

typedef AcArray<PropertyData_DockBarChildDlg*> DlgArray;

class DataDlgManager
{
public:
	DataDlgManager ();
	virtual ~DataDlgManager ();

	void regDlg(const CString& type, PropertyData_DockBarChildDlg* pDlg);
	void load(const AcDbObjectId& objId);

private:
	AcDbObjectId m_last_objId;
	int m_last_pos;
	
	AcStringArray m_types;
	DlgArray m_dlgArray;
} ;
