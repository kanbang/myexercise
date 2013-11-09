#include "StdAfx.h"

#include "FFC_Data.h"
#include "GasParam.h"
#include "GasParamHelper.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* ȫ�ֺ���(Tool.cpp) */
extern void GetHosts( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts );
extern void FindAllObturations( AcDbObjectIdArray& objIds );

static double GetDoubleData( const AcDbObjectId& objId, const CString& field )
{
    if( objId.isNull() ) return 0;

    CString value;
    DataHelper::GetPropertyData( objId, field, value );

    double q = 0;
    ArxUtilHelper::StringToDouble( value, q );
    return q;
}

static void GetPointSourceData( const AcDbObjectIdArray& allObjIds, const AcDbObjectIdArray& hosts, const AcDbObjectId& objId, ObturationData& data )
{
    // �����������������ܱ�
    AcDbObjectIdArray objIds;
    for( int i = 0; i < hosts.length(); i++ )
    {
        if( hosts[i] == objId )
        {
            objIds.append( allObjIds[i] );
        }
    }

    double q = 0;
    double ch4 = 0;
    double o2 = 0;

    // ��������û���ܱգ�ֱ�Ӷ�ȡ��������
    if( objIds.isEmpty() )
    {
        q = abs( GetDoubleData( objId, _T( "����" ) ) );
        ch4 = abs( GetDoubleData( objId, _T( "��˹Ũ��" ) ) );
        o2 = abs( GetDoubleData( objId, _T( "����Ũ��" ) ) );
    }
    else
    {
        // ������ܱգ����ȡ©��������
        // ��ȡ���е��ܱ����ݣ���ȡ����©������
        for( int i = 0; i < objIds.length(); i++ )
        {
            double lq = abs( GetDoubleData( objIds[i], _T( "©����" ) ) );
            if( q < lq ) q = lq; // ȡ����������

            double lch4 = abs( GetDoubleData( objIds[i], _T( "��˹Ũ��" ) ) );
            if( ch4 < lch4 ) ch4 = lch4; // ȡ�����˹Ũ��

            double lo2 = abs( GetDoubleData( objIds[i], _T( "����Ũ��" ) ) );
            if( o2 < lo2 ) o2 = lo2; // ȡ�������Ũ��
        }
    }

    // ��������
    data.q = q;
    data.ch4 = ch4;
    data.o2 = o2;
}

void GetObturationData( const AcDbObjectIdArray& ob_objIds, const AcDbIntArray& ob_types, ObturationDataArray& ob_datas )
{
    // �������е��ܱ�
    AcDbObjectIdArray allObjIds;
    FindAllObturations( allObjIds );

    // �ܱչ��������
    AcDbObjectIdArray hosts;
    GetHosts( allObjIds, hosts );

    for( int i = 0; i < ob_objIds.length(); i++ )
    {
        ObturationData data;
        GetPointSourceData( allObjIds, hosts, ob_objIds[i], data );
        // ob_types[i] = 1��ʾʼ�����ڲɿ����ϣ���ɿ�����©��
        // ob_types[i] = -1��ʾĩ�����ڲɿ����ϣ���ɿ�����©��
        data.q = -1 * ob_types[i] * ( data.q );
        ob_datas.append( data );
    }
}

void GetInletBoundaryData( const AcDbObjectIdArray& objIds, InletDataArray& datas )
{
    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        double area = GetDoubleData( objIds[i], _T( "�������" ) );
        double q = GetDoubleData( objIds[i], _T( "����" ) );
        double ch4 = GetDoubleData( objIds[i], _T( "����Ũ��" ) );
        double o2 = GetDoubleData( objIds[i], _T( "����Ũ��" ) );
        double t = GetDoubleData( objIds[i], _T( "�¶�" ) );

        InletData data;
        data.area = abs( area );
        data.q = abs( q );
        data.ch4 = abs( ch4 );
        data.o2 = abs( o2 );
        data.t = t;  // �¶ȿ���������
        datas.append( data );
    }
}

void GetGasBoundaryData( const AcDbObjectIdArray& gas_objIds, GasParam& gp, GasBoundaryDataArray& gas_datas )
{
    // ��ȡȫ�ֵ���˹����
    GasParamHelper::ReadGasParam( gp );

    // ��ȡ���������˹����
    int n = gas_objIds.length();
    for( int i = 0; i < n; i++ )
    {
        GasBoundaryData data;
        data.q = GetDoubleData( gas_objIds[i], _T( "��������˹ӿ����" ) );
        gas_datas.append( data );
    }
}

void GetAirLeakData( const AcDbObjectIdArray& al_objIds, AirLeakDataArray& al_datas )
{
    for( int i = 0; i < al_objIds.length(); i++ )
    {
        double q = GetDoubleData( al_objIds[i], _T( "©����" ) );
        if( ArxUtilHelper::IsEqualType( _T( "SourceAirLeak" ), al_objIds[i] ) )
        {
            q = abs( q );
        }
        else
        {
            q = -1 * abs( q );
        }

        AirLeakData data;
        data.q = q;
        al_datas.append( data );
    }
}

void GetGasPipeData( const AcDbObjectIdArray& gas_pipe_objIds, GasPipeDataArray& gas_pipe_datas )
{
    for( int i = 0; i < gas_pipe_objIds.length(); i++ )
    {
        AcDbObjectId objId = gas_pipe_objIds[i];
        double q = GetDoubleData( objId, _T( "�����" ) );
        double ch4 = GetDoubleData( objId, _T( "����Ũ��" ) );
        double o2 = GetDoubleData( objId, _T( "����Ũ��" ) );

        GasPipeData data;
        data.q = abs( q );
        data.ch4 = abs( ch4 );
        data.o2 = abs( o2 );
        gas_pipe_datas.append( data );
    }
}

void GetNitrogenPipeData( const AcDbObjectIdArray& n2_pipe_objIds, NitrogenPipeDataArray& n2_pipe_datas )
{
    for( int i = 0; i < n2_pipe_objIds.length(); i++ )
    {
        AcDbObjectId objId = n2_pipe_objIds[i];
        double n2 = GetDoubleData( objId, _T( "ע����" ) );

        NitrogenPipeData data;
        data.n2 = abs( n2 );
        n2_pipe_datas.append( data );
    }
}