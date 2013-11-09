#pragma once

#include "aced.h"

/*
 * ���ڴ���LinkedGEͼԪ�ıպ�����
 * ���磬�����ִ���ƶ��������ƶ���
 * �������ԭʼλ�õ����ӵ�պ�
 * �Լ������������λ�õ����ӵ�պ�
 */
class LinkedGE_EditorReactor : public AcEditorReactor {

protected:
	bool mbAutoInitAndRelease ;

public:
	LinkedGE_EditorReactor (const bool autoInitAndRelease =true) ;
	virtual ~LinkedGE_EditorReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// ������ִ�н����������ӵ�պ�
	virtual void commandEnded(const ACHAR * cmdStr);
} ;
