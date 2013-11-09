#include "stdafx.h"
#include "DaoData2.h"

DaoData2::DaoData2() : DaoData()
{

}

DaoData2::DaoData2( const CString& tableName ) : DaoData( tableName )
{

}

void DaoData2::readFromTable( DataReader* pDataReader )
{
    doReadFromTable( pDataReader );
}

void DaoData2::doReadFromTable( DataReader* pDataReader )
{
    readFromTableImpl( pDataReader );
}