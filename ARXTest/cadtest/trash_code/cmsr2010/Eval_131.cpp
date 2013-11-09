#include "StdAfx.h"
#include "Eval_131.h"

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
/************ EvalDataExtractor_131 ***********/
/**********************************************************/
EvalDataExtractor_131::EvalDataExtractor_131()
{
    acutPrintf( _T( "\n����·������" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_131::~EvalDataExtractor_131()
{
    clean();
}

void EvalDataExtractor_131::init()
{
    acutPrintf( _T( "\n131 -- ���ҳ���Ҷ���ͨ��·��" ) );

    // �������еı�ը���Ͽ�
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "ChargeRoom" ), objIds );

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
            //acutPrintf(_T("\n131�������ݴ���"));
            delete pFD;
            clean();
            return;
        }
        AppendPathName( p1, pFD->p1 );
        AppendPathName( p2, pFD->p2 );
        pFD->includeAir = ( fip.isIncludeAirEdge( p1 ) || fip.isIncludeAirEdge( p2 ) );

        ds.push_back( pFD ); // ��ӵ�������
    }
    acutPrintf( _T( "\n131 -- �������" ) );
}

void EvalDataExtractor_131::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_131::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_131::doExtract( EvalData* pEvalData )
{
    FipDataLink* pFD = *itr;
    EvalData_131* pVData = ( EvalData_131* )pEvalData;

    pVData->name = pFD->name;
    pVData->p1.append( pFD->p1 );
    pVData->p2.append( pFD->p2 );
    pVData->includeAir = pFD->includeAir;

    itr++;
}

/**********************************************************/
/********************* EvalData_131 *******************/
/**********************************************************/
EvalData_131::EvalData_131()
{

}

void EvalData_131::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_COMMENT_FIELD( ����ͨ· )
    DEFINE_STRING_FIELD( �����÷�ص� )
    DEFINE_COMMENT_FIELD( �ط�ͨ· )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_131::writeToTableImpl( DataWriter* pDataWriter )
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
/********************** Eval_131 **********************/
/**********************************************************/

bool Eval_131::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_131::eval_once( EvalData* pEvalData )
{
    EvalData_131* pVData = ( EvalData_131* )pEvalData;
    return !( pVData->includeAir );
}

/**********************************************************/
/*************** EvalFactory_131 **************/
/**********************************************************/
Eval* EvalFactory_131::createEvalObject()
{
    return new Eval_131();
}

EvalData* EvalFactory_131::createEvalDataObject()
{
    return new EvalData_131();
}

EvalDataExtractor* EvalFactory_131::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_131();
}