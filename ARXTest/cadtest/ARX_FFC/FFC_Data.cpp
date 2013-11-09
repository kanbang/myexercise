#include "StdAfx.h"

#include "FFC_Data.h"
#include "GasParam.h"
#include "GasParamHelper.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* 全局函数(Tool.cpp) */
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

static double GetPointSourceData( const AcDbObjectIdArray& allObjIds, const AcDbObjectIdArray& hosts, const AcDbObjectId& objId )
{
    // 查找与该巷道关联的密闭
    AcDbObjectIdArray objIds;
    for( int i = 0; i < hosts.length(); i++ )
    {
        if( hosts[i] == objId )
        {
            objIds.append( allObjIds[i] );
        }
    }

    double q = 0;
    // 如果巷道上没有密闭，直接读取风量数据
    if( objIds.isEmpty() )
    {
        CString value;
        DataHelper::GetPropertyData( objId, _T( "风量" ), value );
        ArxUtilHelper::StringToDouble( value, q );
        q = abs( q );
    }
    else
    {
        // 如果有密闭，则读取漏风量数据
        // 读取所有的密闭数据，读取最大的漏风数据
        for( int i = 0; i < objIds.length(); i++ )
        {
            CString value;
            DataHelper::GetPropertyData( objIds[i], _T( "漏风量" ), value );
            double lq = 0;
            ArxUtilHelper::StringToDouble( value, lq );
            lq = abs( lq );
            if( q < lq ) q = lq; // 取最大风量数据
        }
    }
    return q;
}

void GetObturationData( const AcDbObjectIdArray& ob_objIds, const AcDbIntArray& ob_types, ObturationDataArray& ob_datas )
{
    // 查找所有的密闭
    AcDbObjectIdArray allObjIds;
    FindAllObturations( allObjIds );

    // 密闭关联的巷道
    AcDbObjectIdArray hosts;
    GetHosts( allObjIds, hosts );

    for( int i = 0; i < ob_objIds.length(); i++ )
    {
        double q = 	GetPointSourceData( allObjIds, hosts, ob_objIds[i] );

        // ob_types[i] = 1表示始点落在采空区上，向采空区外漏风
        // ob_types[i] = -1表示末点落在采空区上，向采空区内漏风
        ObturationData data;
        data.q = -1 * ob_types[i] * q;
        ob_datas.append( data );
    }
}

void GetInletBoundaryData( const AcDbObjectIdArray& objIds, InletDataArray& datas )
{
    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        double area = GetDoubleData( objIds[i], _T( "断面面积" ) );
        double q = GetDoubleData( objIds[i], _T( "风量" ) );
        double ch4 = GetDoubleData( objIds[i], _T( "甲烷浓度" ) );
        double o2 = GetDoubleData( objIds[i], _T( "氧气浓度" ) );

        InletData data;
        data.area = abs( area );
        data.q = abs( q );
        data.ch4 = abs( ch4 );
        data.o2 = abs( o2 );
        datas.append( data );
    }
}

void GetGasBoundaryData( const AcDbObjectIdArray& gas_objIds, GasParam& gp, GasBoundaryDataArray& gas_datas )
{
    // 读取全局的瓦斯数据
    GasParamHelper::ReadGasParam( gp );

    // 读取工作面的瓦斯数据
    int n = gas_objIds.length();
    for( int i = 0; i < n; i++ )
    {
        GasBoundaryData data;
        data.q = GetDoubleData( gas_objIds[i], _T( "工作面瓦斯涌出量" ) );
        gas_datas.append( data );
    }
}

void GetAirLeakData( const AcDbObjectIdArray& al_objIds, AirLeakDataArray& al_datas )
{
    for( int i = 0; i < al_objIds.length(); i++ )
    {
        double q = GetDoubleData( al_objIds[i], _T( "漏风量" ) );
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
        double q = GetDoubleData( objId, _T( "抽放量" ) );
        double ch4 = GetDoubleData( objId, _T( "甲烷浓度" ) );
        double o2 = GetDoubleData( objId, _T( "氧气浓度" ) );

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
        double n2 = GetDoubleData( objId, _T( "注氮量" ) );

        NitrogenPipeData data;
        data.n2 = abs( n2 );
        n2_pipe_datas.append( data );
    }
}