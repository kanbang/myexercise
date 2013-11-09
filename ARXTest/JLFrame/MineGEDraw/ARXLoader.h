#pragma once

/*
 * ARXģ�����/ж����
 * ����ARXģ����Ҫָ��һ����������(Service Name)
 *
 * ���磺DrawVentCmd��Ŀ
 *  // 1) ��acrxEntryPoint.cpp�ж���ע���������
	#ifndef DRAW_VENT_CMD_SERVICE_NAME
	#define DRAW_VENT_CMD_SERVICE_NAME _T("DRAWVENTCMD_SERVICE_NAME")
	#endif

	// 2) ��acrxEntryPoint.cpp�е�On_kInitAppMsg()������ע���������
	acrxRegisterService(DRAW_VENT_CMD_SERVICE_NAME);

	// 3) ��acrxEntryPoint.cpp�е�On_kUnloadAppMsg()������ɾ����������
	delete acrxServiceDictionary->remove(DRAW_VENT_CMD_SERVICE_NAME);

	// 4) ����ARXģ��Ĵ���Ƭ��
	CString arxName = _T("DrawVentCmd.arx");  // ���·�������·��
	CString serviceName = DRAW_VENT_CMD_SERVICE_NAME;

	// �жϷ��������Ƿ��Ѿ�ע��
	AcRxObject *pSvc = acrxServiceDictionary->at (serviceName);
	if(pSvc == NULL)
	{
		// ������δע�ᣬ����arxģ��
		if( !acrxDynamicLinker->loadModule(arxName, false) )
		{
			return false;
		}
	}
 */
class ArxLoader
{
public:
	ArxLoader(const CString& arxFilePath, const CString& serviceName);

	bool loadLibrary();                                    // ����ARXģ��
	void releaseLibrary();                                 // ж��ARXģ��
	void* getFunction(const CString& functionName);        // ����ARXģ���е���������ַ

private:
	CString m_arxFilePath;                                 // ARXģ��·��
	CString m_serviceName;                                 // ARXģ���ע���������
};