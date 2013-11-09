#pragma once

#include "AFCDataLink.h"

class TableCreator;
class DataWriter;

class AirFlowCaculate
{
public:
	void doAFC(const CString& path, AcArray<AFCDataLink*>& ts);
	void setMineGasState(bool isHigh);

private:
	void doWsAFC(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void doTwsAFC(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void doDhcAFC1(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void doDhcAFC2(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void doDhcAFC3(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void doOtAFC(TableCreator* pTableCreator, DataWriter* pDataWriter);

	void init_WSModel();
	void init_TWSModel();
	void init_StorageModel1();
	void init_StorageModel2();
	void init_StorageModel3();
	void init_OTModel();
	void clean();

private:
	AcArray<AFCDataLink*> ds;
	bool highGasMine;  // 是否高瓦斯矿井(默认为true)
};