#pragma once

#include "dlimexp.h"

/* 前向声明 */
class MineGEDraw;
class MineGEPluginManager;
class MineGEDrawManager;

/* 
 * 可视化系统，定义了一个全局可视化系统对象
 * 采用单件模式(Singleton Pattern)实现
 * 只能通过静态方法GetInstance()得到对象指针
 */
class MINEGE_DRAW_DLLIMPEXP MineGEDrawSystem
{
public:
	/* 创建全局可视化系统对象 */
	static MineGEDrawSystem* CreateInstance();
	/* 获取全局可视化系统对象 */
	static MineGEDrawSystem* GetInstance();
	/* 销毁全局可视化系统对象 */
	static void DestroyInstance();

	// 插件功能
public:
	// 加载可视化效果插件
	void loadPlugin(const CString &arxFilePath);

	// 卸载可视化效果插件
	void unloadPlugin(const CString &arxFilePath);

	// 可视化效果功能
public:
	// 获取图元的默认可视化效果
	MineGEDraw* getDefaultGEDraw(const CString& geType);

	// 获取图元的默认可视化效果
	CString getDefaultGEDrawName(const CString& geType);

	// 获取图元的指定可视化效果的对象指针
	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	// 设置图元的当前可视化效果
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// 获取图元的所有可视化效果(用于遍历)
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

private:
	/* 声明一个全局的静态对象指针，指向可视化系统对象 */
	static MineGEDrawSystem* m_pMineGEService;

	/* 
	 * 禁用构造函数和析构函数
	 * 开发人员只能通过静态方法来访问全局可视化对象 
	 */
	MineGEDrawSystem(void);
	~MineGEDrawSystem(void);

private:
	MineGEPluginManager* m_pMineGEPluginManager;
	MineGEDrawManager* m_pMineGEDrawManager;
};
