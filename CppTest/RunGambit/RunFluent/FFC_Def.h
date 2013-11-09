#pragma once

/* 数据文件目录 */
#define JOU_FILE_NAME _T("ffc_var.jou")
#define DATA_DIR _T("data\\")
#define BOUND_FILE_NAME _T("边界设置.txt")
#define GOAF_TB_GAS_FILE_NAME _T("采空区顶底板瓦斯源.txt")
#define POROUS_FILE_NAME _T("多孔介质设置.txt")
#define OBTURATION_FILE_NAME _T("密闭源汇.txt")
#define AIR_LEAK_FILE_NAME _T("漏风源汇.txt")
#define GAS_PIPE_FILE_NAME _T("瓦斯钻孔源.txt")
#define N2_PIPE_FILE_NAME _T("注氮孔源.txt")

/* Gambit相关宏定义 */

// Gabmit的注册表项以及路径
#define GAMBIT_KEY HKEY_CURRENT_USER
#define GAMBIT_SUB_KEY _T("Software\\Fluent Inc Products\\Gambit")
#define GAMBIT_BIN _T("ntbin\\ntx86\\gambit.exe")

// gambit主要jou执行文件
#define GAMBIT_VAR_JOU_FIEL_NAME _T("GambitVar.jou")

// Gambit Startup窗口的标题
#define GAMBIT_STARTUP_WINDOWS_NAME _T("Gambit Startup")

// Gambti的session文件
#define GAMBIT_SESSION_ID _T("ffc")
#define GAMBIT_MESH_FILE _T("ffc.msh")
#define GAMBIT_JOU_FILE _T("ffc.jou")
#define GAMBIT_TRN_FILE _T("ffc.trn")
#define GAMBIT_DBS_FILE _T("ffc.dbs")

// 生成msh失败时,额外生成的错误文件
#define GAMBIT_ERROR_FILE _T("ffcerror")

/* Fluent相关宏定义 */

// Fluent注册表项以及路径
#define FLUENT_KEY HKEY_CURRENT_USER
#define FLUENT_SUB_KEY _T("Software\\Fluent Inc Products\\Fluent")
#define FLUENT_BIN _T("ntbin\\ntx86\\fluent.exe")

#define FLUENT_CAS_FILE _T("ffc.cas")
#define FLUENT_DAT_FILE _T("ffc.dat")
#define FLUENT_ERROR_FILE _T("ffcerror")
#define FLUENT_LOAD_SCM_FILE _T("load.scm")

/* 后处理相关宏定义 */
// Tecplot显示的类型
#define TECPLOT_DATA_DIR _T("plot\\")

enum TECPLOT_SHOW_TYPE
{
    TST_VELOCITY_NEPHOGRAM                  = 0,              // 速度云图
    TST_VELOCITY_VECTOR_GRAPH               = 1,              // 速度矢量图
    TST_PRESS_NEPHOGRAM                     = 2,              // 静压力云图
    TST_VELOCITY_STREAM_LINE_ANIMATION      = 3,              // 速度流线动画
    TST_OXYGEN_MASS_FRACTION_DISTRIBUTION   = 4,              // 氧气质量分数分布
    TST_NITROGEN_MAS_FRACTION_DISTRIBUTION  = 5,              // 氮气质量分数分布
    TST_CH4_MAS_FRACTION_DISTRIBUTION       = 6               // 甲烷质量分数分布
};

// 多线程查询时间间隔
#define THREAD_CHECK_TIME_INTERVAL 1000
