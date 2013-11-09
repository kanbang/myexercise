#pragma once

// ��˹��ֵ������
enum GasContourType
{
	GCT_FLOW     = 0,   // ������˹ӿ����(GFP)
	GCT_CONTENT  = 1,   // ��˹����(GCP)
	GCT_PRESSURE = 2    // ��˹ѹ��ֵ(GPP)
};

extern CString GetGasTypeName(GasContourType gct);
extern CString GetGasFieldName(GasContourType gct);
