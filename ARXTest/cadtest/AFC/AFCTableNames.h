#pragma once

// 风量计算的表名称
namespace PJ
{
	const CString AFC_RESULT_TABLE_NAME               = _T("cacl_result");  // 风量计算汇总表
	const CString WS_AFC_TABLE_NAME                   = _T("ws");           // 回采工作面风量计算表
	const CString TWS_AFC_TABLE_NAME                  = _T("tws");          // 掘进工作面风量计算表
	const CString DHC_AFC_TABLE_NAME                  = _T("dhc");          // 硐室风量计算表
	const CString OT_AFC_TABLE_NAME                   = _T("ot");           // 其它巷道风量计算表
	const CString MC_AFC_TABLE_NAME                   = _T("mc");           // 气象条件风量计算表
	const CString GAS_AFC_TABLE_NAME                  = _T("gas");          // 瓦斯涌出量风量计算表
	const CString TVE_AFC_TABLE_NAME                  = _T("tve");          // 根据温度选择适宜风速风量计算表
	const CString MAX_PEOPLE_NUM_AFC_CHECK_TABLE_NAME = _T("mpn");          // 工作面最多人数风量验算表
	const CString MAX_WEIGHT_AFC_CHECK_TABLE_NAME     = _T("mw");           // 一次爆炸最大炸药量风量验算表
	const CString VE_AFC_CHECK_TABLE_NAME             = _T("ve");           // 风速验算表
	const CString LOCAL_FAN_AFC_TABLE_NAME            = _T("lf");           // 局部通风机风量计算表
	const CString UEM_AFC_TABLE_NAME                  = _T("uem");          // 井下爆炸材料库风量计算表
	const CString PCR_AFC_TABLE_NAME                  = _T("pcr");          // 井下充电室风量计算表
	const CString MEC_AFC_TABLE_NAME                  = _T("mec");          // 硐室风量计算表
	const CString OT_GAS_AFC_TABLE_NAME               = _T("ot_gas");       // 其它巷道瓦斯涌出量风量计算表
	const CString RTC_AFC_TABLE_NAME                  = _T("rtc");          // 胶轮车风量计算表
	const CString CONST_MINQ_AFC_CHECK_TABLE_NAME     = _T("cminq");        // 硐室固定最小风量验算表
}