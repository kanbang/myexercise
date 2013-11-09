#pragma once

class AcApDocument;

class ReactorHelper
{
public:
	static void CreateCadQuitEditorReactor();
	static void RemoveCadQuitEditorReactor();

	static void CreateDocReactor();
	static void RemoveDocReactor();

	static void CreateDBReactor();
	static void RemoveDBReactor();
};

