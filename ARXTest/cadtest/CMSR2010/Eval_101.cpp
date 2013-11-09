#include "StdAfx.h"
#include "Eval_101.h"

#include "../MineGE/DrawHelper.h"

#define MIN_VELOCITY 0
#define MAX_VELOCITY 100

/* ȫ�ֺ���(ʵ����Tool.cpp��) */
extern int FindShafts( AcDbObjectIdArray& shafts );

/**********************************************************/
/************ EvalDataExtractor_101 ***********/
/**********************************************************/
EvalDataExtractor_101::EvalDataExtractor_101()
{
    acutPrintf( _T( "\n��������..." ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_101::~EvalDataExtractor_101()
{
    clean();
}

void EvalDataExtractor_101::init()
{
    init1();
    init2();
    init3();
    //init4();
}

void EvalDataExtractor_101::init1()
{
    acutPrintf( _T( "\101 -- �������лزɹ�����ͼԪ" ) );
    // 1���������лزɹ�����ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        WSVelocityDataLink* pWvm = new WSVelocityDataLink();
        pWvm->setDataSource( objIds[i] );
        if( !pWvm->updateData( false ) )
        {
            // ����,���˳�
            delete pWvm;
            clean();
            break;
        }
        //acutPrintf(_T("\n%name:%s\t%.4f\t%.4f\t%d\t%d"), pWvm->name, pWvm->v, pWvm->q_ch4, pWvm->tm, pWvm->cm);
        ds.push_back( pWvm );
    }
    acutPrintf( _T( "\n101 -- �������" ) );
}

void EvalDataExtractor_101::init2()
{
    acutPrintf( _T( "\101 -- �������о��������ͼԪ" ) );

    // 1���������о��������ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TWSVelocityDataLink* pTWvm = new TWSVelocityDataLink();
        pTWvm->setDataSource( objIds[i] );
        if( !pTWvm->updateData( false ) )
        {
            // ����,���˳�
            delete pTWvm;
            clean();
            break;
        }
        //acutPrintf(_T("\n%name:%s\t%.4f\t%.4f\t%d"), pTWvm->name, pTWvm->v, pTWvm->q_ch4, pTWvm->tm);
        ds.push_back( pTWvm );
    }
    acutPrintf( _T( "\n101 -- �������" ) );
}

void EvalDataExtractor_101::init3()
{
    acutPrintf( _T( "\101 -- �����������ͼԪ" ) );

    // 1�����ҽ��羮�ͻط羮
    AcDbObjectIdArray shafts;
    int pos = FindShafts( shafts );

    // 1�������������ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TunnelVelocityDataLink* pTvm = new TunnelVelocityDataLink();
        pTvm->setDataSource( objIds[i] );
        if( !pTvm->updateData( false ) )
        {
            // ����,���˳�
            delete pTvm;
            clean();
            break;
        }
        pTvm->isShaft = shafts.contains( objIds[i] ); // �Ƿ�Ͳ

        // �����������ͨ������������ۣ����ܵ���word��������ٶȷǳ���
        if( !( pTvm->isShaft ) && pTvm->ft == 0 ) continue; // �ų���ͨ���

        //acutPrintf(_T("\n%name:%s\t%.4f\t%.4f"), pTvm->name, pTvm->v, pTvm->q_ch4);

        if( pTvm->isShaft )
        {
            ds.push_front( pTvm ); // ����Ǿ�Ͳ���ͷ��ڶ��е�ǰ��
        }
        else
        {
            ds.push_back( pTvm );
        }
    }
    acutPrintf( _T( "\n101 -- �������" ) );
}

void EvalDataExtractor_101::init4()
{
    acutPrintf( _T( "\101 -- ���ҷ���ͼԪ" ) );

    // 1���������з���ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WindBridge" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        BridgeVelocityDataLink* pBv = new BridgeVelocityDataLink();
        pBv->setDataSource( objIds[i] );
        if( !pBv->updateData( false ) )
        {
            // ����,���˳�
            delete pBv;
            clean();
            break;
        }
        acutPrintf( _T( "\n%name:%s\t%.4f\t%.4f" ), pBv->name, pBv->v, pBv->q_ch4 );
        ds.push_back( pBv );
    }
    acutPrintf( _T( "\n101 -- �������" ) );
}

