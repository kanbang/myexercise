#pragma once

#include "dbmain.h"

// �������һ��LinkedGEͼԪ��ı仯
class LinkedGEAppendReactor : public AcDbDatabaseReactor {

protected:
	//----- Pointer to the document this reactor instance belongs to.
	AcDbDatabase *mpDatabase ;

public:
	LinkedGEAppendReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~LinkedGEAppendReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	virtual void objectAppended(const AcDbDatabase* db, const AcDbObject* pObj);
} ;