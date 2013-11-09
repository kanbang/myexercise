#pragma once

#include "dlimexp.h"

// 表数据写入器
class DAO_HELPER_EXPORT_API DataWriter
{
public:
	DataWriter(CDaoDatabase* pDB);
	~DataWriter(void);

	// 获取数据库所在文件夹路径
	CString getDBDirPath() const;

	DataWriter* clone();

	// 开始写入数据
	void beginWriteData(const CString& tableName);

	// 结束写入数据
	long endWriteData();

	// 写入字符串数据(也包括"备注"类型的字段数据)
	void writeStringData(const CString& fieldName, const CString& value);

	// 写入整型数据
	void writeIntData(const CString& fieldName, int value);

	// 写入实数数据
	void writeRealData(const CString& fieldName, double value);

	// 写入布尔数据
	void writeBoolData(const CString& fieldName, bool value);

private:
	CDaoRecordset* m_pRecordSet;
	CDaoDatabase* m_pDB;
};