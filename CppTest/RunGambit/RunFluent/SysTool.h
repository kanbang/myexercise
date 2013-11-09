#pragma once

extern CString GetAppPathDir();
extern CString BuildPath(const CString& dir, const CString& fileName);
extern void RemoveFile(const CString& filePath);
extern bool IsFileExist(const CString& filePath);
extern void CreateDataDirectory(const CString& data_dir);
extern void KillProcess(const CString& name);
