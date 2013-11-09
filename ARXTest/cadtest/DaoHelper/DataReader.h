#pragma once

#include "dlimexp.h"

// 表数据读取器
class DAO_HELPER_EXPORT_API DataReader
{
public:
	DataReader(CDaoDatabase* pDB);
	~DataReader(void);

	// 开始读取数据
	// 读取表中的所有数据，目前暂不支持参数查询
	void beginReadData(const CString& tableName);
	
	// 结束读取数据
	void endReadData();

	// 是否还有记录可读
	bool hasNext() const;

	// 移动到下一条记录
	void moveNext();

	// 读取字符串数据(也包括"备注"类型的字段数据)
	void readStringData(const CString& fieldName, CString& value);

	// 读取整型数据
	void readIntData(const CString& fieldName, int& value);

	// 读取实数数据
	void readRealData(const CString& fieldName, double& value);

	// 读取布尔数据
	void readBoolData(const CString& fieldName, bool& value);

private:
	CDaoRecordset* m_pRecordSet;
	CDaoDatabase* m_pDB;
};
