#include "StdAfx.h"
#include "Eval_132.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

static void AppendPathName( const AcDbObjectIdArray& objIds, AcStringArray& p )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelNameDataLink tn;
        tn.setDataSource( objIds[i] );
        tn.updateData( false ); // ����Ͳ���������
        //acutPrintf(_T("\n\t���ӷ�֧:%s"), tn.name);
        p.append( tn.name );
    }
}

// ����ַ���Ϊ�հף���ĳ�NULL
static void EmptyString2NULL( CString& name )
{
    name.Trim();
    if( name.GetLength() == 0 )
    {
        name = "NULL";
    }
}

// ��·���ַ�����"->"��������
static CString MakePathString( AcStringArray& p )
{
    CString str;
    int len = p.length();
    for( int i = 0; i < len; i++ )
    {
        CString name = p[i].kACharPtr();
        EmptyString2NULL( name );
        str.AppendFormat( _T( "%s->" ), name );
    }
    return str;
}

/**********************************************************/
/************ EvalDataExtractor_132 ***********/
/**********************************************************/
EvalDataExtractor_132::EvalDataExtractor_132()
{
    acutPrintf( _T( "\n����·������" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_132::~EvalDataExtractor_132()
{
    clean();
}

void EvalDataExtractor_132::init()
{
    acutPrintf( _T( "\n132 -- ���һ������Ҷ���ͨ��·��" ) );

    // �������еı�ը���Ͽ�
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MachineRoom" ), objIds );

    FIPHelper fip;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        //acutPrintf(_T("\n��%d���÷�ص㿪ʼfip..."), i);
        AcDbObjectIdArray p1, p2;
        if( !fip.doFIP( objIds[i], p1, p2 ) ) continue;

        //acutPrintf(_T("\n��%d���÷�ص�ɹ�����fip..."), i);

        FipDataLink* pFD = new FipDataLink();
        pFD->setDataSource( objIds[i] );
        if( !pFD->updateData( false ) )
        {
            //acutPrintf(_T("\n132�������ݴ���"));
            delete pFD;
            clean();
            return;
        }
        AppendPathName( p1, pFD->p1 );
        AppendPathName( p2, pFD->p2 );
        pFD->includeAir = ( fip.isIncludeAirEdge( p1 ) || fip.isIncludeAirEdge( p2 ) );

        ds.push_back( pFD ); // ��ӵ�������
    }
    acutPrintf( _T( "\n132 -- �������" ) );
}

void EvalDataExtractor_132::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_132::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_132::doExtract( EvalData* pEvalData )
{
    FipDataLink* pFD = *itr;
    EvalData_132* pVData = ( EvalData_132* )pEvalData;

    pVData->name = pFD->name;
    pVData->p1.append( pFD->p1 );
    pVData->p2.append( pFD->p2 );
    pVData->includeAir = pFD->includeAir;

    itr++;
}

/**********************************************************/
/********************* EvalData_132 *******************/
/**********************************************************/
EvalData_132::EvalData_132()
{

}

void EvalData_132::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_COMMENT_FIELD( ����ͨ· )
    DEFINE_STRING_FIELD( �����÷�ص� )
    DEFINE_COMMENT_FIELD( �ط�ͨ· )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_132::writeToTableImpl( DataWriter* pDataWriter )
{
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( ����ͨ·, MakePathString( p1 ) )
    EmptyString2NULL( name );
    WRITE_STRING_DATA( �����÷�ص�, name )
    WRITE_STRING_DATA( �ط�ͨ·, MakePathString( p2 ) )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_132 **********************/
/**********************************************************/

bool Eval_132::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_132::eval_once( EvalData* pEvalData )
{
    EvalData_132* pVData = ( EvalData_132* )pEvalData;
    return !( pVData->includeAir );
}

/**********************************************************/
/*************** EvalFactory_132 **************/
/**********************************************************/
Eval* EvalFactory_132::createEvalObject()
{
    return new Eval_132();
}

EvalData* EvalFactory_132::createEvalDataObject()
{
    return new EvalData_132();
}

EvalDataExtractor* EvalFactory_132::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_132();
}