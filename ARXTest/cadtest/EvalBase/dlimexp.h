#pragma once

#ifdef EVALBASE_MODULE
#define EVAL_BASE_EXPORT_API _declspec(dllexport)
#else
#define EVAL_BASE_EXPORT_API _declspec(dllimport)
#endif