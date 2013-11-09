#pragma once

#ifdef VNC_EXPORTS
#define VNC_EXPORT_API __declspec(dllexport)
#else
#define VNC_EXPORT_API __declspec(dllimport)
#endif
