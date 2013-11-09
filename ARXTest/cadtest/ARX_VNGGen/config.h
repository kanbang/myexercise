#pragma once

/*
 * 节点之间的最小间距,
 * 在dot中默认为0.25英寸
 * 本程序默认为0.5英寸->12.7毫米
 */
#define DEF_NODE_SEP 12.7

/*
 * 垂直层次之间的最小间距
 * 在dot中默认为0.5英寸
 * 在本程序默认为1英寸->25.4毫米
 */
#define DEF_RANK_SEP 25.4

/* 默认图幅宽/高比值 */
#define DEF_GRAPH_RATIO 3

/* 默认图幅宽度800毫米 */
#define DEF_GRAPH_WIDTH 600

/* 默认图幅高度600毫米 */
#define DEF_GRAPH_HEIGHT 800

/*
 * 是否使用默认的宽高设置
 * 即不设置ratio以及宽度
 */
#define DEF_GRAPH_USE_DEF_WH 1

/* 
 * 节点的宽度 
 * 在dot中默认为0.75英寸
 */
#define DEF_NODE_WIDTH 19.05

/*
 * 节点的高度
 * 在dot中默认为0.5英寸
 */
#define DEF_NODE_HEIGHT 12.7

/*
 * dot的字体高度默认为14pt
 * 换算成mm==>14/72*25.4 = 4.94
 */
#define DEF_NODE_TEXT_HEIGHT 4.94


/*
 * 在dot中, 箭头宽度默认为1英寸
 * 在本程序中默认为4毫米
 */
#define DEF_ARROW_WIDTH 4

/* 
 * 箭头默认长度
 * 在本程序中默认为9毫米
 */
#define DEF_ARROW_LENGTH 9

/*
 * dot的字体高度默认为14pt
 * 换算成mm==>14/72*25.4 = 4.94
 */
#define DEF_EDGE_TEXT_HEIGHT 4.94

/* 默认不显示分支编号 */
#define DEF_NEED_EDGE 0

/* 在cad词典中存储通风网络图绘制参数，也即dot需要的参数 */

#define VNG_PARAM_DICT _T("通风网络图绘制参数词典")

#define NODE_SEP_KEY _T("水平最小间距")
#define RANK_SEP_KEY _T("垂直最小间距")

#define GRAPH_RATIO_KEY _T("图形宽高比值")
#define GRAPH_WIDTH_KEY _T("图形宽度")
#define GRAPH_HEIGHT_KEY _T("图形高度")
#define GRAPH_USE_DEF_WH_KEY _T("是否使用默认的宽高设置")

#define NODE_WIDTH_KEY _T("节点宽度")
#define NODE_HEIGHT_KEY _T("节点高度")
#define NODE_TEXT_HEIGHT_KEY _T("节点标注字体高度")

#define ARROW_WIDTH_KEY _T("箭头宽度")
#define ARROW_LENGTH_KEY _T("箭头长度")
#define EDGE_TEXT_HEIGHT_KEY _T("分支标注字体高度")

#define NEED_EDGE_KEY _T("显示分支编号")