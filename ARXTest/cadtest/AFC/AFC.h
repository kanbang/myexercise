#pragma once

#include "dlimexp.h"
#include "CoalMineInfo.h"

#include <map>

class TableCreator;
class DataWriter;

// !!!派生类导出的时候一定要写析构函数!!!
namespace PJ
{
	class AFCStrategy;
	class AFCStrategyComposite;	

	class AFC_EXPORT_API AFC
	{
	public:
		virtual ~AFC(void);
		double doAFC(TableCreator* pTableCreator, DataWriter* pDataWriter);
		void setName(const CString& name);
		void setUserQ(double q);

	protected:
		AFC(const CString& tableName);
		void regAFCStrategy(const CString& tableName, AFCStrategy* pAFCStrategy);

	private:
		void prepare();
		void clean();
		typedef std::map<CString, AFCStrategy*> AFCStrategyCollection;
		AFCStrategyCollection* pAFCStrategies;
		AFCStrategyComposite* pAFC;
		CString name;  // 名称
		double userQ;  // 用户指定风量
	};

	// !!!派生类导出的时候一定要写析构函数!!!
	class AFC_EXPORT_API WS_AFC : public AFC
	{
	public:
		WS_AFC();
		~WS_AFC();
		void initMC_AFCStrategy(double max_kdj, double min_kdj, double height, double reasonalVelocity, double k_height, double k_length, double k_temperature);
		void initGas_AFCStrategy(double q_ch4, double k_ch4, double q_ch4_tail = 0);
		void initTve_AFCStrategy(double reasonalVelocity, double area);
		void initMaxPeopleNum_AFC_CheckStrategy(int maxPeopleNum);
		void initMaxWeight_AFC_CheckStrategy(double maxWeight);
		void initVE_AFC_CheckStrategy(double area, bool hasGasTunnel = false);
	};

	class AFC_EXPORT_API TWS_AFC : public AFC
	{
	public:
		TWS_AFC();
		~TWS_AFC();
		void initGas_AFCStrategy(double q_ch4, double k_ch4);
		void initLocalFan_AFCStrategy(double area, int fanNum, double q_fan, bool isRock = true);
		void initMaxPeopleNum_AFC_CheckStrategy(int maxPeopleNum);
		void initMaxWeight_AFC_CheckStrategy(double maxWeight);
		void initVE_AFC_CheckStrategy(double area, bool isRock = true);
	};

	class AFC_EXPORT_API DHC_AFC : public AFC
	{
	public:
		DHC_AFC();
		~DHC_AFC();
		void initUEM_AFCStrategy(double V);
		void initPCR_AFCStrategy(double q_h2);
		void initLargeMEC_AFCStrategy(double power, double theta, double delta_t);
		void initSmallMEC_AFCStrategy(double power, double q_def);
		void initConstMinQ_AFC_CheckStrategy();
	};

	class AFC_EXPORT_API OT_AFC : public AFC
	{
	public:
		OT_AFC();
		~OT_AFC();
		void initOT_Gas_AFCStrategy(double q_ch4_max, CoalMineGasInfo gasInfo, TunnelMaterial tm);
		void initVE_AFC_CheckStrategy(double area, bool isRock = true);
		void initEl_VE_AFC_CheckStrategy(double area);
		void initRTC_AFCStrategy(int num, double power);
	};
}