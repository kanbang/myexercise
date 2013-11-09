#pragma once

#ifdef DAOHELPER_EXPORTS
#define DAO_HELPER_EXPORT_API _declspec(dllexport)
#else
#define DAO_HELPER_EXPORT_API _declspec(dllimport)
#endif