#pragma once

class PropertyDataDlgHelper
{
public:
	// 显示"属性对话框"(显示全部数据)
	static void DisplayAllPropertyDataDlg(const AcDbObjectId& objId);

	// 显示"属性对话框"(显示部分数据)
	static void DisplayPartialPropertyDataDlg(const AcDbObjectId& objId, const AcStringArray& fields);

	// 通用属性对话框，自动判断是图元还是数据对象
	static bool DisplayPropertyDataDlg(const AcDbObjectId& objId);
};
