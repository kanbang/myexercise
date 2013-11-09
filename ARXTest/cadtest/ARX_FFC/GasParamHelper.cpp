#include "StdAfx.h"
#include "GasParamHelper.h"
#include "GasDict.h"

#include "../ArxHelper/ArxDataTool.h"
#include "../ArxHelper/ArxDictTool.h"
#include "../ArxHelper/ArxUtilHelper.h"

#define ZERO_GAS _T("0")

void GasParamHelper::InitParams()
{
    // 词典存在
    if( ArxDataTool::IsDictExist( GAS_PARAM_DICT ) ) return;

    ArxDataTool::RegDict( GAS_PARAM_DICT );

    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( GAS_PARAM_DICT );

    pDictTool->addEntry( GAS_F1_KEY, ZERO_GAS );
    pDictTool->addEntry( GAS_F2_KEY, ZERO_GAS );
    pDictTool->addEntry( GAS_F3_KEY, ZERO_GAS );
    pDictTool->addEntry( GAS_F4_KEY, ZERO_GAS );
    pDictTool->addEntry( GAS_F5_KEY, ZERO_GAS );

    delete pDictTool;
}

void GasParamHelper::ReadGasParam( GasParam& gp )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( GAS_PARAM_DICT );

    CString str;

    /* 读取图形参数 */
    pDictTool->getEntry( GAS_F1_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.f1 );

    pDictTool->getEntry( GAS_F2_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.f2 );

    pDictTool->getEntry( GAS_F3_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.f3 );

    pDictTool->getEntry( GAS_F4_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.f4 );

    pDictTool->getEntry( GAS_F5_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.f5 );

    delete pDictTool;
}

void GasParamHelper::WriteGasParam( const GasParam& gp )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( GAS_PARAM_DICT );

    CString str;

    ArxUtilHelper::DoubleToString( gp.f1, str );
    pDictTool->modifyEntry( GAS_F1_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.f2, str );
    pDictTool->modifyEntry( GAS_F2_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.f3, str );
    pDictTool->modifyEntry( GAS_F3_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.f4, str );
    pDictTool->modifyEntry( GAS_F4_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.f5, str );
    pDictTool->modifyEntry( GAS_F5_KEY, 1, str );

    delete pDictTool;
}