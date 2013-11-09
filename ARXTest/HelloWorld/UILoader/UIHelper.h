#pragma once

class UIHelper
{
public:
	static void RegisterAppMenu();
	static void UnRegisterAppMenu();

	static void RegisterAppToolBar();
	static void UnRegisterAppToolBar();

	static void RegisterObjectContextMenu(const void *appId);
	static void UnRegisterObjectContextMenu();
};
