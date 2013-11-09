#include "StdAfx.h"
#include "Eval_102.h"

#include "../MineGE/DrawHelper.h"

/* ȫ�ֺ���(ʵ����Tool.cpp��) */
extern int FindShafts( AcDbObjectIdArray& shafts );

#define MIN_TEMP 0
#define MAX_TEMP 50

/**********************************************************/
/************ EvalDataExtractor_102 ***********/
/**********************************************************/
EvalDataExtractor_102::EvalDataExtractor_102()
{
    acutPrintf( _T( "\n102--�¶�����..." ) );
    init();
    acutPrintf( _T( "\n�ҵ����������ݵĸ���:%d" ), ds.size() );
    itr = ds.begin();
}

EvalDataExtractor_102::~EvalDataExtractor_102()
{
    clean();
}

void EvalDataExtractor_102::init()
{
    // ��ʱ�ų���ͨ���
    // ����������е���ͨ������¶ȣ����ܵ���word���������ٶȷǳ���
    // ͨ������£���ͨ������¶ȴ���������Ҫ��
    //init1();
    init2();
    init3();
    //init4();
}

// ���ҽ��羮
static void FindInShafts( AcDbObjectIdArray& inShafts )
{
    // ����ȫ����Ͳ
    AcDbObjectIdArray shafts;
    int pos = FindShafts( shafts );

    for( int i = 0; i < pos; i++ )
    {
        inShafts.append( shafts[i] );
    }
}

void EvalDataExtractor_102::init1()
{
    acutPrintf( _T( "\n102 -- �������зǽ��羮���" ) );

    // �������еĽ��羮
    AcDbObjectIdArray inShafts;
    FindInShafts( inShafts );

    // 1�������������
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pStm = new TempDataLink();
        pStm->setDataSource( objIds[i] );
        if( !pStm->updateData( false ) )
        {
            // ����,���˳�
            delete pStm;
            clean();
            break;
        }
        if( inShafts.contains( objIds[i] ) ) continue; // �ų����羮

        pStm->type = TEMP_COMMON; // ��ͨ���
        //acutPrintf(_T("\n%name:%s\t%.4f"), pStm->name, pStm->t);
        ds.push_back( pStm );
    }
    acutPrintf( _T( "\n102 -- �������" ) );
}

void EvalDataExtractor_102::init2()
{
    acutPrintf( _T( "\n102 -- �������вɾ�����" ) );
    // 1���������вɾ�����
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "WorkSurface" ), objIds );
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pTm = new TempDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // ����,���˳�
            delete pTm;
            clean();
            break;
        }
        pTm->type = TEMP_TWS; // �ɾ���
        //acutPrintf(_T("\n%name:%s\t%.4f"), pTm->name, pTm->t);
        ds.push_back( pTm );
    }
    acutPrintf( _T( "\n102 -- �������" ) );
}

void EvalDataExtractor_102::init3()
{
    acutPrintf( _T( "\n102 -- �������л����豸����ͼԪ" ) );

    // 1���������л�������ͼԪ
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "MachineRoom" ), objIds );
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pTm = new TempDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // ����,���˳�
            delete pTm;
            clean();
            break;
        }
        pTm->type = TEMP_MR; // ��������
        acutPrintf( _T( "\n%name:%s\t%.4f" ), pTm->name, pTm->t );
        ds.push_back( pTm );
    }
    acutPrintf( _T( "\n102 -- �������" ) );
}

void EvalDataExtractor_102::init4()
{
    acutPrintf( _T( "\n102 -- ��������ͼԪ(��ҩ�⡢�����)" ) );

    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "PowderStorage" ), objIds ); // ��ҩ��
    DrawHelper::FindMineGEs( _T( "ChargeRoom" ), objIds ); // �����
    if( objIds.isEmpty() ) return;

    // 2������
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        TempDataLink* pTm = new TempDataLink();
        pTm->setDataSource( objIds[i] );
        if( !pTm->updateData( false ) )
        {
            // ����,���˳�
            delete pTm;
            clean();
            break;
        }
        pTm->type = TEMP_MR2;
        acutPrintf( _T( "\n%name:%s\t%.4f" ), pTm->name, pTm->t );
        ds.push_back( pTm );
    }
    acutPrintf( _T( "\n102 -- �������" ) );
}

void EvalDataExtractor_102::clean()
{
    for( DataSet::iterator itr = ds.begin(); itr != ds.end(); itr++ )
    {
        delete ( *itr );
    }
    ds.clear();
}

bool EvalDataExtractor_102::hasNext()
{
    return itr != ds.end();
}

void EvalDataExtractor_102::doExtract( EvalData* pEvalData )
{
    TempDataLink* pVM = *itr;
    EvalData_102* pVData = ( EvalData_102* )pEvalData;

    pVData->name = pVM->name;
    pVData->t = pVM->t;
    pVData->minT = MIN_TEMP;
    pVData->maxT = MAX_TEMP;
    pVData->type = pVM->type;

    switch( pVM->type )
    {
    case TEMP_COMMON:
    {
        initMinMaxTemperature1( pVData );
    }
    break;
    case TEMP_TWS:
    {
        initMinMaxTemperature2( pVData );
    }
    break;
    case TEMP_MR:
    {
        initMinMaxTemperature3( pVData );
    }
    break;

    case TEMP_MR2:
        initMinMaxTemperature4( pVData );
        break;
    }
    itr++;
}

