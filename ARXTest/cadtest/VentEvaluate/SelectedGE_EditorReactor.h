#pragma once

#include "aced.h"

class SelectedGE_EditorReactor : public AcEditorReactor {

protected:
	bool mbAutoInitAndRelease ;

public:
	SelectedGE_EditorReactor (const bool autoInitAndRelease =true) ;
	virtual ~SelectedGE_EditorReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// ����PickSet�ı仯
	virtual void pickfirstModified(void);
};