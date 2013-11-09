#include "StdAfx.h"
#include "Eval_103.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "AirFlowCaculate.h"

// ��EvalData_103::createTableImpl()�����и�ֵ
static CString EvalDataBaseDirPath = _T( "" ); // ���۽�����ݿ�������ļ���·��

// �����ݴ�AcArray���Ƶ�std::deque
static void CopyFromAcArray( AcArray<AFCDataLink*>& ts, std::deque<AFCDataLink*>& ds )
{
    int len = ts.length();
    for( int i = 0; i < len; i++ )
    {
        ds.push_back( ts[i] );
    }
}

// ��ȡ�󾮵���˹�ȼ�����
static int ReadGasLevel()
{
    AcDbObjectId objId;
    if( !SingleDataObjectHelper::GetObjectId( _T( "����Ϣ" ), objId ) ) return GL_LOW;

    CString value;
    if( !DataHelper::GetPropertyData( objId, _T( "��˹�ȼ�" ), value ) ) return GL_LOW;

    // ����ٶ����ݸ�ʽ������ȷ��({0,1,2}--GAS_LEVEL)
    return _ttoi( value ) % 3;
}

/**********************************************************/
/************ EvalDataExtractor_103 ***********/
/**********************************************************/
EvalDataExtractor_103::EvalDataExtractor_103()
{
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_103::~EvalDataExtractor_103()
{
    clean();
}

void EvalDataExtractor_103::init()
{
    acutPrintf( _T( "\n������������" ) );
    acutPrintf( _T( "\n103 -- ��ʼ��" ) );

    // ��ȡ�󾮵���˹�ȼ���Ϣ
    gas_level = ReadGasLevel();

    // �������
    AcArray<AFCDataLink*> ts;
    AirFlowCaculate afc;
    afc.setMineGasState( !( gas_level == GL_LOW ) );

    acutPrintf( _T( "\n��������·��:%s" ), EvalDataBaseDirPath );

    CString path;
    path.Format( _T( "%s\\%s" ), EvalDataBaseDirPath, _T( "afc.mdb" ) );
    afc.doAFC( path, ts ); // ִ�з������㣬������afc.mdb���ݿ�

    // �õ����е����ݼ���
    CopyFromAcArray( ts, ds );

    acutPrintf( _T( "\n103 -- ��ʼ�����" ) );
}

void EvalDataExtractor_103::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_103::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_103::doExtract( EvalData* pEvalData )
{
    AFCDataLink* pAFC = *itr;
    EvalData_103* pVData = ( EvalData_103* )pEvalData;

    pVData->name = pAFC->name;
    pVData->q = pAFC->q;
    pVData->maxQ = pAFC->maxQ;

    itr++;
}

/**********************************************************/
/********************* EvalData_103 *******************/
/**********************************************************/
EvalData_103::EvalData_103()
{

}

void EvalData_103::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( �÷�ص����� )
    DEFINE_REAL_FIELD( ����� )
    DEFINE_REAL_FIELD( ʵ�ʷ��� )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );

    // ��ȡ���ݿ��ļ���·��
    EvalDataBaseDirPath = pTableCreator->getDBDirPath();
}

void EvalData_103::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( �÷�ص�����, name )
    WRITE_REAL_DATA( �����, maxQ )
    WRITE_REAL_DATA( ʵ�ʷ���, q )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_103 **********************/
/**********************************************************/

bool Eval_103::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_103::eval_once( EvalData* pEvalData )
{
    EvalData_103* pVData = ( EvalData_103* )pEvalData;
    bool ret = true;
    if( ( pVData->maxQ < 0 ) || ( pVData->q < 0 ) )
    {
        ret = false;
    }
    else
    {
        ret = ( pVData->q >= pVData->maxQ );
    }
    return ret;
}

/**********************************************************/
/*************** EvalFactory_103 **************/
/**********************************************************/
Eval* EvalFactory_103::createEvalObject()
{
    return new Eval_103();
}

EvalData* EvalFactory_103::createEvalDataObject()
{
    return new EvalData_103();
}

EvalDataExtractor* EvalFactory_103::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_103();
}