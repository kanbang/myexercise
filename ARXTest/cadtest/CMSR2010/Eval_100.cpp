#include "StdAfx.h"
#include "Eval_100.h"
#include "../MineGE/DrawHelper.h"

// �Ŵ�10e7
// �����ɷֵ�Ũ��ֵ�ǳ�С��ֱ�ӽ��и������ȽϿ��ܻ��е�����
#define MAGNIFY(x) x*10000000

/**********************************************************/
/************ EvalDataExtractor_100 ***********/
/**********************************************************/
EvalDataExtractor_100::EvalDataExtractor_100()
{
    acutPrintf( _T( "\n�����ɷ�����" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_100::~EvalDataExtractor_100()
{
    clean();
}

void EvalDataExtractor_100::init()
{
    acutPrintf( _T( "\n100 -- �������вɾ�����ͼԪ" ) );
    // 1���������вɾ�����ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TWSDataLink* pTws = new TWSDataLink();
        pTws->setDataSource( objIds[i] );
        if( !pTws->updateData( false ) )
        {
            // ����,���˳�
            delete pTws;
            clean();
            break;
        }
        //acutPrintf(_T("\n%name:%s\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f"),
        //	pTws->name, pTws->O2, pTws->CO2,
        //	pTws->CO, pTws->NO, pTws->SO2,
        //	pTws->H2S, pTws->NH3);
        ds.push_back( pTws );
    }
    acutPrintf( _T( "\n100 -- �������" ) );
}

void EvalDataExtractor_100::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete ( *itr );
    }
    ds.clear();
}

bool EvalDataExtractor_100::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_100::doExtract( EvalData* pEvalData )
{
    TWSDataLink* pTWS = *itr;
    EvalData_100* pGCData = ( EvalData_100* )pEvalData;

    pGCData->name = pTWS->name;
    pGCData->O2 = pTWS->O2;
    pGCData->CO2 = pTWS->CO2;
    pGCData->CO = pTWS->CO;
    pGCData->NO = pTWS->NO;
    pGCData->SO2 = pTWS->SO2;
    pGCData->H2S = pTWS->H2S;
    pGCData->NH3 = pTWS->NH3;

    itr++;
}

/**********************************************************/
/****************** EvalData_100 **************/
/**********************************************************/
void EvalData_100::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( ���������� )
    DEFINE_REAL_FIELD( ����Ũ�� )
    DEFINE_REAL_FIELD( ������̼Ũ�� )
    DEFINE_REAL_FIELD( һ����̼Ũ�� )
    DEFINE_REAL_FIELD( ������Ũ�� )
    DEFINE_REAL_FIELD( ��������Ũ�� )
    DEFINE_REAL_FIELD( ����Ũ�� )
    DEFINE_REAL_FIELD( ��Ũ�� )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

EvalData_100::EvalData_100()
{

}

void EvalData_100::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( ����������, name )
    WRITE_REAL_DATA( ����Ũ��, O2 )
    WRITE_REAL_DATA( ������̼Ũ��, CO2 )
    WRITE_REAL_DATA( һ����̼Ũ��, CO )
    WRITE_REAL_DATA( ������Ũ��, NO )
    WRITE_REAL_DATA( ��������Ũ��, SO2 )
    WRITE_REAL_DATA( ����Ũ��, H2S )
    WRITE_REAL_DATA( ��Ũ��, NH3 )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/******************* Eval_100 *****************/
/**********************************************************/

bool Eval_100::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_100::eval_once( EvalData* pEvalData )
{
    EvalData_100* pGCData = ( EvalData_100* )pEvalData;
    if( !eval_O2( pGCData->O2 ) ) return false;
    if( !eval_CO2( pGCData->CO2 ) ) return false;
    if( !eval_CO( pGCData->CO ) ) return false;
    if( !eval_NO( pGCData->NO ) ) return false;
    if( !eval_SO2( pGCData->SO2 ) ) return false;
    if( !eval_H2S( pGCData->H2S ) ) return false;
    if( !eval_NH3( pGCData->NH3 ) ) return false;
    return true;
}

bool Eval_100::eval_O2( double O2 )
{
    return ( O2 >= 20 ); // ������20%
}

bool Eval_100::eval_CO2( double CO2 )
{
    return ( MAGNIFY( CO2 ) <= MAGNIFY( 0.5 ) ); // ������0.5%
}

bool Eval_100::eval_CO( double CO )
{
    return ( MAGNIFY( CO ) <= MAGNIFY( 0.0024 ) ); // ������0.0024%
}

bool Eval_100::eval_NO( double NO )
{
    return ( MAGNIFY( NO ) <= MAGNIFY( 0.00025 ) ); // ������0.00025%
}

bool Eval_100::eval_SO2( double SO2 )
{
    return ( MAGNIFY( SO2 ) <= MAGNIFY( 0.0005 ) ); // ������0.0005%
}

bool Eval_100::eval_H2S( double H2S )
{
    return ( MAGNIFY( H2S ) <= MAGNIFY( 0.00066 ) ); //  ������0.00066%
}

bool Eval_100::eval_NH3( double NH3 )
{
    return ( MAGNIFY( NH3 ) <= MAGNIFY( 0.004 ) ); // ������0.004%
}

/**********************************************************/
/*************** EvalFactory_100 **************/
/**********************************************************/
Eval* EvalFactory_100::createEvalObject()
{
    return new Eval_100();
}

EvalData* EvalFactory_100::createEvalDataObject()
{
    return new EvalData_100();
}

EvalDataExtractor* EvalFactory_100::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_100();
}