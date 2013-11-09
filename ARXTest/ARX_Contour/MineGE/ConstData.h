#pragma once

// Ԥ����һЩͼԪ���������ݳ���(ö��)

/*******************************/
/****** ���(Tunnel)���*******/
/*******************************/

// ��������
enum MATERIAL_TYPE
{
	MT_ROCK     = 0,   // ����(Ĭ��)
	MT_COAL     = 1,   // ú��
	MT_MIXED    = 2    // ��ú����
};

// ͨ������
// ���û��ָ����Ĭ��Ϊ����
enum VENT_TYPE
{
	VT_IN       = 0,   // ����
	VT_OUT      = 1,   // �ط�
};

// ��������
// ���û��ָ����Ĭ��Ϊ��ͨ���
enum FUNCTION_TYPE
{
	FT_SIMPLE    = 0,  // ��ͨ���(���ˡ�ͨ���)
	FT_MAIN      = 1,  // ��Ҫ����(����/�ط�)
	FT_SUBMAIN   = 2,  // ��������(����/�ط�)
	FT_TRANSPORT = 3,  // �������(������ͨ���������ú�����)
	FT_ELECTRIC  = 4,  // ���ߵ������
	FT_DIESEL    = 5   // ���ֳ���
};

//	FT_LIAISON   = 5   // ������

// ��Ͳ��������
enum SHAFT_EXTRA_PROPERTY
{
	SEP_NO_EXTRA_PROPERTY         = 0x0000,    // �޸�������(Ĭ��)

	// ������;
	SEP_NO_LIFT_USE               = 0x0001,    // ��������;
	SEP_LIFT_COAL                 = 0x0002,    // ����ú̿
	SEP_LIFT_MATERIAL_OR_STUFF    = 0x0004,    // �������ϻ�������Ա

	// ��������/��Ա
	SEP_HAS_GLADER_OR_REPAIR      = 0x0008,    // �Ƿ������Ӽ��������
	SEP_LIFT_STUFF                = 0x0010,    // ������Ա
	SEP_LIFT_MATERIAL             = 0x0020,    // ��������

	// ��ú��ʽ
	SEP_SKIP_HOIST                = 0x0040,    // ��������
	SEP_BELT_CONVEY               = 0x0080,    // Ƥ������

	// ���ƴ�ʩ
	SEP_ENCLOSE                   = 0x0100,    // ���Ƶķ�մ�ʩ
	SEP_DUST_PROOF                = 0x0200,    // �ɿ��ķ�����ʩ
	SEP_CH4_INTERRUPT             = 0x0400,    // ��װ�˼���ϵ���
	SEP_FIRE_ALARM                = 0x0800,    // ��װ���Զ��������װ��
	SEP_FIRE_CONTROL_PIPE         = 0x1000     // ������������·	
};

/*******************************/
/*** ������(WorkSurface)���***/
/*******************************/

// �������ú����
enum CoalMiningMethod
{
	CMM_COMPOSITIVE = 0,    // �۲�(Ĭ��)
	CMM_GENERAL     = 1,    // �ղ�
	CMM_BOMB        = 2,    // �ڲ�
};

// ���¿��ƴ�ʩ
enum HighTempControlMeasure
{
	HTCM_NONE                  = 0,    // �޳��¿��ƴ�ʩ(Ĭ��)
	HTCM_TIME_AND_PROTECTION   = 1,    // ���̳��µص㹤����Ա�Ĺ���ʱ�䣬��������±�������
	HTCM_STOP_WORK             = 2     // ֹͣ��ҵ
};

// ��˹�ȼ�
enum GAS_LEVEL
{
	GL_LOW      = 0,       // ����˹(Ĭ��)
	GL_HIGH     = 1,       // ����˹
	GL_OUTBURST = 2        // ú����˹ͻ��
};

// ͨ�緽ʽ
enum VENT_PATTERN
{
	VP_PULL    = 0,       // ���ʽ(Ĭ��)
	VP_PUSH    = 1        // ѹ��ʽ
};

// ������������
enum ROOM_TYPE
{
	RT_AIR_COMPRESSOR   = 0,  // ����ѹ������
	RT_POWER_SUBSTATION = 1,  // �����
	RT_HOIST_ROOM       = 2,  // �ʳ���
	RT_WATER_PUMP       = 3   // ˮ�÷�
};