void EvalDataExtractor_102::initMinMaxTemperature1( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = MAX_TEMP; // �¶�����(50)
}

void EvalDataExtractor_102::initMinMaxTemperature2( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = 26;
}

void EvalDataExtractor_102::initMinMaxTemperature3( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = 30;
}

void EvalDataExtractor_102::initMinMaxTemperature4( EvalData_102* pVData )
{
    pVData->minT = 2;
    pVData->maxT = 26;
}

/**********************************************************/
/********************* EvalData_102 *******************/
/**********************************************************/
EvalData_102::EvalData_102() : minT( MIN_TEMP ), maxT( MAX_TEMP )
{

}

void EvalData_102::createTableImpl( TableCreator* pTableCreator )
{
    BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
    DEFINE_STRING_FIELD( ������ڵ����� )
    DEFINE_REAL_FIELD( �����¶� )
    DEFINE_STRING_FIELD( ����¶� )
    DEFINE_STRING_FIELD( ����¶� )
    DEFINE_STRING_FIELD( ���ƴ�ʩ )
    END_DEFINE_FIELD

    EvalData::createTableImpl( pTableCreator );
}
void EvalData_102::writeToTableImpl( DataWriter* pDataWriter )
{
    CString emptyStr = _T( "" );
    BEGIN_WRITE_DATA( DataWriter, pDataWriter )
    WRITE_STRING_DATA( ������ڵ�����, name )
    WRITE_REAL_DATA( �����¶�, t )
    if( minT == MIN_TEMP )
    {
        WRITE_STRING_DATA( ����¶�, emptyStr );
    }
    else
    {
        CString value;
        value.Format( _T( "%d" ), ( int )minT );
        WRITE_STRING_DATA( ����¶�, value )
    }
    if( maxT == MAX_TEMP )
    {
        WRITE_STRING_DATA( ����¶�, emptyStr );
    }
    else
    {
        CString value;
        /*if(type == TEMP_TWS)
        {
        	value.Format(_T("�ɾ�����--%d"),(int)maxT);
        }
        else if(type == TEMP_MR)
        {
        	value.Format(_T("��������--%d"),(int)maxT);
        }
        else
        {*/
        value.Format( _T( "%d" ), ( int )maxT );
        //}
        WRITE_STRING_DATA( ����¶�, value )
    }
    WRITE_STRING_DATA( ���ƴ�ʩ, strCM )
    END_WRITE_DATA

    EvalData::writeToTableImpl( pDataWriter );
}

/**********************************************************/
/********************** Eval_102 **********************/
/**********************************************************/

bool Eval_102::doEval( EvalData* pEvalData )
{
    return eval_once( pEvalData );
}

bool Eval_102::eval_once( EvalData* pEvalData )
{
    EvalData_102* pVData = ( EvalData_102* )pEvalData;
    double t = pVData->t;
    double minT = pVData->minT;
    double maxT = pVData->maxT;
    int cm = pVData->cm;
    CString& strCM = pVData->strCM;
    strCM = _T( "��" );

    bool ret = false;
    switch( pVData->type )
    {
    case TEMP_COMMON:
    {
        ret = ( t >= minT );
        strCM = _T( "" );
    }
    break;

    case TEMP_TWS:
    {
        if( t < minT )
        {
            ret = false;
        }
        else if( t <= 26 )
        {
            ret = true;
        }
        else if( t < 30 )
        {
            ret = false;
            if( cm == 1 )
            {
                ret = true;
                strCM = _T( "���̹���ʱ�䣬������±�������" );
            }
            else if( cm == 2 )
            {
                ret = true;
                strCM = _T( "ֹͣ��ҵ" );
            }
        }
        else
        {
            ret = false;
            if( cm == 2 )
            {
                ret = true;
                strCM = _T( "ֹͣ��ҵ" );
            }
        }
    }
    break;

    case TEMP_MR:
    {
        if( t < minT )
        {
            ret = false;
        }
        else if( t <= 30 )
        {
            ret = true;
        }
        else if( t < 34 )
        {
            ret = false;
            if( cm == 1 )
            {
                ret = true;
                strCM = _T( "���̹���ʱ�䣬������±�������" );
            }
            else if( cm == 2 )
            {
                ret = true;
                strCM = _T( "ֹͣ��ҵ" );
            }
        }
        else
        {
            ret = false;
            if( cm == 2 )
            {
                ret = true;
                strCM = _T( "ֹͣ��ҵ" );
            }
        }
    }
    break;

    case TEMP_MR2:
    {
        if( t < minT )
        {
            ret = false;
        }
        else if( t <= maxT )
        {
            ret = true;
        }
        else
        {
            ret = false;
            if( cm == 1 )
            {
                ret = true;
                strCM = _T( "���̹���ʱ�䣬������±�������" );
            }
            else if( cm == 2 )
            {
                ret = true;
                strCM = _T( "ֹͣ��ҵ" );
            }
        }
    }
    break;
    }
    return ret;
}

/**********************************************************/
/*************** EvalFactory_102 **************/
/**********************************************************/
Eval* EvalFactory_102::createEvalObject()
{
    return new Eval_102();
}

EvalData* EvalFactory_102::createEvalDataObject()
{
    return new EvalData_102();
}

EvalDataExtractor* EvalFactory_102::createEvalDataExtractorObject()
{
    return new EvalDataExtractor_102();
}