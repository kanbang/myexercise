#pragma once

#include "AFCDataLink.h"

class TableCreator;
class DataWriter;

#define DEF_POWER 300

class AirFlowCaculate
{
public:
	void test(const CString& path);
	void setMineGasState(bool isHigh);

private:
	void ws_afc_test(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void tws_afc_test(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void dhc_afc_test(TableCreator* pTableCreator, DataWriter* pDataWriter);
	void ot_afc_test(TableCreator* pTableCreator, DataWriter* pDataWriter);

	void init_WSModel();
	void init_TWSModel();
	void init_StorageModel1();
	void init_StorageModel2();
	void init_StorageModel3();
	void init_OTModel();
	void clean();

private:
	AcArray<DataLink*> mc;
	bool highGasMine;  // 是否高瓦斯矿井(默认为true)
};