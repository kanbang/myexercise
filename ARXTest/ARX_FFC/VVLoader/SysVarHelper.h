#pragma once

class SysVarHelper
{
public:
	// �޸�/��ԭϵͳ����
	static void ModifyTolerance();
	static void RestoreTolerace();

	// �޸�/��ԭϵͳ���� -- SELECTPREVIEW
	static void ModifySelectPreview();
	static void RestoreSelectPreview();

	// �޸�/��ԭϵͳ���� -- PICKADD
	static void ModifyPickAdd();
	static void RestorePickAdd();
};
