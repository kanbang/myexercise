#pragma once

// 将所有定义的常量字符串合并在一处
// 将代码集中在一起，避免手写错误
// 同时也不影响具体的字段名称修改

#include "../DaoHelper/DaoDataNames.h"
#include "../EvalBase/EvalNames.h"

#define EVAL_RESULT_DATA_BASE_NAME _T("常规评价结果")

#define REGULATION_INFO_MAIN_TABLE_NAME _T("规程信息索引")

// 规程信息索引表的字段
#define NAME_FIELD _T("名称")
#define VERSION_FIELD _T("版本")
#define PUBLISHER_FIELD _T("发布单位")
#define LINK_TABLE_FIELD _T("对应的数据库表")

// 具体规程表的字段
#define PARENT_UNIT_FIELD _T("父评价单元")
#define UNIT_FIELD _T("评价单元")
//#define CLAUSE_NUM_FIELD _T("条款编号")
#define CLAUSE_CONTENT_FIELD _T("条款内容")
#define INIT_QUSESTION_NAIRE_RESULT_FIELD _T("初始调查结果")
//#define SIMPLE_CAUSE_FIELD _T("简单原因")

// 评价信息数据文件的ENTRY-KEY
#define EVAL_DATA_INFO_FILE_NAME _T("评价数据信息链接")

#define REGULATION_INFO_SECTION _T("规程信息")
#define REGULATION_DATABASE_PATH_KEY _T("规程信息索引数据库路径")
#define REGULATION_TABLE_KEY _T("待评价的规程对应数据库表")

#define EVAL_RESULT_SECTION _T("评价结果信息")
#define EVAL_RESULT_KEY _T("规程是否实现")

#define COAL_MINE_SECTION _T("矿井信息")
#define COAL_MINE_NAME_KEY _T("名称")
#define COMPANY_NAME_KEY _T("隶属集团公司")
#define COAL_MINE_AIR_FLOW_COEFF_KEY _T("通风需风系数")
#define COAL_MINE_AIR_FLOW_COEFF2_KEY _T("通风系数")

#define EVAL_PROJECT_INFO_SECTION _T("评价项目信息")
#define EVAL_REGULATION_KEY _T("评价依据")
#define	PREPARE_SOLUTION_BEGIN_KEY _T("制定评价方案开始时间")
#define	PREPARE_SOLUTION_END_KEY _T("制定评价方案结束时间")
#define PREPARE_INFO_BEGIN_KEY _T("收集评价资料开始时间")
#define PREPARE_INFO_END_KEY _T("收集评价资料结束时间")
#define PREPARE_CONTRACT_BEGIN_KEY _T("签订评价合同开始时间")
#define PREPARE_CONTRACT_END_KEY _T("签订评价合同结束时间")
#define SURVEY_LEARN_BEGIN_KEY _T("评价宣传贯彻开始时间")
#define SURVEY_LEARN_END_KEY _T("评价宣传贯彻结束时间")
#define WORK_DO_BEGIN_KEY _T("开展评价工作开始时间")
#define WORK_DO_END_KEY _T("开展评价工作结束时间")
#define WORK_REPORT_BEGIN_KEY _T("编制评价报告开始时间")
#define WORK_REPORT_END_KEY _T("编制评价报告结束时间")


#define EVAL_REPORT_FILE_NAME_SECTION _T("评价报告")
#define EVAL_REPORT_FILE_NAME_KEY _T("评价报告名称")

#define EVAL_REPORT_TEMPLATE_FILE_NAME _T("评价报告模板")
