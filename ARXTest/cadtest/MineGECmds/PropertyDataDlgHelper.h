#pragma once

class PropertyDataDlgHelper
{
public:
	// ��ʾ"���ԶԻ���"(��ʾȫ������)
	static void DisplayAllPropertyDataDlg(const AcDbObjectId& objId);

	// ��ʾ"���ԶԻ���"(��ʾ��������)
	static void DisplayPartialPropertyDataDlg(const AcDbObjectId& objId, const AcStringArray& fields);

	// ͨ�����ԶԻ����Զ��ж���ͼԪ�������ݶ���
	static bool DisplayPropertyDataDlg(const AcDbObjectId& objId);
};
