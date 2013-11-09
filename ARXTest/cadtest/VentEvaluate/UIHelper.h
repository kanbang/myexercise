#pragma once

class UIHelper
{
public:
	static void ShowMineInfoDlg();
	static void ShowProjectInfoDlg();

	static void ShowDataInputDockBar();
	static void DestroyDataInputDockBar();

	static void ShowWindStationDockBar();
	static void DestroyWindStationDockBar();

	static void ShowInstrumentDlg();

	static void ShowEvalDlg();

	// 网络检查相关的对话框
	static void ShowCycleListDockBar();
	static void DestroyCycleListDockBar();

	static void ShowCCListDockBar();
	static void DestroyCCListDockBar();

	static void ShowVentShaftDockBar();
	static void DestroyVentShaftDockBar();

	static void ShowDataListDockBar();
	static void DestroyDataListDockBar();
};

