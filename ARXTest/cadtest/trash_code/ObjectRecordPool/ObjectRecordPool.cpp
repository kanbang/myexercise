#include "StdAfx.h"
#include "ObjectRecordPool.h"

#include <deque>
#include <algorithm>
typedef std::deque<AcDbObjectId> ObjectQueue;  // 对象队列

struct ObjectRecordPool
{
    ObjectRecordPool() : useORP( false )
    {
    }

    bool getState()
    {
        return useORP;
    }

    void setState( bool flag )
    {
        this->useORP = flag;
    }

    void clearAll()
    {
        oq.clear();
    }

    bool addObject( const AcDbObjectId& objId )
    {
        if( !useORP ) return false;

        bool ret = ( oq.end() == std::find( oq.begin(), oq.end(), objId ) );
        if( ret )
        {
            oq.push_back( objId );
        }
        return ret;
    }

    bool getObject( AcDbObjectId& objId )
    {
        if( !useORP ) return false;
        if( oq.empty() ) return false;

        objId = oq.front();
        oq.pop_front();

        return true;
    }

    int getCount() const
    {
        return oq.size();
    }

    ObjectQueue oq; // 对象池
    bool useORP;    // 是否启用对象池
};

ObjectRecordPool* pObjectRecordPool = 0;

void ORPHelper::CreateORP()
{
    if( pObjectRecordPool == 0 )
    {
        pObjectRecordPool = new ObjectRecordPool();
    }
}

void ORPHelper::DeleteORP()
{
    if( pObjectRecordPool != 0 )
    {
        delete pObjectRecordPool;
        pObjectRecordPool = 0;
    }
}

void ORPHelper::SetORPState( bool flag )
{
    if( pObjectRecordPool != 0 )
    {
        pObjectRecordPool->setState( flag );
    }
}

bool ORPHelper::getORPState()
{
    if( pObjectRecordPool == 0 ) return false;
    return pObjectRecordPool->getState();
}

bool ORPHelper::AddObject( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return false;
    if( pObjectRecordPool == 0 ) return false;
    return pObjectRecordPool->addObject( objId );
}

bool ORPHelper::GetObject( AcDbObjectId& objId )
{
    if( pObjectRecordPool == 0 ) return false;

    return pObjectRecordPool->getObject( objId );
}

void ORPHelper::ClearAll()
{
    if( pObjectRecordPool != 0 )
    {
        pObjectRecordPool->clearAll();
    }
}

int ORPHelper::GetCount()
{
    if( pObjectRecordPool == 0 ) return -1;
    return pObjectRecordPool->getCount();
}