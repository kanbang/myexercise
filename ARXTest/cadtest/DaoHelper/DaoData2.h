#pragma once

#include "DaoData.h"

#include "DataReader.h"

// ��DaoData�Ļ����ϣ������˶����ݵĹ���
class DAO_HELPER_EXPORT_API DaoData2 : public DaoData
{
public:
	void readFromTable(DataReader* pDataReader);

protected:
	DaoData2();
	DaoData2(const CString& tableName);

	virtual void doReadFromTable(DataReader* pDataReader);
	virtual void readFromTableImpl(DataReader* pDataReader) = 0;
};