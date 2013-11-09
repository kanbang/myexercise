#include "StdAfx.h"
#include "AFCResult.h"
#include "AFCStrategyComposite.h"

namespace PJ
{
    AFCResult::AFCResult( const CString& tableName )
        : DaoData( tableName ), pAFCStrategyComposite( 0 )
        , maxQ( 0 ), userQ( 0 )
    {
    }

    void AFCResult::regAFCStrategyComposite( const CString& name_, double userQ_, AFCStrategyComposite* pAFCStrategyComposite_ )
    {
        if( pAFCStrategyComposite_ != 0 )
        {
            this->pAFCStrategyComposite = pAFCStrategyComposite_;
            this->name = name_;
            this->userQ = userQ_;
            this->pAFCStrategyComposite->setUserQ( userQ_ );
        }
    }

    void AFCResult::doCreateTable( TableCreator* pTableCreator )
    {
        // 先创建子表
        TableCreator* pCloneTableCreator = pTableCreator->clone();
        pAFCStrategyComposite->createTable( pCloneTableCreator );
        delete pCloneTableCreator;

        // 再创建父表
        DaoData::doCreateTable( pTableCreator );
    }

    void AFCResult::doWriteToTable( DataWriter* pDataWriter )
    {
        DataWriter* pCloneDataWriter = pDataWriter->clone();
        pAFCStrategyComposite->writeToTable( pCloneDataWriter );
        delete pCloneDataWriter;

        DaoData::doWriteToTable( pDataWriter );
    }

    void AFCResult::createTableImpl( TableCreator* pTableCreator )
    {
        BEGIN_DEFINE_FIELD( TableCreator, pTableCreator )
        DEFINE_INT_FIELD( afc_id )
        DEFINE_STRING_FIELD( afc_type )
        DEFINE_STRING_FIELD( name )
        DEFINE_REAL_FIELD( maxQ )
        DEFINE_REAL_FIELD( userQ )
        END_DEFINE_FIELD
    }

    void AFCResult::writeToTableImpl( DataWriter* pDataWriter )
    {
        BEGIN_WRITE_DATA( DataWriter, pDataWriter )
        WRITE_INT_DATA( afc_id, pAFCStrategyComposite->getCurrentRecordID() )
        WRITE_STRING_DATA( afc_type, pAFCStrategyComposite->getTableName() )
        WRITE_STRING_DATA( name, name )
        maxQ = pAFCStrategyComposite->getMaxQ(); // 记录计算得到的最大值
        WRITE_REAL_DATA( maxQ, maxQ )
        WRITE_REAL_DATA( userQ, userQ )
        END_WRITE_DATA
    }

    double AFCResult::getMaxQ() const
    {
        return max( maxQ, userQ );
    }
}