void EvalDataExtractor_101::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete ( *itr );
    }
    ds.clear();
}

bool EvalDataExtractor_101::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_101::doExtract( EvalData* pEvalData )
{
    VelocityDataLink* pVM = *itr;
    EvalData_101* pVData = ( EvalData_101* )pEvalData;

    pVData->name = pVM->name;
    pVData->q_ch4 = pVM->q_ch4;
    pVData->v = pVM->v;
    pVData->minV = 0;
    pVData->maxV = 0;

    switch( pVM->type )
    {
    case VD_TUNNEL:
    {
        TunnelVelocityDataLink* pTVM = ( TunnelVelocityDataLink* )pVM;
        pVData->ft = pTVM->ft;
        pVData->sp = pTVM->sp;
        pVData->isShaft = pTVM->isShaft;
        initMinMaxVelocity1( pVData );
    }
    break;

    case VD_WS:
    {
        WSVelocityDataLink* pWVM = ( WSVelocityDataLink* )pVM;
        pVData->tm = pWVM->tm;
        pVData->cm = pWVM->cm;
        initMinMaxVelocity2( pVData );
    }
    break;

    case VD_TWS:
    {
        TWSVelocityDataLink* pTWVM = ( TWSVelocityDataLink* )pVM;
        pVData->tm = pTWVM->tm;
        initMinMaxVelocity3( pVData );
    }
    break;
    case VD_BRIDGE:
    {
        initMinMaxVelocity4( pVData );
    }
    break;
    }

    itr++;
}

void EvalDataExtractor_101::initShaftVelocity( EvalData_101* pVData )
{
    pVData->minV = MIN_VELOCITY;
    int sp = pVData->sp;
    if( sp <= 0 )
    {
        pVData->typeName = _T( "����ľ�Ͳ����" );
        pVData->typeName = _T( "�������豸�ķ羮" );
        pVData->maxV = 15;
    }
    else if( ( sp & SEP_LIFT_COAL ) != 0 ) // ����ú̿
    {
        // �ڵ�110�����ἰ
        if( ( sp & SEP_SKIP_HOIST ) != 0 ) // ��������
        {
            pVData->typeName = _T( "����������Ͳ" );
            pVData->maxV = 6;
        }
        else
        {
            pVData->typeName = _T( "װ�д�ʽ���ͻ��ľ�Ͳ" );
            pVData->maxV = 4;
        }
    }
    else if( ( sp & SEP_HAS_GLADER_OR_REPAIR ) != 0 ) // �����еľ�Ͳ
    {
        pVData->typeName = _T( "�������Ӽ�������еľ�Ͳ" );
        pVData->maxV = 8;
    }
    else if( ( sp & SEP_NO_LIFT_USE ) != 0 ) // �������豸�ķ羮
    {
        pVData->typeName = _T( "�������豸�ķ羮" );
        pVData->maxV = 15;
    }
    else if( ( sp & SEP_LIFT_MATERIAL_OR_STUFF ) != 0 ) // ������Ա
    {
        if( ( sp & SEP_LIFT_MATERIAL ) != 0 )
        {
            if( ( sp & SEP_LIFT_STUFF ) != 0 )
            {
                pVData->typeName = _T( "רΪ�������ϵľ�Ͳ" );
                pVData->maxV = 12;
            }
            else
            {
                pVData->typeName = _T( "������Ա�����ϵľ�Ͳ" );
                pVData->maxV = 8;
            }
        }
    }
    else
    {
        pVData->typeName = _T( "����ľ�Ͳ����" );
        pVData->maxV = MAX_VELOCITY;
    }
}

