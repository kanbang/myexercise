#pragma once

#include "AcDblClkEdit.h"

class DblClick : public AcDbDoubleClickEdit
{
public:
	DblClick(void);
	~DblClick(void);

	virtual void startEdit(AcDbEntity *pEnt, AcGePoint3d pt);
	virtual void finishEdit(void);
};
