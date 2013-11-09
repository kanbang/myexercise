#pragma once

#include "aced.h"

// 用于处理cad退出的时候，卸载cui文件
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
