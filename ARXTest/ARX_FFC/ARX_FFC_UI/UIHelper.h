#pragma once

class UIHelper
{
public:
	static void RegAppMenu();
	static void UnRegAppMenu();

	static void RegAppToolBar();
	static void UnRegAppToolBar();

	static void LoadCui();
	static void UnloadCui();

	static void RegObjectContextMenu(const void *appId);
	static void UnRegObjectContextMenu();
};
