#pragma once

#include "FFC_Def.h"

extern bool ReadGambitBinPath(CString& path);
extern void CloseGambitStartupWindow();
extern bool ReadFluentBinPath(CString& path);

extern bool RunGambit(
	const CString& gambitExePath, 
	const CString& cwdPath, 
	const CString& jouFile, 
	HANDLE& hProcess, 
	HANDLE& hThread, 
	bool hidden=true);

extern bool RunFluent(
	const CString& fluentExePath, 
	const CString& cwdPath, 
	const CString& scmFile, 
	HANDLE& hProcess, 
	HANDLE& hThread, 
	bool hidden=true);

extern CString GetTecplotTypeName(TECPLOT_SHOW_TYPE type);
extern CString GetGambitErrorMessage(RUN_ERROR_CODE rst);
extern CString GetFluentErrorMessage(RUN_ERROR_CODE rst);
extern CString GetTecplotErrorMessage(RUN_ERROR_CODE rst);
extern CString GetGeoErrorMessage(GEO_ERROR_CODE ge);
