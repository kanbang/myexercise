#include "StdAfx.h"
#include "Eval_106.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

/**********************************************************/
/************ EvalDataExtractor_106 ***********/
/**********************************************************/
EvalDataExtractor_106::EvalDataExtractor_106()
{
    acutPrintf( _T( "\n��ȫ�Ǳ�����" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_106::~EvalDataExtractor_106()
{
    clean();
}

void EvalDataExtractor_106::init()
{
    acutPrintf( _T( "\n106 -- ���Ұ�ȫ�Ǳ�" ) );

    AcStringArray names;
    DataObjectListHelper::GetAllNames( names );
    if( names.isEmpty() || !names.contains( _T( "��ȫ����Ǳ�" ) ) ) return;

    // ��ȡ���а�ȫ�Ǳ�
    AcStringArray values;
    DataObjectListHelper::GetObjectNameList( _T( "��ȫ����Ǳ�" ), values );
    if( values.isEmpty() ) return;

    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        InstrumentDataLink* pID = new InstrumentDataLink();
        AcDbObjectId objId;
        DataObjectListHelper::GetObjectId( _T( "��ȫ����Ǳ�" ), values[i].kACharPtr(), objId );
        pID->setDataSource( objId );
        if( !pID->updateData( false ) )
        {
            delete pID;
            clean();
            break;
        }
        ds.push_back( pID );
    }
    acutPrintf( _T( "\n106 -- �������" ) );
}

void EvalDataExtractor_106::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_106::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_106::doExtract( EvalData* pEvalData )
{
    InstrumentDataLink* pID = *itr;
    EvalData_106* pVData = ( EvalData_106* )pEvalData;

    pVData->name = pID->name;
    pVData->mn = pID->mn;
    pVData->amount = pID->amount;
    pVData->qi = pID->qi;

    itr++;
}

/**********************************************************/
/********************* EvalData_106 *******************/
/**********************************************************/
EvalData_106::EvalData_106()
{

}

void EvalData_106::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( �Ǳ����� )
    DEFINE_STRING_FIELD( �Ǳ��ͺ� )
    DEFINE_INT_FIELD( �Ǳ����� )
    DEFINE_STRING_FIELD( ���������� )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_106::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( �Ǳ�����, name )
    WRITE_STRING_DATA( �Ǳ��ͺ�, mn )
    WRITE_INT_DATA( �Ǳ�����, amount )
    if( amount > 0 )
    {
        WRITE_STRING_DATA( ����������, ( qi ? _T( "�ϸ�" ) : _T( "���ϸ�" ) ) )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_106 **********************/
/**********************************************************/

bool Eval_106::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_106::eval_once( EvalData* pEvalData )
{
    EvalData_106* pVData = ( EvalData_106* )pEvalData;
    bool ret = ( pVData->amount == 0 );
    if( !ret )
    {
        ret = pVData->qi;
    }
    return ret;
}

/**********************************************************/
/*************** EvalFactory_106 **************/
/**********************************************************/
Eval* EvalFactory_106::createEvalObject()
{
    return new Eval_106();
}

EvalData* EvalFactory_106::createEvalDataObject()
{
    return new EvalData_106();
}

EvalDataExtractor* EvalFactory_106::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_106();
}