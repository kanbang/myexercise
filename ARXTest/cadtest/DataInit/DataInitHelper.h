#pragma once

/*
 * �ֶμ����ݳ�ʼ��������
 * ���ļ��ж�ȡ�ֶκ�����
 */
class DataInitHelper
{
public:
	static void InitDataField(const CString& fileName);

	static void InitStrList(const CString& fileName);
	static void InitIntStrList(const CString& fileName);
	static void InitDataObjectList(const CString& fileName);
	static void InitSingleDataObject(const CString& fileName);
};
