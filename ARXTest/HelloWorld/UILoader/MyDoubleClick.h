#pragma once

#include "aced.h"

class MyDoubleClick : public AcEditorReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	MyDoubleClick (const bool autoInitAndRelease =true) ;
	virtual ~MyDoubleClick () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// -----------------------------------------------------------------------------
	virtual void beginDoubleClick(const AcGePoint3d & clickPoint);
} ;
