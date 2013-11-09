#include "StdAfx.h"
#include "Eval_107.h"

#include "../MineGE/HelperClass.h"

#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

static void AppendPathName( const AcDbObjectIdArray& objIds, AcStringArray& p )
{
    p.removeAll();
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
/************ EvalDataExtractor_107 ***********/
/**********************************************************/
EvalDataExtractor_107::EvalDataExtractor_107()
{
    acutPrintf( _T( "\n����·������" ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_107::~EvalDataExtractor_107()
{
    clean();
}

void EvalDataExtractor_107::init()
{
    acutPrintf( _T( "\n107 -- �����÷�ص����ͨ��·��" ) );

    // �����÷�ص�
    AcDbObjectIdArray objIds;
    FIPHelper fip;
    fip.findAirEdges( objIds );
    acutPrintf( _T( "\n�÷�ص������%d" ), objIds.length() );

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString name;
        DataHelper::GetPropertyData( objIds[i], _T( "����" ), name );

        acutPrintf( _T( "\n��%d���÷�ص㣺��%s����ʼfip..." ), i, name );
        AcDbObjectIdArray p1, p2;
        if( !fip.doFIP( objIds[i], p1, p2 ) ) continue;

        acutPrintf( _T( "\n��%d���÷�ص�ɹ�����fip..." ), i );

        FipDataLink* pFD = new FipDataLink();
        pFD->setDataSource( objIds[i] );
        if( !pFD->updateData( false ) )
        {
            //acutPrintf(_T("\n107�������ݴ���"));
            delete pFD;
            clean();
            return;
        }
        AppendPathName( p1, pFD->p1 );
        AppendPathName( p2, pFD->p2 );
        bool ret1 = fip.isIncludeAirEdge( p1 );
        bool ret2 = fip.isIncludeAirEdge( p2 );
        if( ret1 ) acutPrintf( _T( "\n****��%d���÷�ص�ġ�����ͨ·�С���������ͨ��" ), i );
        if( ret2 ) acutPrintf( _T( "\n****��%d���÷�ص�ġ��ط�ͨ·���а�������ͨ��" ), i );
        pFD->includeAir =  ret1 || ret2;

        ds.push_back( pFD ); // ��ӵ�������
    }
    acutPrintf( _T( "\n107 -- �������" ) );
}

void EvalDataExtractor_107::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete *itr;
    }
    ds.clear();
}

bool EvalDataExtractor_107::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_107::doExtract( EvalData* pEvalData )
{
    FipDataLink* pFD = *itr;
    EvalData_107* pVData = ( EvalData_107* )pEvalData;

    pVData->name = pFD->name;
    pVData->p1.append( pFD->p1 );
    pVData->p2.append( pFD->p2 );
    pVData->includeAir = pFD->includeAir;

    itr++;
}

/**********************************************************/
/********************* EvalData_107 *******************/
/**********************************************************/
EvalData_107::EvalData_107()
{

}

void EvalData_107::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_COMMENT_FIELD( ����ͨ· )
    DEFINE_STRING_FIELD( �����÷�ص� )
    DEFINE_COMMENT_FIELD( �ط�ͨ· )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_107::writeToTableImpl( DataWriter* pDataWriter )
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
/********************** Eval_107 **********************/
/**********************************************************/

bool Eval_107::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_107::eval_once( EvalData* pEvalData )
{
    EvalData_107* pVData = ( EvalData_107* )pEvalData;
    return !( pVData->includeAir );
}

/**********************************************************/
/*************** EvalFactory_107 **************/
/**********************************************************/
Eval* EvalFactory_107::createEvalObject()
{
    return new Eval_107();
}

EvalData* EvalFactory_107::createEvalDataObject()
{
    return new EvalData_107();
}

EvalDataExtractor* EvalFactory_107::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_107();
}