#pragma once

#include "DockBarChildDlg.h"

class PropertyData_DockBarChildDlg : public DockBarChildDlg 
{
	DECLARE_DYNAMIC (PropertyData_DockBarChildDlg)

public:
	~PropertyData_DockBarChildDlg();

protected:
	PropertyData_DockBarChildDlg(UINT idd, CWnd *pParent =NULL, HINSTANCE hInstance = NULL) ;

	virtual void readPropertyData() {}
	virtual void writePropertyData() {}
	
public:
	AcDbObjectId m_objId;
	void updatePropertyData(bool save);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnCancel();
} ;
