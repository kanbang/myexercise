#pragma once

extern bool GetWordExePathFromRegistry(CString& path);
extern void OpenWordDocument(const CString& filePath);

extern CString String2Time(const CString& v);
extern int Access2Bool(int value);
