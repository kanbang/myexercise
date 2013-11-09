#pragma once

class AcApDocument;

class ReactorHelper
{
public:
	static void CreateLinkedGE_EditorReactor();
	static void RemoveLinkedGE_EditorReactor();

	// �����ĵ���ص�reactor
	// ��2����������kLoadAppMsg��kUnloadAppMsg��Ϣ
	static void CreateDocumentReactorMap();
	static void RemoveDocumentReactorMap();

	// ��2����������kLoadDwgMsg��kUnloadDwgMsg��Ϣ
	static void AddDocumentReactor(AcApDocument* pDoc);
	static void RemoveDocumentReactor(AcApDocument* pDoc);
};
