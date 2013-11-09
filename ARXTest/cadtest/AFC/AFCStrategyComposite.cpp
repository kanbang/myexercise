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
        // ��������Ͳ���������Զ����Ƿ��Ѵ���
        if( pAFCStrategy != 0 ) afcs.push_back( pAFCStrategy );
    }

    void AFCStrategyComposite::doCreateTable( TableCreator* pTableCreator )
    {
        // �ȴ����ӱ�
        for( AFCStrategyCollection::iterator itr = afcs.begin(); itr != afcs.end(); itr++ )
        {
            TableCreator* pCloneTableCreator = pTableCreator->clone();
            ( *itr )->createTable( pCloneTableCreator );
            delete pCloneTableCreator;
        }

        // �ٴ�������
        DaoData::doCreateTable( pTableCreator );
    }

    void AFCStrategyComposite::doWriteToTable( DataWriter* pDataWriter )
    {
        // ��д���ӱ�����
        //maxQ = 0;
        maxQ = userQ; // ���û�ָ���ķ�����Ϊ�ϴεļ������(!!!!)
        for( AFCStrategyCollection::iterator itr = afcs.begin(); itr != afcs.end(); itr++ )
        {
            AFCStrategy* pAFCStrategy = *itr;

            DataWriter* pCloneDataWriter = pDataWriter->clone();

            // �������
            pAFCStrategy->setLastQ( maxQ );
            maxQ = pAFCStrategy->getMaxQ();

            pAFCStrategy->writeToTable( pCloneDataWriter ); // �����Ѿ��������
            delete pCloneDataWriter;
        }

        // ��д�븸������
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
