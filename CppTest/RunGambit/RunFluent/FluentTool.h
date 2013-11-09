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

extern CString GetTecplotType(TECPLOT_SHOW_TYPE type);