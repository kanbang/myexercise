#pragma once

extern CString GetAppPathDir();
extern CString BuildPath(const CString& dir, const CString& fileName);
extern CString GetPathDir(const CString& path);

extern bool CreateEmptyFile(const CString& path);
extern void RemoveFile(const CString& filePath);
extern bool IsFileExist(const CString& filePath);
extern CDaoDatabase* CreateDB(const CString& dbName, bool openExist = false);