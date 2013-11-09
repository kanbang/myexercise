#include "StdAfx.h"
#include "Eval_109.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

/* ȫ�ֺ���(ʵ����Tool.cpp��) */
extern bool HasGate( const AcDbObjectId& objId );

// ����ַ���Ϊ�հף���ĳ�NULL
static void EmptyString2NULL( CString& name )
{
    name.Trim();
    if( name.GetLength() == 0 )
    {
        name = "NULL";
    }
}

/**********************************************************/
/************ EvalDataExtractor_109 ***********/
/**********************************************************/
EvalDataExtractor_109::EvalDataExtractor_109()
{
    acutPrintf( _T( "\n����������" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_109::~EvalDataExtractor_109()
{
    clean();
}

static bool GetEdgeName( const AcDbObjectId& objId, CString& name )
{
    bool ret = true;
    ILTDataLink tn;
    tn.setDataSource( objId );
    if( !tn.updateData( false ) )
    {
        ret = false;
    }
    else
    {
        name = tn.name;
    }
    return ret;
}

// ʹ��datalink�Ƚϵ��鷳��Ҳ����ʹ�������ķ���
// ����DataHelper�е�get/set����
void EvalDataExtractor_109::init()
{
    acutPrintf( _T( "\n109 -- ����������" ) );

    // ����������
    AcDbObjectIdArray objIds;
    ILTHelper ilt;
    if( !ilt.doILT( objIds ) ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObjectId objId = objIds[i];

        // �������������ߺͳ������
        AcDbObjectIdArray s1, s2;
        if( !ilt.findInOutEdges( objId, s1, s2 ) ) continue;

        ILTDataLink* pTN = new ILTDataLink();
        pTN->setDataSource( objId );
        if( !pTN->updateData( false ) )
        {
            delete pTN;
            break;
        }

        if( !s1.isEmpty() )
        {
            GetEdgeName( s1[0], pTN->inEdgeName ); // ��ȡ����������
        }
        if( !s2.isEmpty() )
        {
            GetEdgeName( s2[0], pTN->outEdgeName ); // ��ȡ�����������
        }

        // �ж����������Ƿ��й�����
        pTN->hasWall = HasGate( objId );

        ds.push_back( pTN );
    }
    acutPrintf( _T( "\n109 -- �������" ) );
}

void EvalDataExtractor_109::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_109::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_109::doExtract( EvalData* pEvalData )
{
    ILTDataLink* pTN = *itr;
    EvalData_109* pVData = ( EvalData_109* )pEvalData;

    pVData->name = pTN->name;
    pVData->inEdgeName = pTN->inEdgeName;
    pVData->outEdgeName = pTN->outEdgeName;
    pVData->hasWall = pTN->hasWall;

    itr++;
}

/**********************************************************/
/********************* EvalData_109 *******************/
/**********************************************************/
EvalData_109::EvalData_109()
{

}

void EvalData_109::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( �������� )
    DEFINE_STRING_FIELD( ���ط��� )
    DEFINE_STRING_FIELD( ������ )
    DEFINE_STRING_FIELD( ������ )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_109::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    EmptyString2NULL( inEdgeName );
    WRITE_STRING_DATA( ��������, inEdgeName )
    EmptyString2NULL( outEdgeName );
    WRITE_STRING_DATA( ���ط���, outEdgeName )
    EmptyString2NULL( name );
    WRITE_STRING_DATA( ������, name )
    if( hasWall )
    {
        WRITE_STRING_DATA( ������, _T( "��" ) )
    }
    else
    {
        WRITE_STRING_DATA( ������, _T( "��" ) )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_109 **********************/
/**********************************************************/

bool Eval_109::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_109::eval_once( EvalData* pEvalData )
{
    EvalData_109* pVData = ( EvalData_109* )pEvalData;
    return ( pVData->hasWall );
}

/**********************************************************/
/*************** EvalFactory_109 **************/
/**********************************************************/
Eval* EvalFactory_109::createEvalObject()
{
    return new Eval_109();
}

EvalData* EvalFactory_109::createEvalDataObject()
{
    return new EvalData_109();
}

EvalDataExtractor* EvalFactory_109::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_109();
}