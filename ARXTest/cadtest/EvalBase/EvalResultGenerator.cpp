#include "StdAfx.h"
#include "EvalResultGenerator.h"

#include "EvalNames.h"

namespace PJ
{
    EvalResultGenerator::EvalResultGenerator( EvalMethod* pEvalMethod )
        : m_pEvalMethod( pEvalMethod ), EvalData()
    {
    }

    EvalResultGenerator::~EvalResultGenerator( void )
    {
    }

    void EvalResultGenerator::startEval()
    {
        pEval = 0;
        pEvalData = 0;
        pEvalDataExtractor = 0;

        bInitState = false;
    }

    void EvalResultGenerator::setClauseNum( int clauseNum )
    {
        this->clauseNum = clauseNum;
    }

    EvalFactory* EvalResultGenerator::getEvalFactory()
    {
        return m_pEvalMethod->getEvalFactoryByClauseNum( clauseNum );
    }

    void EvalResultGenerator::initEvalData()
    {
        EvalFactory* pEvalFactory = getEvalFactory();
        if( pEvalFactory == 0 )
        {
            bInitState = false;
            return;
        }

        // ����Ҳ��һ��������Ҫ��Eval��صĶ�����new������
        pEvalData = pEvalFactory->createEvalDataObject();

        CString tableName;
        tableName.Format( _T( "%s_%d" ), m_pEvalMethod->getRegulationName(), clauseNum );
        pEvalData->setTableName( tableName );

        bInitState = true;
    }

    void EvalResultGenerator::initEvalObject()
    {
        EvalFactory* pEvalFactory = getEvalFactory();
        if( pEvalFactory == 0 )
        {
            bInitState = false;
            return;
        }

        // ����Ҳ��һ��������Ҫ��Eval��صĶ�����new������
        pEval = pEvalFactory->createEvalObject();
        pEvalDataExtractor = pEvalFactory->createEvalDataExtractorObject();

        bInitState = true;
    }

    void EvalResultGenerator::endEval()
    {
        // ������
        delete pEval;
        delete pEvalData;
        delete pEvalDataExtractor;
    }

    void EvalResultGenerator::doCreateTable( TableCreator* pTableCreator )
    {
        // �ȴ����ӱ�
        if( bInitState )
        {
            TableCreator* pCloneTableCreator = pTableCreator->clone();
            pEvalData->createTable( pCloneTableCreator );
            delete pCloneTableCreator;
        }

        // �ٴ�������
        EvalData::doCreateTable( pTableCreator );
    }

    // 1���������û��ʵ�֣���ô���۽��δ֪(�û���ָ�������Ч)
    // 2�����������ʵ�֣��������ڲ����ִ������۽��Ҳ��δ֪(�û�ָ�����۽����Ч)
    // 3�����������ʵ�֣������������ۣ������۽��Ϊ��(�û�ָ�����۽����Ч)
    void EvalResultGenerator::doWriteToTable( DataWriter* pDataWriter )
    {
        if( bInitState )
        {
            int count = 0; // ������(���ݼ�¼����)
            bool total_ret = true;
            DataWriter* pCloneDataWriter = pDataWriter->clone();
            while( pEvalDataExtractor->hasNext() )
            {
                pEvalDataExtractor->doExtract( pEvalData );

                bool ret = pEval->doEval( pEvalData ); // ִ�����ۣ�����boolֵ
                pEvalData->eval_result = ConvertBoolToEvalResult( ret );

                total_ret = ( total_ret && ret );

                pEvalData->writeToTable( pCloneDataWriter );

                count++;
            }
            delete pCloneDataWriter;

            if( count > 0 )
            {
                this->eval_result = ConvertBoolToEvalResult( total_ret ); // �������յ����۽���
            }
            else
            {
                this->eval_result = NoneResult; // �޸�������
            }
        }
        else
        {
            this->eval_result = UnknownResult;  // û��ʵ�֣����۽��δ֪
        }

        EvalData::doWriteToTable( pDataWriter );
    }

    void EvalResultGenerator::createTableImpl( TableCreator* pTableCreator )
    {
        EvalData::createTableImpl( pTableCreator );

        pTableCreator->addIntField( CLAUSE_NUM_FIELD );
        pTableCreator->addStringField( REF_DATA_TABLE_FIELD );
    }

    void EvalResultGenerator::writeToTableImpl( DataWriter* pDataWriter )
    {
        EvalData::writeToTableImpl( pDataWriter );

        pDataWriter->writeIntData( CLAUSE_NUM_FIELD, clauseNum );
        if( bInitState )
        {
            pDataWriter->writeStringData( REF_DATA_TABLE_FIELD, pEvalData->getTableName() );
        }
    }
}