void EvalDataExtractor_101::initMinMaxVelocity1( EvalData_101* pVData )
{
    if( pVData->isShaft )
    {
        initShaftVelocity( pVData );
    }
    else
    {
        switch( pVData->ft )
        {
        case FT_SIMPLE:
        {
            pVData->typeName = _T( "����ͨ���������" );
            pVData->minV = 0.15;
            pVData->maxV = MAX_VELOCITY;
        }
        break;

        case FT_MAIN:
        {
            pVData->typeName = _T( "��Ҫ�����ط���" );
            pVData->minV = MIN_VELOCITY;
            pVData->maxV = 8;
        }
        break;

        case FT_SUBMAIN:
        {
            pVData->typeName = _T( "���������ط���" );
            pVData->minV = 0.25;
            pVData->maxV = 6;
        }
        break;

        case FT_TRANSPORT:
        {
            pVData->typeName = _T( "�������" );
            pVData->minV = 0.25;
            pVData->maxV = 6;
        }
        break;

        case FT_ELECTRIC:
        {
            pVData->maxV = 8;
            if( pVData->q_ch4 < 0.001 ) // ���Ƶ���0
            {
                pVData->typeName = _T( "����˹ӿ���ļ��ߵ糵�������" );
                pVData->minV = 0.5;
            }
            else
            {
                pVData->typeName = _T( "����˹ӿ���ļ��ߵ糵�������" );
                pVData->minV = 1.0;
            }
        }
        break;
        // û�п������ݵĴ������???
        }
    }
}

void EvalDataExtractor_101::initMinMaxVelocity2( EvalData_101* pVData )
{
    pVData->minV = 0.25;
    if( pVData->cm == CMM_COMPOSITIVE ) // �۲�
    {
        pVData->typeName = _T( "�ۺϻ�е����ú������" );
        pVData->maxV = 5;
    }
    else
    {
        pVData->typeName = _T( "��ͨ��ú������" );
        pVData->maxV = 4;
    }
}

void EvalDataExtractor_101::initMinMaxVelocity3( EvalData_101* pVData )
{
    pVData->maxV = 4;
    if( pVData->cm == MT_ROCK )
    {
        pVData->typeName = _T( "����е�����" );
        pVData->minV = 0.15;
    }
    else
    {
        pVData->typeName = _T( "����е�ú��Ͱ�ú����" );
        pVData->minV = 0.25;
    }
}

void EvalDataExtractor_101::initMinMaxVelocity4( EvalData_101* pVData )
{
    pVData->typeName = _T( "����" );
    pVData->maxV = 10;
}

/**********************************************************/
/********************* EvalData_101 *******************/
/**********************************************************/
EvalData_101::EvalData_101() : minV( MIN_VELOCITY ), maxV( MAX_VELOCITY )
{

}

void EvalData_101::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( �������� )
    DEFINE_STRING_FIELD( �������� )
    DEFINE_REAL_FIELD( ���� )
    DEFINE_STRING_FIELD( ��ͷ��� )
    DEFINE_STRING_FIELD( ��߷��� )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}

void EvalData_101::writeToTableImpl( DataWriter* pDataWriter )
{
    CString emptyStr = _T( "-" );
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( ��������, name )
    WRITE_STRING_DATA( ��������, typeName )
    WRITE_REAL_DATA( ����, v )
    if( minV == MIN_VELOCITY )
    {
        WRITE_STRING_DATA( ��ͷ���, emptyStr )
    }
    else
    {
        CString value;
        value.Format( _T( "%.2f" ), minV );
        WRITE_STRING_DATA( ��ͷ���, value )
    }
    if( maxV == MAX_VELOCITY )
    {
        WRITE_STRING_DATA( ��߷���, emptyStr )
    }
    else
    {
        CString value;
        value.Format( _T( "%.2f" ), maxV );
        WRITE_STRING_DATA( ��߷���, value )
    }
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_101 **********************/
/**********************************************************/

bool Eval_101::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_101::eval_once( EvalData* pEvalData )
{
    EvalData_101* pVData = ( EvalData_101* )pEvalData;
    return eval_v( pVData->v, pVData->minV, pVData->maxV );
}

bool Eval_101::eval_v( double v, double minV, double maxV )
{
    // ???�������ıȽϿ��ܴ�������???
    return ( ( v >= minV ) && ( v <= maxV ) ); // ����ķ���
}

/**********************************************************/
/*************** EvalFactory_101 **************/
/**********************************************************/
Eval* EvalFactory_101::createEvalObject()
{
    return new Eval_101();
}

EvalData* EvalFactory_101::createEvalDataObject()
{
    return new EvalData_101();
}

EvalDataExtractor* EvalFactory_101::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_101();
}