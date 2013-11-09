#pragma once

class SysVarHelper
{
public:
	// 修改/还原系统精度
	static void ModifyTolerance();
	static void RestoreTolerace();

	// 修改/还原系统变量 -- SELECTPREVIEW
	static void ModifySelectPreview();
	static void RestoreSelectPreview();

	// 修改/还原系统变量 -- PICKADD
	static void ModifyPickAdd();
	static void RestorePickAdd();
};
