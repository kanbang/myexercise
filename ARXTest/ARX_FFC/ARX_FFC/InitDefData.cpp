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
        linkDoubleData( _T( "����" ), &q );
        linkDoubleData( _T( "�¶�" ), &t );
    }

public:
    void initDef()
    {
        if( O2 <= 0 || O2 >= 100 )
        {
            O2 = 21;  // 21%
        }
        if( CH4 <= 0 || CH4 >= 1 )
        {
            CH4 = 0; // 0%
        }
        if( area <= 0 )
        {
            area = 16;
        }
        if( q <= 0 )
        {
            q = 1200;
        }
        if( t <= 0 )
        {
            t = 25;
        }
    }
public:
    double q;
    double O2;
    double CH4;
    double area;
    double t;
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
        odl.initDef();		// ��ʼ��Ĭ������
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