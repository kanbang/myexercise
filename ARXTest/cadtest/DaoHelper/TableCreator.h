#pragma once

#include "dlimexp.h"

// DAO创建数据库表
class DAO_HELPER_EXPORT_API TableCreator
{
public:
	TableCreator(CDaoDatabase* pDB);
	~TableCreator(void);

	// 获取数据库所在文件夹路径
	CString getDBDirPath() const;

	TableCreator* clone();

	// 检测表是否存在
	bool isTableExist(const CString& tableName);

	// 创建表
	void createTable(const CString& tableName);

	// 关闭表
	void closeTable();

	// 添加一个主键字段
	void addPrimaryKeyField(const CString& primarykeyName);

	// 添加一个整形字段
	void addIntField(const CString& fieldName);

	// 添加一个实数字段
	void addRealField(const CString& fieldName);

	// 添加一个布尔字段
	void addBoolField(const CString& fieldName);

	// 添加一个字符串字段(最大255Byte的内容）
	void addStringField(const CString& fieldName);

	// 添加一个备注字段(大段的文字)
	void addCommentField(const CString& fieldName);

private:
	CDaoTableDef* m_pTableDef;
	CDaoDatabase* m_pDB;
};