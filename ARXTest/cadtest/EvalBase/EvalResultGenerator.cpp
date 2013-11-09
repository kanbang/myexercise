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

        // 这里也有一个隐含的要求，Eval相关的对象都是new出来的
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

        // 这里也有一个隐含的要求，Eval相关的对象都是new出来的
        pEval = pEvalFactory->createEvalObject();
        pEvalDataExtractor = pEvalFactory->createEvalDataExtractorObject();

        bInitState = true;
    }

    void EvalResultGenerator::endEval()
    {
        // 清理工作
        delete pEval;
        delete pEvalData;
        delete pEvalDataExtractor;
    }

    void EvalResultGenerator::doCreateTable( TableCreator* pTableCreator )
    {
        // 先创建子表
        if( bInitState )
        {
            TableCreator* pCloneTableCreator = pTableCreator->clone();
            pEvalData->createTable( pCloneTableCreator );
            delete pCloneTableCreator;
        }

        // 再创建父表
        EvalData::doCreateTable( pTableCreator );
    }

    // 1、如果条款没有实现，那么评价结果未知(用户可指定结果有效)
    // 2、如果条款有实现，但程序内部出现错误，评价结果也是未知(用户指定评价结果无效)
    // 3、如果条款有实现，且能正常评价，以评价结果为主(用户指定评价结果无效)
    void EvalResultGenerator::doWriteToTable( DataWriter* pDataWriter )
    {
        if( bInitState )
        {
            int count = 0; // 计数器(数据记录行数)
            bool total_ret = true;
            DataWriter* pCloneDataWriter = pDataWriter->clone();
            while( pEvalDataExtractor->hasNext() )
            {
                pEvalDataExtractor->doExtract( pEvalData );

                bool ret = pEval->doEval( pEvalData ); // 执行评价，返回bool值
                pEvalData->eval_result = ConvertBoolToEvalResult( ret );

                total_ret = ( total_ret && ret );

                pEvalData->writeToTable( pCloneDataWriter );

                count++;
            }
            delete pCloneDataWriter;

            if( count > 0 )
            {
                this->eval_result = ConvertBoolToEvalResult( total_ret ); // 计算最终的评价结论
            }
            else
            {
                this->eval_result = NoneResult; // 无该项评价
            }
        }
        else
        {
            this->eval_result = UnknownResult;  // 没有实现，评价结果未知
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