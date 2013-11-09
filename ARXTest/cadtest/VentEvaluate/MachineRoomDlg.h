#pragma once

#include "Resource.h"

class MachineRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(MachineRoomDlg)

public:
	MachineRoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MachineRoomDlg();

// 对话框数据
	enum { IDD = IDD_MACHINE_ROOM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void initHotType(double c);
	void recordCoeff();
	void updateCoeff(int minR, int maxR, int v);
	void setCoeffText(int pos);

public:
	AcDbObjectId m_objId;
	double m_power;
	double m_deltaT;
	int m_hotType;
	CSliderCtrl m_hotSlider;

	// 发热系数对应的整数
	int cf1, cf2, cf3, cf4;

	// 读写机电硐室数据
	void readPropertyData();
	void writePropertyData();

	afx_msg void OnBnClickedMachineTypeRadio1();
	afx_msg void OnBnClickedMachineTypeRadio2();
	afx_msg void OnBnClickedMachineTypeRadio3();
	afx_msg void OnBnClickedMachineTypeRadio4();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
