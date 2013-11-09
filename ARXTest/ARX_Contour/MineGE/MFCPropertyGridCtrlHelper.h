#pragma once

#include "dlimexp.h"

typedef bool (*ClickPropModifyCallBack)(const CString& oldValue, CString& newValue);

class CMFCPropertyGridCtrl;

// PropertyGridCtrl辅助类
class MINEGE_DLLIMPEXP MFCPropertyGridCtrlHelper
{
public:
	MFCPropertyGridCtrlHelper(CMFCPropertyGridCtrl* m_pWndPropList);

	// 添加单个字符串属性
	void addStringProperty(const CString& name, const CString& value, LPCTSTR descr=NULL, bool bEnable=true);

	// 添加连续区间[minValue, maxValue]之间的单个整数/浮点数属性
	void addIntProperty(const CString& name, int value, int nMinValue, int nMaxValue, LPCTSTR descr=NULL, bool bEnable=true);
	void addDoubleProperty(const CString& name, double value, double dMinValue, double dMaxValue, unsigned short precise=6, LPCTSTR descr=NULL, bool bEnable=true);

	// 添加布尔值属性
	void addBoolProperty(const CString& name, bool value, LPCTSTR descr=NULL, bool bEnable=true);

	// 添加日期属性
	void addDateTimeProperty(const CString& name, const COleDateTime& value, LPCTSTR descr = NULL, bool bEnable=true);

	// 添加离散的字符串属性列表
	void addStringPropertyList(const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr=NULL, bool bEnable=true);

	// 添加离散的(整数<--字符串)属性列表
	// 对于一些固定的数据, 一般使用枚举(整数)表示
	// 为了提高可读性, 在界面上一般用解释性的字符串映射到枚举	
	// 本程序对CMFCPropertyGridProperty进行了简单的派生
	// 使得外部显示的是字符串, 内部的数据是整数的
	void addIntPropertList(const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR descr=NULL, bool bEnable=true);
	
	void addDataObjectPropertyList(const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr=NULL, bool bEnable=true);

	// ***添加点击类属性(暂不使用)***
	void addClickProperty(const CString& name, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR descr, bool bEnable);

private:
	CMFCPropertyGridCtrl* m_pWndPropList;
};
