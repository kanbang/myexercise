#pragma once

/* �����ļ�Ŀ¼ */
#define JOU_FILE_NAME _T("ffc_var.jou")
#define DATA_DIR _T("data\\")
#define BOUND_FILE_NAME _T("�߽�����.txt")
#define GOAF_TB_GAS_FILE_NAME _T("�ɿ������װ���˹Դ.txt")
#define POROUS_FILE_NAME _T("��׽�������.txt")
#define OBTURATION_FILE_NAME _T("�ܱ�Դ��.txt")
#define AIR_LEAK_FILE_NAME _T("©��Դ��.txt")
#define GAS_PIPE_FILE_NAME _T("��˹���Դ.txt")
#define N2_PIPE_FILE_NAME _T("ע����Դ.txt")

/* Gambit��غ궨�� */

// Gabmit��ע������Լ�·��
#define GAMBIT_KEY HKEY_CURRENT_USER
#define GAMBIT_SUB_KEY _T("Software\\Fluent Inc Products\\Gambit")
#define GAMBIT_BIN _T("ntbin\\ntx86\\gambit.exe")

// gambit��Ҫjouִ���ļ�
#define GAMBIT_LOAD_JOU_FIEL _T("GambitVar.jou")

// Gambit Startup���ڵı���
#define GAMBIT_STARTUP_WINDOWS_NAME _T("Gambit Startup")

// Gambti��session�ļ�
#define GAMBIT_SESSION_ID _T("ffc")
#define GAMBIT_MESH_FILE _T("ffc.msh")
#define GAMBIT_JOU_FILE _T("ffc.jou")
#define GAMBIT_TRN_FILE _T("ffc.trn")
#define GAMBIT_DBS_FILE _T("ffc.dbs")

// ����mshʧ��ʱ,�������ɵĴ����ļ�
#define GAMBIT_ERROR_FILE _T("ffc_gambit.error")

/* Fluent��غ궨�� */

// Fluentע������Լ�·��
#define FLUENT_KEY HKEY_CURRENT_USER
#define FLUENT_SUB_KEY _T("Software\\Fluent Inc Products\\Fluent")
#define FLUENT_BIN _T("ntbin\\ntx86\\fluent.exe")

#define FLUENT_CAS_FILE _T("ffc.cas")
#define FLUENT_DAT_FILE _T("ffc.dat")
#define FLUENT_ERROR_FILE _T("ffc_fluent.error")
#define FLUENT_LOAD_SCM_FILE _T("load.scm")

/* ������غ궨�� */
// Tecplot��ʾ������
#define TECPLOT_LOAD_SCM_FILE _T("loadplot.scm")
#define TECPLOT_PLOT_TYPE_FILE _T("displayplot.txt")
#define TECPLOT_DATA_DIR _T("plot\\")

enum TECPLOT_SHOW_TYPE
{
	TST_VELOCITY_NEPHOGRAM                  = 0,              // �ٶ���ͼ
	TST_VELOCITY_VECTOR_GRAPH               = 1,              // �ٶ�ʸ��ͼ
	TST_PRESS_NEPHOGRAM                     = 2,              // ��ѹ����ͼ
	TST_VELOCITY_STREAM_LINE_ANIMATION      = 3,              // �ٶ����߶���
	TST_OXYGEN_MASS_FRACTION_DISTRIBUTION   = 4,              // �������������ֲ�
	TST_NITROGEN_MAS_FRACTION_DISTRIBUTION  = 5,              // �������������ֲ�
	TST_CH4_MAS_FRACTION_DISTRIBUTION       = 6               // �������������ֲ�
};

// ���̲߳�ѯʱ����
#define THREAD_CHECK_TIME_INTERVAL 1000

enum RUN_ERROR_CODE
{
	REC_NO_EEOR        = 0,      // �޴���
	REC_NOT_INSTALL    = 1,      // fluent���δ��װ
	REC_PROCESS_ERROR  = 2,      // �޷���������
	REC_FILE_NOT_EXIST = 3       // �����ļ�������
};

// ���ν�ģ��������
enum GEO_ERROR_CODE
{
	GEC_NO_ERROR   =  0,       // ���ν�ģ��ȷ
	GEC_NO_LINES   =  1,       // ͼ����û�����/������
	GEC_NO_WS      =  2,       // û�й�����
	GEC_NO_GOAF    =  3,       // ͼ����û�вɿ���
	GEC_NO_REGION  =  4,       // ͼ��û�й�������
	GEC_NO_WS_GAS  =  5,       // û�й�������˹�߽�(������û����ɿ���������һ��)
	GEC_NO_PRESS   =  6        // û�й�����ɿ�����ѹ����(������û����ɿ���������һ��)
};