#pragma once

// http://blog.csdn.net/sunnyboychina/archive/2006/04/13/661763.aspx
class CIniFile
{
public:
	CIniFile();
	virtual ~CIniFile();

	//    ����ini�ļ�·��
	//    �ɹ�����TRUE;���򷵻�FALSE
	BOOL         SetPath(const CString& strPath);

	//    ���section�Ƿ����
	//    ���ڷ���TRUE;���򷵻�FALSE
	BOOL SectionExist(const CString& strSection);

	//    ��ָ����Section��Key��ȡKeyValue
	//    ����KeyValue
	CString GetKeyValue(const CString& strSection, const CString& strKey);        

	//    ����Section��Key�Լ�KeyValue����Section����Key�������򴴽�

	void SetKeyValue(const CString&    strSection,
		             const CString&    strKey,
					 const CString&    strKeyValue);

	//    ɾ��ָ��Section�µ�һ��Key
	void DeleteKey(const CString& strSection, const CString& strKey);

	//    ɾ��ָ����Section�Լ����µ�����Key
	void DeleteSection(const CString& strSection);

	//    ������е�Section
	//    ����Section��Ŀ
	int  GetAllSections(CStringArray& strArrSection);

	//    ����ָ��Section�õ����µ�����Key��KeyValue
	//    ����Key����Ŀ

	int GetAllKeysAndValues(const CString& strSection,
		                    CStringArray& strArrKey,
							CStringArray& strArrKeyValue);

	// ɾ������Section
	void DeleteAllSections();

private:
	// ini�ļ�·��
	CString m_strPath;
};