#pragma once

#include "dlimexp.h"

#include "TableCreator.h"
#include "DataWriter.h"
#include "HelperMacro.h"

// �߱��������д�����ݹ���
class DAO_HELPER_EXPORT_API DaoData
{
public:
	virtual ~DaoData() {}

	CString getTableName() const;
	void setTableName(const CString& tableName);
	long getCurrentRecordID() const;

	void createTable(TableCreator* pTableCreator);
	void writeToTable(DataWriter* pDataWriter);

protected:
	DaoData();
	DaoData(const CString& tableName);

	virtual void doCreateTable(TableCreator* pTableCreator);
	virtual void doWriteToTable(DataWriter* pDataWriter);

	virtual void createTableImpl(TableCreator* pTableCreator) = 0;
	virtual void writeToTableImpl(DataWriter* pDataWriter) = 0;

private:
	CString m_tableName;
	long current_id; // ��ǰ��¼��idֵ
};
