#include "stdafx.h"

#include "config.h"
#include "CurDrawTool.h"
#include "DrawHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGEDraw/MineGEDrawSystem.h"

/*
 * 创建当前可视化效果词典
 * 参数types数组保存了当前系统中所有注册的图元类型
 */
static void BuildCurDrawsDict( const AcStringArray& types )
{
    // 获取可视化系统全局对象
    MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();
    if( pDrawSystem == 0 ) return;

    // 打开词典
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    int len = types.length();

    for( int i = 0; i < len; i++ )
    {
        // 字符串数组第i个元素
        CString type = types[i].kACharPtr();

        if( !pDictTool->findKey( type ) )
        {
            // 如果图元对应的key不存在，则向可视化系统请求该图元的默认可视化效果名称
            // 并添加"键-值"对到词典
            // 例如: type = Tunnel ==>  则添加(Tunnel --> DoubleTunnel)到词典
            pDictTool->addEntry( type, pDrawSystem->getDefaultGEDrawName( type ) );
        }
    }

    delete pDictTool;
}

// 设置当前可视化效果
void InitAllCurDraws()
{
    // 从可视化系统中读取所有已注册的图元类型
    AcStringArray types;
    DrawHelper::GetAllRegGETypesForDraw( types );
    if( types.isEmpty() ) return;

    // 创建当前可视化效果词典
    BuildCurDrawsDict( types );
}

bool SetCurDraw( const CString& type, const CString& draw )
{
    // 通过ArxHelper模块中ArxDictTool辅助类操纵词典

    // 打开词典
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    // 修改第1个位置的数据
    // 注：第1个位置的数据就是"当前可视化效果"
    bool ret = pDictTool->modifyEntry( type, 1, draw );
    delete pDictTool;

    return ret;
}

bool GetCurDraw( const CString& type, CString& draw )
{
    // 通过ArxHelper模块中ArxDictTool辅助类操纵词典

    // 打开词典
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );

    // 获取第1个位置的数据
    // 注：第1个位置的数据就是"当前可视化效果"
    bool ret = pDictTool->getEntry( type, 1, draw );
    delete pDictTool;

    return ret;
}