#pragma once

#include "resource.h"

#include "../MineGE/DataLink.h"

class ShaftExtraPropertyDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	int sp;  // ��������
};

class ShaftExtraPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(ShaftExtraPropertyDlg)

public:
	ShaftExtraPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ShaftExtraPropertyDlg();

// �Ի�������
	enum { IDD = IDD_SHAFT_EXTRA_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLiftUsageRadio();        // ������;�л��¼�
	afx_msg void OnBnClickedVentOutRadio();
	afx_msg void OnBnClickedLiftCoalRadio();
	afx_msg void OnBnClickedLiftMaterialRadio();
	afx_msg void OnBnClickedOk();

public:
	void setRelatedShaft(const AcDbObjectId& objId);  // ����һ����ͲShaft

private:
	int m_liftUsage;              // ��ʼ��Ϊ0(��������;)
	int m_liftCoalMethod;         // ��ʼ��Ϊ0(��������)

	void switchExtraProperty1(bool flag);
	void switchExtraProperty2(bool flag);
	void readExtraPropertyFromShaft();    // �Ӿ�Ͳ�ж�ȡ��������
	void readExtraProperty1(int value);    // ��ȡ���ݲ����µ���������1�Ŀؼ���
	void readExtraProperty2(int value);    // ��ȡ���ݲ����µ���������2�Ŀؼ���

	void writeExtraPropertyToShaft();     // �����º������д�ؾ�Ͳ��
	int caclLiftUsage();
	int caclLiftCoalMethod();
	int caclExtraPropety1();
	int caclExtraPropety2();
	CButton* getButtonCtrl(long control_id);

	ShaftExtraPropertyDataLink* pSEPM;
};
