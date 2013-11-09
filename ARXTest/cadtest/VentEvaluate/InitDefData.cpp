#include "stdafx.h"

#include "../MineGE/DataLink.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

class OxygenDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "氧气浓度" ), &O2 );
    }

public:
    double O2;
};

class TempDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "温度" ), &T );
    }

public:
    double T;
};

// 初始化默认数据

// 1) 氧气浓度20%
static void InitDefOxygenData()
{
    // 查找所有的采掘面
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

// 2) 干球温度20度
static void InitDefTempData()
{
    // 查找所有的分支类图元
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
            tdl.T = 20; // 20度
        }
        tdl.updateData( true );
    }
}

// 3) 默认按照规定测风
static void InitDefWindData()
{
    // 查找所有的测风站
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WindStation" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString v;
        DataHelper::GetPropertyData( objIds[i], _T( "是否按规定测风" ), v );
        if( v.GetLength() == 0 )
        {
            DataHelper::SetPropertyData( objIds[i], _T( "是否按规定测风" ), _T( "1" ) );
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
    msg.Format( _T( "\n设置矿井的大部分巷道材质类型：" ) );
    msg.AppendFormat( _T( "\n\t0) 岩巷\n\t1) 煤巷\n\t2) 半煤岩巷" ) );
    acutPrintf( msg );

    int tm;
    if( RTNORM != acedGetInt( _T( "\n请选择一个编号{0, 1, 2}设置巷道材质类型:" ), &tm ) ) return;
    if( tm > 3 || tm < 0 )
    {
        acedAlert( _T( "请从{0, 1, 2}中选择一个编号!!!" ) );
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
        DataHelper::SetPropertyData( objIds[i], _T( "材质类型" ), v );
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
        DataHelper::GetPropertyData( objIds[i], _T( "风量" ), v );

        double d = 0;
        ArxUtilHelper::StringToDouble( v, d );

        v.Format( _T( "%.3f" ), d * 60 ); // m3/s ==> m3/min
        DataHelper::SetPropertyData( objIds[i], _T( "风量" ), v );
    }
}