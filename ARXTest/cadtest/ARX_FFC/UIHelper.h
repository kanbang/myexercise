#pragma once

class UIHelper
{
public:
	static void CreateDataDlgManager();
	static void DestroyDataDlgManager();
	static void LoadData(const AcDbObjectId& objId);

	static void ShowGoafGasDlg();
	static void ShowFFCDlg();
};

