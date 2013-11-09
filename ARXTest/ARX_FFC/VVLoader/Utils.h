#pragma once

extern CString GetAppPathDir();
extern CString BuildPath(const CString& dir, const CString& fileName);
//extern CString BuildArxFileName(const CString& arxModuleName);
//extern CString BuildServiceName(const CString& arxModuleName);

extern bool loadArxModule(const CString& arxModuleName);
extern void unloadArxModule(const CString& arxModuleName);

//extern bool AddSupportPath(const CString& arxPath);
//extern bool RemoveSupportPath(const CString& arxPath);