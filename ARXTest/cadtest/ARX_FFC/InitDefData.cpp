#include "stdafx.h"

#include "../MineGE/DataLink.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

class TunnelDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "����Ũ��" ), &O2 );
        linkDoubleData( _T( "����Ũ��" ), &CH4 );
        linkDoubleData( _T( "�������" ), &area );
    }

public:
    double O2;
    double CH4;
    double area;
};

// ��ʼ��Ĭ������

// 1) ����Ũ��20%
static void InitDefTunnelData()
{
    // ���������������
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelDataLink odl;
        odl.setDataSource( objIds[i] );
        odl.updateData( false );

        //acutPrintf(_T("\nO2=%.2f"), odl.O2);
        if( odl.O2 <= 0 || odl.O2 >= 1 )
        {
            odl.O2 = 20; // 20%
        }
        if( odl.CH4 <= 0 || odl.CH4 >= 1 )
        {
            odl.CH4 = 0; // 0%
        }
        if( odl.area <= 0 )
        {
            odl.area = 16; // 20%
        }
        odl.updateData( true );
    }
}

void InitDefData()
{
    InitDefTunnelData();
}

void ConvertQUnit()
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString v;
        DataHelper::GetPropertyData( objIds[i], _T( "����" ), v );

        double d = 0;
        ArxUtilHelper::StringToDouble( v, d );

        v.Format( _T( "%.3f" ), d * 60 ); // m3/s ==> m3/min
        DataHelper::SetPropertyData( objIds[i], _T( "����" ), v );
    }
}