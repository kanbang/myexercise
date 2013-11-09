#pragma once

#include "aced.h"

// 监视被选择的图元(PickSet)的变化
class SelectedGE_EditorReactor : public AcEditorReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	SelectedGE_EditorReactor (const bool autoInitAndRelease =true) ;
	virtual ~SelectedGE_EditorReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// 监视PickSet的变化
	virtual void pickfirstModified(void);
} ;
