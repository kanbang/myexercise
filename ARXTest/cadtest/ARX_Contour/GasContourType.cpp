#include "stdafx.h"
#include "GasContourType.h"

static CString GasTypeName[3] =
{
    _T( "GFP" ),
    _T( "GCP" ),
    _T( "GPP" )
};

static CString GasFieldName[3] =
{
    _T( "������˹ӿ����" ),
    _T( "��˹����" ),
    _T( "��˹ѹ��ֵ" )
};

static CString GasContourLayerName[3] =
{
    _T( "��ֵ��-��˹ӿ����" ),
    _T( "��ֵ��-��˹����" ),
    _T( "��ֵ��-��˹ѹ��" )
};

CString GetGasTypeName( GasContourType gct )
{
    return GasTypeName[gct];
}

CString GetGasFieldName( GasContourType gct )
{
    return GasFieldName[gct];
}

CString GetGasContourLayerName( GasContourType gct )
{
    return GasContourLayerName[gct];
}