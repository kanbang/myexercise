#pragma once

#include "dlimexp.h"

class ARXCUI_DLLIMPEXP CUIHelper
{
public:
	// 使用命令方式存在的问题
	// 1) 如果在kLoadAppMsg消息中调用该方法,菜单不能显示,只能显示工具栏
	// 2) cad直接退出的时候不能卸载菜单
	//	 2.1) acedCommand执行无效失败
	//	 2.2) sendStringToExecute方法执行成功，但还是无法卸载菜单
	// 3) 在非application上下文下
	//	 3.1) acedCommand执行有效
	//	 3.2) sendStringToExecute执行会显示cuiload对话框(filedia=0变量不起作用)
	static bool UnLoadPartialMenuUseCmd(const CString& menuGroupName);
	static bool LoadPartialMenuUseCmd(const CString& filePath, const CString& menuGroupName);

	// 使用com方式存在的问题
	// 1) cad直接退出的时候不能卸载菜单
	// 2) 使用com卸载cui的时候，菜单和工具栏会消失，
	//    但在acad.cuix(主自定义文件)中仍然保留了记录
	// 猜测原因：使用com方式无法影响到acad.cuix
	static bool UnLoadPartialMenuUseCom(const CString& menuGroupName);
	static bool LoadPartialMenuUseCom(const CString& filePath, const CString& menuGroupName);
};
