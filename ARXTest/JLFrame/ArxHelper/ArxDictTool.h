#pragma once

#include "dlimexp.h"

// ��Ч��entry����
#define INVALID_ENTRY 0

/*
 * CAD�ʵ������������
 * AcDbDictionay��key���Ǵ�Сд���е�
 * ����NAME, name, Name, naMe�ȶ���һ����
 * 
 * һ��key->AcDbXRecord(��¼���������ܶ�entry��)
 * AcDbXRecordʵ����һ��resbuf list, ���Ա���һ�����ݣ�
 * ����ʹ��AcDbXRecord����һЩ�򵥵��ı�������(����ֻ����1��entry��Ҳ���԰������entry)
 *
 * ʾ����
 * ʹ�ôʵ䱣���ֶ�
 *	   Tunnel       -->  (���� ���� ����  ���� �ֲ����� Ħ��ϵ�� ...)
 *	   Fan          -->  (���� �������״̬ a0 a1 a2 a3 a4 ...)
 *	   Gate         -->  (���� ©���� ѹ�� ...)
 */

class ARXHELPER_DLLIMPEXP ArxDictTool
{
public:
	/* �򵥵Ĺ��췽��������ʵ�����ƣ�����һ��ArxDictTool����ָ�� 
	 * ǰ�᣺�ôʵ���������ݿ��У���������ڣ�Ӧ����ʹ��
	 * ArxDataTool::RegDict()����ע��ʵ�
	 *
	 * �÷�����ֱ��ʹ��new��һ����
	 * ArxDictTool* pDictTool = ArxDictTool::GetDictTool(_T("test"));
	 * �ȼ���
	 * ArxDictTool* pDictTool = new ArxDictTool(_T("test"));
	 */
	static ArxDictTool* GetDictTool(const CString& dictName);

	/* �ж�key�Ƿ���� */
	bool findKey(const CString& key);
	/* ɾ��key�Լ�key��Ӧ�����е����� */
	void removeKey(const CString& key);
	/* ��ȡ�ʵ��е�����key */
	void getAllKeys(AcStringArray& keys);

	/* ���entry,���key�����ڣ����Զ���� */
	int addEntry(const CString& key, const CString& entry);
	/* ɾ��entry */
	int removeEntry(const CString& key, const CString& entry);
	/* ����entry������λ�� */
	int findEntry(const CString& key, const CString& entry);
	/* ��ȡkey�����ĵ�����entry */
	void getAllEntries(const CString& key, AcStringArray& entries);
	/* ͳ��key����������entry���� */
	int countEntries(const CString& key);
	
	/* �޸�key�е�index���ֵ
	 * ע��index����Ҫ����0
	 */
	bool modifyEntry(const CString& key, int index, const CString& newEntry);
	/* ��ȡkey�е�index���ֵ
	 * ע��index����Ҫ����0
	 */
	bool getEntry(const CString& key, int index, CString& entry);

private:
	ArxDictTool(const CString& dictName);
	/* Ҫ�����Ĵʵ����� */
	CString m_dictName;
};

/* CAD�ʵ������
 * һ��key->AcDbObject(���ߴ�AcDbObjectֱ����������������AcDbEntityͼ��ʵ�����)
 */
class ARXHELPER_DLLIMPEXP ArxDictTool2
{
public:
	static ArxDictTool2* GetDictTool(const CString& dictName);

	// ��ӵ��ʵ��У�ͬʱ�Զ��رն���pObj->close();
	bool addEntry(const CString& key, AcDbObject* pObj);         // ���
	bool removeEntry(const CString& key);                        // ɾ��
	bool findEntry(const CString& key, AcDbObjectId& objId);     // ����
	void getAllKeys(AcStringArray& keys);
	void getAllEntries(AcDbObjectIdArray& objIds);               // ��ȡ���еĶ���

private:
	ArxDictTool2(const CString& dictName);
	CString m_dictName;
};

class ARXHELPER_DLLIMPEXP ArxDictHelper
{
public:
	static void GetAllKeys(const CString& dictName, AcStringArray& keys);
	static bool GetAllEntries(const CString& dictName, const CString& key, AcStringArray& entries);
	static bool AddEntry(const CString& dictName, const CString& key, const CString& entry);
	static void RemoveAllEntries(const CString& dictName, const CString& key);
	static bool RemoveEntry(const CString& dictName, const CString& key, const CString& entry);
};