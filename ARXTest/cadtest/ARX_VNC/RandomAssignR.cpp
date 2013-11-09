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

// �����������������桢����桢���ҵ�[�����Ħ������]
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

        DataHelper::SetPropertyData( objIds[i], _T( "Ħ������" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "����" ), str );
    }
}

// �������������
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
        DataHelper::SetPropertyData( objIds[i], _T( "����-��ѹ��������0����" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a0" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "����-��ѹ��������1����" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a1" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "����-��ѹ��������2����" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a2" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "����-��ѹ��������3����" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a3" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "����-��ѹ��������4����" ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "H_a4" ), str );

        DoubleToString( RandomDouble( rnd, a, b ), str );
        DataHelper::SetPropertyData( objIds[i], _T( "����-��ѹ��������5����" ), str );
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
        DataHelper::SetPropertyData( objIds[i], _T( "����" ), str );
    }
}

// ������
void RandomAssignParam()
{
    RandomAssignTunnelParam( 0.0001, 10 );
    RandomAssignFanParam( -1000, 1000 );
    RandomAssignChimneyParam( 0.0001, 10 );
}