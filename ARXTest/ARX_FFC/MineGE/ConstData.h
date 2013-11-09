#pragma once

// 预定义一些图元的属性数据常量(枚举)

/*******************************/
/****** 巷道(Tunnel)相关*******/
/*******************************/

// 材质类型
enum MATERIAL_TYPE
{
	MT_ROCK     = 0,   // 岩巷(默认)
	MT_COAL     = 1,   // 煤巷
	MT_MIXED    = 2    // 半煤岩巷
};

// 通风类型
// 如果没有指定，默认为进风
enum VENT_TYPE
{
	VT_IN       = 0,   // 进风
	VT_OUT      = 1,   // 回风
};

// 功能类型
// 如果没有指定，默认为普通巷道
enum FUNCTION_TYPE
{
	FT_SIMPLE    = 0,  // 普通巷道(行人、通风等)
	FT_MAIN      = 1,  // 主要大巷(进风/回风)
	FT_SUBMAIN   = 2,  // 采区大巷(进风/回风)
	FT_TRANSPORT = 3,  // 运输机巷(工作面通过运输机运煤的巷道)
	FT_ELECTRIC  = 4,  // 架线电机车巷
	FT_DIESEL    = 5   // 胶轮车巷
};

//	FT_LIAISON   = 5   // 联络巷

// 井筒附加属性
enum SHAFT_EXTRA_PROPERTY
{
	SEP_NO_EXTRA_PROPERTY         = 0x0000,    // 无附加属性(默认)

	// 提升用途
	SEP_NO_LIFT_USE               = 0x0001,    // 无提升用途
	SEP_LIFT_COAL                 = 0x0002,    // 提升煤炭
	SEP_LIFT_MATERIAL_OR_STUFF    = 0x0004,    // 提升物料或升降人员

	// 提升物料/人员
	SEP_HAS_GLADER_OR_REPAIR      = 0x0008,    // 是否有梯子间或修理中
	SEP_LIFT_STUFF                = 0x0010,    // 提升人员
	SEP_LIFT_MATERIAL             = 0x0020,    // 提升物料

	// 提煤方式
	SEP_SKIP_HOIST                = 0x0040,    // 箕斗提升
	SEP_BELT_CONVEY               = 0x0080,    // 皮带输送

	// 控制措施
	SEP_ENCLOSE                   = 0x0100,    // 完善的封闭措施
	SEP_DUST_PROOF                = 0x0200,    // 可靠的防尘措施
	SEP_CH4_INTERRUPT             = 0x0400,    // 安装了甲烷断电仪
	SEP_FIRE_ALARM                = 0x0800,    // 安装了自动报警灭火装置
	SEP_FIRE_CONTROL_PIPE         = 0x1000     // 敷设了消防管路	
};

/*******************************/
/*** 工作面(WorkSurface)相关***/
/*******************************/

// 工作面采煤方法
enum CoalMiningMethod
{
	CMM_COMPOSITIVE = 0,    // 综采(默认)
	CMM_GENERAL     = 1,    // 普采
	CMM_BOMB        = 2,    // 炮采
};

// 超温控制措施
enum HighTempControlMeasure
{
	HTCM_NONE                  = 0,    // 无超温控制措施(默认)
	HTCM_TIME_AND_PROTECTION   = 1,    // 缩短超温地点工作人员的工作时间，并给予高温保健待遇
	HTCM_STOP_WORK             = 2     // 停止作业
};

// 瓦斯等级
enum GAS_LEVEL
{
	GL_LOW      = 0,       // 低瓦斯(默认)
	GL_HIGH     = 1,       // 高瓦斯
	GL_OUTBURST = 2        // 煤与瓦斯突出
};

// 通风方式
enum VENT_PATTERN
{
	VP_PULL    = 0,       // 抽出式(默认)
	VP_PUSH    = 1        // 压入式
};

// 机电硐室类型
enum ROOM_TYPE
{
	RT_AIR_COMPRESSOR   = 0,  // 空气压缩机房
	RT_POWER_SUBSTATION = 1,  // 变电所
	RT_HOIST_ROOM       = 2,  // 绞车房
	RT_WATER_PUMP       = 3   // 水泵房
};