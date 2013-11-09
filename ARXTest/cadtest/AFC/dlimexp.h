#pragma once

#ifdef AFC_EXPORTS
	#define AFC_EXPORT_API _declspec(dllexport)
#else
	#define AFC_EXPORT_API _declspec(dllimport)
#endif