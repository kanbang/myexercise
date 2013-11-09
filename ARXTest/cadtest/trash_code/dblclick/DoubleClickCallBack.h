#pragma once

#include "dlimexp.h"

// 双击回调(Call Back)函数签名定义
typedef void (*DoubleClickFunc)(const AcDbObjectId& objId);

extern MINEGE_DLLIMPEXP void RegDoubleClickFunc(DoubleClickFunc dcf);

#define REG_DOUBLE_CLICK_FUNC(dcf) \
	struct RegDoubleClickFunc_##dcf##_tag \
	{ \
		RegDoubleClickFunc_##dcf##_tag() \
		{\
			RegDoubleClickFunc(&dcf); \
		}\
	};\
	RegDoubleClickFunc_##dcf##_tag global_##RegDoubleClickFunc_##dcf##_tag;