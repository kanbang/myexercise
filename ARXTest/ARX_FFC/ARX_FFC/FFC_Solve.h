#pragma once

#include "FFC_Def.h"

extern GEO_ERROR_CODE GenGeometryAndData(const CString& appDir);
extern RUN_ERROR_CODE RunGambit(const CString& appDir, HANDLE& hThread);
extern RUN_ERROR_CODE RunFluent(const CString& appDir, HANDLE& hThread);
extern RUN_ERROR_CODE RunTecplot(const CString& appDir, int type);
extern void CleanWork(const CString& appDir);

extern bool IsRunGambitOk(const CString& appDir);
extern bool IsRunFluentOk(const CString& appDir);
