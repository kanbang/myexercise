#include "StdAfx.h"

#include "ContourParamHelper.h"
#include "ContourParamconfig.h"

#include "../ArxHelper/HelperClass.h"

void ContourParamHelper::InitParams()
{
    // �ʵ����
    if( ArxDataTool::IsDictExist( CONTOUR_PARAM_DICT ) ) return;

    ArxDataTool::RegDict( CONTOUR_PARAM_DICT );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CONTOUR_PARAM_DICT );

    CString str;
    ArxUtilHelper::IntToString( DEF_SMOOTH_TYPE, str );
    pDictTool->addEntry( CONTOUR_SMOOTH_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_TEXT_HEIGHT, str );
    pDictTool->addEntry( CONTOUR_TEXT_HEIGHT_KEY, str );

    delete pDictTool;
}

// ����Ĭ�ϲ���
static void SetDefParam( bool& bSmooth, double& textHeight )
{
    bSmooth = ( DEF_SMOOTH_TYPE != 0 );
    textHeight = DEF_TEXT_HEIGHT;
}

void ContourParamHelper::ReadParams( bool& bSmooth, double& textHeight )
{
    // ����Ĭ�ϲ���
    // ����ʵ����ʧ�ܣ���֤�����Ǻ����
    SetDefParam( bSmooth, textHeight );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CONTOUR_PARAM_DICT );

    CString str;
    pDictTool->getEntry( CONTOUR_SMOOTH_KEY, 1, str );
    int i;
    ArxUtilHelper::StringToInt( str, i );
    bSmooth = ( i != 0 );

    pDictTool->getEntry( CONTOUR_TEXT_HEIGHT_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, textHeight );

    delete pDictTool;
}

void ContourParamHelper::WriteParams( bool bSmooth, double textHeight )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CONTOUR_PARAM_DICT );

    CString str;
    ArxUtilHelper::IntToString( bSmooth ? 1 : 0, str );
    pDictTool->modifyEntry( CONTOUR_SMOOTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( textHeight, str );
    pDictTool->modifyEntry( CONTOUR_TEXT_HEIGHT_KEY, 1, str );

    delete pDictTool;
}