#pragma once

/*
 * ARX模块加载/卸载器
 * 加载ARX模块需要指定一个服务名称(Service Name)
 *
 * 例如：DrawVentCmd项目
 *  // 1) 在acrxEntryPoint.cpp中定义注册服务名称
	#ifndef DRAW_VENT_CMD_SERVICE_NAME
	#define DRAW_VENT_CMD_SERVICE_NAME _T("DRAWVENTCMD_SERVICE_NAME")
	#endif

	// 2) 在acrxEntryPoint.cpp中的On_kInitAppMsg()方法中注册服务名称
	acrxRegisterService(DRAW_VENT_CMD_SERVICE_NAME);

	// 3) 在acrxEntryPoint.cpp中的On_kUnloadAppMsg()方法中删除服务名称
	delete acrxServiceDictionary->remove(DRAW_VENT_CMD_SERVICE_NAME);

	// 4) 加载ARX模块的代码片段
	CString arxName = _T("DrawVentCmd.arx");  // 相对路径或绝对路径
	CString serviceName = DRAW_VENT_CMD_SERVICE_NAME;

	// 判断服务名称是否已经注册
	AcRxObject *pSvc = acrxServiceDictionary->at (serviceName);
	if(pSvc == NULL)
	{
		// 服务尚未注册，加载arx模块
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

	bool loadLibrary();                                    // 加载ARX模块
	void releaseLibrary();                                 // 卸载ARX模块
	void* getFunction(const CString& functionName);        // 查找ARX模块中导出函数地址

private:
	CString m_arxFilePath;                                 // ARX模块路径
	CString m_serviceName;                                 // ARX模块的注册服务名称
};