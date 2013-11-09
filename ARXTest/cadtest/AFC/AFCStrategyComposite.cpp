#include "StdAfx.h"
#include "AFCStrategyComposite.h"
#include "AFCStrategy.h"

namespace PJ
{
    AFCStrategyComposite::AFCStrategyComposite( const CString& tableName )
        : DaoData( tableName ), maxQ( 0 ), userQ( 0 )
    {
    }

    void AFCStrategyComposite::regAFCStrategy( AFCStrategy* pAFCStrategy )
    {
        // 简单起见，就不检查计算策略对象是否已存在
        if( pAFCStrategy != 0 ) afcs.push_back( pAFCStrategy );
    }

    void AFCStrategyComposite::doCreateTable( TableCreator* pTableCreator )
    {
        // 先创建子表
        for( AFCStrategyCollection::iterator itr = afcs.begin(); itr != afcs.end(); itr++ )
        {
            TableCreator* pCloneTableCreator = pTableCreator->clone();
            ( *itr )->createTable( pCloneTableCreator );
            delete pCloneTableCreator;
        }

        // 再创建父表
        DaoData::doCreateTable( pTableCreator );
    }

    void AFCStrategyComposite::doWriteToTable( DataWriter* pDataWriter )
    {
        // 先写入子表数据
        //maxQ = 0;
        maxQ = userQ; // 以用户指定的风量作为上次的计算风量(!!!!)
        for( AFCStrategyCollection::iterator itr = afcs.begin(); itr != afcs.end(); itr++ )
        {
            AFCStrategy* pAFCStrategy = *itr;

            DataWriter* pCloneDataWriter = pDataWriter->clone();

            // 计算风量
            pAFCStrategy->setLastQ( maxQ );
            maxQ = pAFCStrategy->getMaxQ();

            pAFCStrategy->writeToTable( pCloneDataWriter ); // 数据已经计算过了
            delete pCloneDataWriter;
        }

        // 再写入父表数据
        DaoData::doWriteToTable( pDataWriter );
    }

    void AFCStrategyComposite::createTableImpl( TableCreator* pTableCreator )
    {
        //pTableCreator->addRealField("maxQ");

        for( AFCStrategyCollection::iterator itr = afcs.begin(); itr != afcs.end(); itr++ )
        {
            pTableCreator->addIntField( ( *itr )->getTableName() + _T( "_id" ) );
        }
    }

    void AFCStrategyComposite::writeToTableImpl( DataWriter* pDataWriter )
    {
        //pDataWriter->writeRealData("maxQ", maxQ);

        for( AFCStrategyCollection::iterator itr = afcs.begin(); itr != afcs.end(); itr++ )
        {
            AFCStrategy* pAFCStrategy = *itr;
            pDataWriter->writeIntData( pAFCStrategy->getTableName() + _T( "_id" ), pAFCStrategy->getCurrentRecordID() );
        }
    }

    void AFCStrategyComposite::clearAllAFCStrategies()
    {
        afcs.clear();
    }

    double AFCStrategyComposite::getMaxQ() const
    {
        return maxQ;
    }

    void AFCStrategyComposite::setUserQ( double q )
    {
        this->userQ = q;
    }
}
