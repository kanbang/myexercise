#include "StdAfx.h"

#include "VNG_ParamHelper.h"
#include "DefParam.h"
#include "VNGDict.h"

#include "../ArxHelper/ArxDataTool.h"
#include "../ArxHelper/ArxDictTool.h"
#include "../ArxHelper/ArxUtilHelper.h"

void VNG_ParamHelper::InitParams()
{
    // 词典存在
    if( ArxDataTool::IsDictExist( VNG_PARAM_DICT ) ) return;

    ArxDataTool::RegDict( VNG_PARAM_DICT );

    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    /* 初始化图形参数 */
    ArxUtilHelper::DoubleToString( DEF_NODE_SEP, str );
    pDictTool->addEntry( NODE_SEP_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_RANK_SEP, str );
    pDictTool->addEntry( RANK_SEP_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_GRAPH_RATIO, str );
    pDictTool->addEntry( GRAPH_RATIO_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_GRAPH_WIDTH, str );
    pDictTool->addEntry( GRAPH_WIDTH_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_GRAPH_HEIGHT, str );
    pDictTool->addEntry( GRAPH_HEIGHT_KEY, str );

    ArxUtilHelper::IntToString( DEF_GRAPH_USE_DEF_WH, str );
    pDictTool->addEntry( GRAPH_USE_DEF_WH_KEY, str );

    /* 初始化节点参数 */
    ArxUtilHelper::DoubleToString( DEF_NODE_WIDTH, str );
    pDictTool->addEntry( NODE_WIDTH_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_NODE_HEIGHT, str );
    pDictTool->addEntry( NODE_HEIGHT_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_NODE_TEXT_HEIGHT, str );
    pDictTool->addEntry( NODE_TEXT_HEIGHT_KEY, str );

    /* 初始化分支参数 */
    ArxUtilHelper::DoubleToString( DEF_ARROW_WIDTH, str );
    pDictTool->addEntry( ARROW_WIDTH_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_ARROW_LENGTH, str );
    pDictTool->addEntry( ARROW_LENGTH_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_EDGE_TEXT_HEIGHT, str );
    pDictTool->addEntry( EDGE_TEXT_HEIGHT_KEY, str );

    ArxUtilHelper::IntToString( DEF_NEED_EDGE, str );
    pDictTool->addEntry( NEED_EDGE_KEY, str );

    /* 初始化风门参数 */
    ArxUtilHelper::DoubleToString( DEF_GATE_RADIUS, str );
    pDictTool->addEntry( GATE_RADIUS_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_GATE_HALF_LENGTH, str );
    pDictTool->addEntry( GATE_HALF_LENGTH_KEY, str );

    /* 初始化风机参数 */
    ArxUtilHelper::DoubleToString( DEF_FAN_RADIUS, str );
    pDictTool->addEntry( FAN_RADIUS_KEY, str );

    ArxUtilHelper::DoubleToString( DEF_FAN_DISTANCE, str );
    pDictTool->addEntry( FAN_DISTRANCE_KEY, str );

    delete pDictTool;
}

// 设置默认图形参数
static void SetDefGraphParam( GraphParam& gp )
{
    gp.m_nodeSep = DEF_NODE_SEP;
    gp.m_rankSep = DEF_RANK_SEP;
    gp.m_graphRatio = DEF_GRAPH_RATIO;
    gp.m_graphWidth = DEF_GRAPH_WIDTH;
    gp.m_graphHeight = DEF_GRAPH_HEIGHT;
    gp.m_useDefWH = true;
}

// 设置默认节点参数
static void SetDefNodeParam( NodeParam& np )
{
    np.m_nodeWidth = DEF_NODE_WIDTH;
    np.m_nodeHeight = DEF_NODE_HEIGHT;
    np.m_nodeTextHeight = DEF_NODE_TEXT_HEIGHT;
}

// 设置默认风门参数
static void SetDefGateParam( GateParam& gp )
{
    gp.m_radius = DEF_GATE_RADIUS;
    gp.m_length = DEF_GATE_HALF_LENGTH;
}

// 设置默认风机参数
static void SetDefFanParam( FanParam& fp )
{
    fp.m_radius = DEF_FAN_RADIUS;
    fp.m_distance = DEF_FAN_DISTANCE;
}

// 设置默认参数
static void SetDefEdgeParam( EdgeParam& ep )
{
    ep.m_arrowWidth = DEF_ARROW_WIDTH;
    ep.m_arrowLength = DEF_ARROW_LENGTH;
    ep.m_edgeTextHeight = DEF_EDGE_TEXT_HEIGHT;
    ep.m_needEdge = ( DEF_NEED_EDGE != 0 );
}

void VNG_ParamHelper::ReadGraphParams( GraphParam& gp )
{
    // 设置默认参数
    // 如果词典操作失败，保证参数是合理的
    SetDefGraphParam( gp );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    CString str;

    /* 读取图形参数 */
    pDictTool->getEntry( NODE_SEP_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_nodeSep );

    pDictTool->getEntry( RANK_SEP_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_rankSep );

    pDictTool->getEntry( GRAPH_RATIO_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_graphRatio );

    pDictTool->getEntry( GRAPH_WIDTH_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_graphWidth );

    pDictTool->getEntry( GRAPH_HEIGHT_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_graphHeight );

    pDictTool->getEntry( GRAPH_USE_DEF_WH_KEY, 1, str );
    int i = 0;
    ArxUtilHelper::StringToInt( str, i );
    gp.m_useDefWH = ( i != 0 );

    delete pDictTool;
}

