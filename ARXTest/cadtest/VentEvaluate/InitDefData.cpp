#include "stdafx.h"

#include "../MineGE/DataLink.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

class OxygenDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "����Ũ��" ), &O2 );
    }

public:
    double O2;
};

class TempDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "�¶�" ), &T );
    }

public:
    double T;
};

// ��ʼ��Ĭ������

// 1) ����Ũ��20%
static void InitDefOxygenData()
{
    // �������еĲɾ���
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        OxygenDataLink odl;
        odl.setDataSource( objIds[i] );
        odl.updateData( false );

        //acutPrintf(_T("\nO2=%.2f"), odl.O2);
        if( odl.O2 <= 0 || odl.O2 >= 1 )
        {
            odl.O2 = 20; // 20%
        }
        odl.updateData( true );
    }
}

// 2) �����¶�20��
static void InitDefTempData()
{
    // �������еķ�֧��ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink tdl;
        tdl.setDataSource( objIds[i] );
        tdl.updateData( false );

        //acutPrintf(_T("\nT=%.2f"), tdl.T);
        if( tdl.T == 0 )
        {
            tdl.T = 20; // 20��
        }
        tdl.updateData( true );
    }
}

// 3) Ĭ�ϰ��չ涨���
static void InitDefWindData()
{
    // �������еĲ��վ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WindStation" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString v;
        DataHelper::GetPropertyData( objIds[i], _T( "�Ƿ񰴹涨���" ), v );
        if( v.GetLength() == 0 )
        {
            DataHelper::SetPropertyData( objIds[i], _T( "�Ƿ񰴹涨���" ), _T( "1" ) );
        }
    }
}

void InitDefData()
{
    InitDefOxygenData();
    InitDefTempData();
    InitDefWindData();
}

void SetTunnelMaterial()
{
    CString msg;
    msg.Format( _T( "\n���ÿ󾮵Ĵ󲿷�����������ͣ�" ) );
    msg.AppendFormat( _T( "\n\t0) ����\n\t1) ú��\n\t2) ��ú����" ) );
    acutPrintf( msg );

    int tm;
    if( RTNORM != acedGetInt( _T( "\n��ѡ��һ�����{0, 1, 2}���������������:" ), &tm ) ) return;
    if( tm > 3 || tm < 0 )
    {
        acedAlert( _T( "���{0, 1, 2}��ѡ��һ�����!!!" ) );
        return;
    }

    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    DrawHelper::FindMineGEs( _T( "ArcTunnel" ), objIds );
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString v;
        v.Format( _T( "%d" ), tm );
        DataHelper::SetPropertyData( objIds[i], _T( "��������" ), v );
    }
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