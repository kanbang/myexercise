#pragma once

#include "aced.h"

// ���ӱ�ѡ���ͼԪ(PickSet)�ı仯
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

	// ����PickSet�ı仯
	virtual void pickfirstModified(void);
} ;