void VNG_ParamHelper::ReadNodeParams( NodeParam& np )
{
    // 设置默认参数
    // 如果词典操作失败，保证参数是合理的
    SetDefNodeParam( np );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    CString str;

    /* 读取节点参数 */
    pDictTool->getEntry( NODE_WIDTH_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, np.m_nodeWidth );

    pDictTool->getEntry( NODE_HEIGHT_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, np.m_nodeHeight );

    pDictTool->getEntry( NODE_TEXT_HEIGHT_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, np.m_nodeTextHeight );

    delete pDictTool;
}

void VNG_ParamHelper::ReadEdgeParams( EdgeParam& ep )
{
    // 设置默认参数
    // 如果词典操作失败，保证参数是合理的
    SetDefEdgeParam( ep );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    CString str;

    /* 读取分支参数 */
    pDictTool->getEntry( ARROW_WIDTH_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, ep.m_arrowWidth );

    pDictTool->getEntry( ARROW_LENGTH_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, ep.m_arrowLength );

    pDictTool->getEntry( EDGE_TEXT_HEIGHT_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, ep.m_edgeTextHeight );

    pDictTool->getEntry( NEED_EDGE_KEY, 1, str );
    int i = 0;
    ArxUtilHelper::StringToInt( str, i );
    ep.m_needEdge = ( i != 0 );

    delete pDictTool;
}

void VNG_ParamHelper::ReadGateParams( GateParam& gp )
{
    SetDefGateParam( gp );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    CString str;
    /* 读取风门参数 */
    pDictTool->getEntry( GATE_RADIUS_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_radius );

    pDictTool->getEntry( GATE_HALF_LENGTH_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, gp.m_length );

    delete pDictTool;
}

void VNG_ParamHelper::ReadFanParams( FanParam& fp )
{
    SetDefFanParam( fp );

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    CString str;
    /* 读取风机参数 */
    pDictTool->getEntry( FAN_RADIUS_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, fp.m_radius );

    pDictTool->getEntry( FAN_DISTRANCE_KEY, 1, str );
    ArxUtilHelper::StringToDouble( str, fp.m_distance );

    delete pDictTool;
}

void VNG_ParamHelper::WriteGraphParams( const GraphParam& gp )
{
    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    /* 写入图形参数 */
    ArxUtilHelper::DoubleToString( gp.m_nodeSep, str );
    pDictTool->modifyEntry( NODE_SEP_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.m_rankSep, str );
    pDictTool->modifyEntry( RANK_SEP_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.m_graphRatio, str );
    pDictTool->modifyEntry( GRAPH_RATIO_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.m_graphWidth, str );
    pDictTool->modifyEntry( GRAPH_WIDTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.m_graphHeight, str );
    pDictTool->modifyEntry( GRAPH_HEIGHT_KEY, 1, str );

    int i = ( gp.m_useDefWH ? 1 : 0 );
    ArxUtilHelper::IntToString( i, str );
    pDictTool->modifyEntry( GRAPH_USE_DEF_WH_KEY, 1, str );

    delete pDictTool;
}

void VNG_ParamHelper::WriteNodeParams( const NodeParam& np )
{
    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    /* 写入节点参数 */
    ArxUtilHelper::DoubleToString( np.m_nodeWidth, str );
    pDictTool->modifyEntry( NODE_WIDTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( np.m_nodeHeight, str );
    pDictTool->modifyEntry( NODE_HEIGHT_KEY, 1, str );

    ArxUtilHelper::DoubleToString( np.m_nodeTextHeight, str );
    pDictTool->modifyEntry( NODE_TEXT_HEIGHT_KEY, 1, str );

    delete pDictTool;
}

void VNG_ParamHelper::WriteEdgeParams( const EdgeParam& ep )
{
    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    /* 写入分支参数 */
    ArxUtilHelper::DoubleToString( ep.m_arrowWidth, str );
    pDictTool->modifyEntry( ARROW_WIDTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( ep.m_arrowLength, str );
    pDictTool->modifyEntry( ARROW_LENGTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( ep.m_edgeTextHeight, str );
    pDictTool->modifyEntry( EDGE_TEXT_HEIGHT_KEY, 1, str );

    int i = ( ep.m_needEdge ? 1 : 0 );
    ArxUtilHelper::IntToString( i, str );
    pDictTool->modifyEntry( NEED_EDGE_KEY, 1, str );

    delete pDictTool;
}

void VNG_ParamHelper::WriteGateParams( const GateParam& gp )
{
    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    /* 写入风门参数 */
    ArxUtilHelper::DoubleToString( gp.m_radius, str );
    pDictTool->modifyEntry( ARROW_WIDTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( gp.m_length, str );
    pDictTool->modifyEntry( ARROW_LENGTH_KEY, 1, str );

    delete pDictTool;
}

void VNG_ParamHelper::WriteFanParams( const FanParam& fp )
{
    CString str;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( VNG_PARAM_DICT );

    /* 写入风机参数 */
    ArxUtilHelper::DoubleToString( fp.m_radius, str );
    pDictTool->modifyEntry( ARROW_WIDTH_KEY, 1, str );

    ArxUtilHelper::DoubleToString( fp.m_distance, str );
    pDictTool->modifyEntry( ARROW_LENGTH_KEY, 1, str );

    delete pDictTool;
}
