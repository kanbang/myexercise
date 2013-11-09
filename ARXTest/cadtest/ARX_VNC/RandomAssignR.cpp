#include "stdafx.h"

#include "../MineGE/HelperClass.h"

#include "../Tool/MyRandom.h"
//#include "../Tool/Timer.h"

static double RandomDouble( MyRandom& rnd, double a, double b )
{
    return rnd.randomDouble() * ( b - a ) + a;
}

static void DoubleToString( double d, CString& str )
{
    str.Format( _T( "%.5f" ), d );
}

// 随机分配巷道、工作面、掘进面、硐室的[风阻和摩擦风阻]
static void RandomAssignTunnelParam( double a, double b )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );
    if( objIds.isEmpty() ) return;

    MyRandom rnd;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString str;
        DoubleToString( RandomDouble( rnd, a, b ), str );

        DataHelper::SetPropertyData( objIds[i], _T( "摩擦风阻" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风阻" ), str );
    }
}

// 随机分配风机参数
static void RandomAssignFanParam( double a, double b )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Fan" ), objIds );
    if( objIds.isEmpty() ) return;

    MyRandom rnd;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString str;
        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风量-风压特性曲线0次项" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a0" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风量-风压特性曲线1次项" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a1" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风量-风压特性曲线2次项" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a2" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风量-风压特性曲线3次项" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a3" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风量-风压特性曲线4次项" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a4" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风量-风压特性曲线5次项" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a5" ), str );
    }
}

static void RandomAssignChimneyParam( double a, double b )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Chimney" ), objIds );
    if( objIds.isEmpty() ) return;

    MyRandom rnd;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString str;
        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "风阻" ), str );
    }
}

// 测试用
void RandomAssignParam()
{
    RandomAssignTunnelParam( 0.0001, 10 );
    RandomAssignFanParam( -1000, 1000 );
    RandomAssignChimneyParam( 0.0001, 10 );
}