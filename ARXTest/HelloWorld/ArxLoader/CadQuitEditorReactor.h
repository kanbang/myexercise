#pragma once

#include "aced.h"

// ���ڴ���cad�˳���ʱ��ж��cui�ļ�
class CadQuitEditorReactor : public AcEditorReactor {

protected:
	bool mbAutoInitAndRelease ;

public:
	CadQuitEditorReactor (const bool autoInitAndRelease =true) ;
	virtual ~CadQuitEditorReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	virtual void quitWillStart(void);
} ;
