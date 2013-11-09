#include "StdAfx.h"
#include "Eval_115.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

static bool IsDown( double iz, double oz )
{
    return iz > oz;
}

/**********************************************************/
/************ EvalDataExtractor_115 ***********/
/**********************************************************/
EvalDataExtractor_115::EvalDataExtractor_115()
{
    acutPrintf( _T( "\n����ͨ������" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_115::~EvalDataExtractor_115()
{
    clean();
}

void EvalDataExtractor_115::init()
{
    acutPrintf( _T( "\n115 -- ���ҹ�����" ) );

    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        WSDataLink* pWS = new WSDataLink();
        pWS->setDataSource( objIds[i] );
        if( !pWS->updateData( false ) )
        {
            delete pWS;
            return;
        }

        pWS->down = IsDown( pWS->iz, pWS->oz ); // �Ƿ�����ͨ��
        ds.push_back( pWS );
    }
    acutPrintf( _T( "\n115 -- �������" ) );
}

void EvalDataExtractor_115::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_115::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_115::doExtract( EvalData* pEvalData )
{
    WSDataLink* pWS = *itr;
    EvalData_115* pVData = ( EvalData_115* )pEvalData;

    pVData->name = pWS->name;
    pVData->hasRisk = pWS->hasRisk;
    pVData->iz = pWS->iz;
    pVData->oz = pWS->oz;
    pVData->down = pWS->down;

    itr++;
}

/**********************************************************/
/********************* EvalData_115 *******************/
/**********************************************************/
EvalData_115::EvalData_115()
{

}

void EvalData_115::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( ���������� )
    DEFINE_STRING_FIELD( �Ƿ���ͻ��Σ�� )
    DEFINE_REAL_FIELD( ���������� )
    DEFINE_REAL_FIELD( �ط������� )
    DEFINE_STRING_FIELD( �Ƿ�����ͨ�� )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_115::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( ����������, name )
    if( hasRisk )
    {
        WRITE_STRING_DATA( �Ƿ���ͻ��Σ��, _T( "��" ) )
    }
    else
    {
        WRITE_STRING_DATA( �Ƿ���ͻ��Σ��, _T( "��" ) )
    }
    WRITE_REAL_DATA( ����������, iz )
    WRITE_REAL_DATA( �ط�������, oz )
    if( down )
    {
        WRITE_STRING_DATA( �Ƿ�����ͨ��, _T( "��" ) )
    }
    else
    {
        WRITE_STRING_DATA( �Ƿ�����ͨ��, _T( "��" ) )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_115 **********************/
/**********************************************************/

bool Eval_115::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_115::eval_once( EvalData* pEvalData )
{
    EvalData_115* pVData = ( EvalData_115* )pEvalData;
//	return (pVData->hasRisk || pVData->down);
    return !( pVData->hasRisk ) ? true : !( pVData->down );
}

/**********************************************************/
/*************** EvalFactory_115 **************/
/**********************************************************/
Eval* EvalFactory_115::createEvalObject()
{
    return new Eval_115();
}

EvalData* EvalFactory_115::createEvalDataObject()
{
    return new EvalData_115();
}

EvalDataExtractor* EvalFactory_115::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_115();
}