#pragma once

// ARXģ�������
class ArxLoader
{
public:
	ArxLoader(const CString& arxFilePath, const CString& serviceName);

	bool loadLibrary();                                    // ����ģ��
	void releaseLibrary();                                 // ж��ģ��
	void* getFunction(const CString& functionName);        // ����ARXģ���еĵ���export������ַ

private:
	CString m_arxFilePath; // arx�ļ�·��
	CString m_serviceName; // arxģ���ע���������
};