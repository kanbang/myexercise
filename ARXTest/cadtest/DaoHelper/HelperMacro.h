#pragma once

// 将符号转变成字符串
#define DAO_HELPER_SYMBOL_STRINGIZE(symbol) DO_SYMBOL_STRINGIZE(symbol)
#define DO_SYMBOL_STRINGIZE(symbol) _T(#symbol)

// 开始添加字段
#define BEGIN_DEFINE_FIELD(TableCreatorType, TableCreatorInstance) \
	{ \
	TableCreatorType* pTempTableCreator = TableCreatorInstance;

// 添加字段辅助宏(将变量名转换成字符串类型，并增加字段)
#define DEFINE_STRING_FIELD(varname) \
	pTempTableCreator->addStringField(DAO_HELPER_SYMBOL_STRINGIZE(varname));

#define DEFINE_COMMENT_FIELD(varname) \
	pTempTableCreator->addCommentField(DAO_HELPER_SYMBOL_STRINGIZE(varname));

#define DEFINE_INT_FIELD(varname) \
	pTempTableCreator->addIntField(DAO_HELPER_SYMBOL_STRINGIZE(varname));

#define DEFINE_REAL_FIELD(varname) \
	pTempTableCreator->addRealField(DAO_HELPER_SYMBOL_STRINGIZE(varname));

#define DEFINE_BOOL_FIELD(varname) \
	pTempTableCreator->addBoolField(DAO_HELPER_SYMBOL_STRINGIZE(varname));

#define END_DEFINE_FIELD }

// 数据写入辅助宏
#define BEGIN_WRITE_DATA(DataWriterType, DataWriterInstance) \
	{ \
	DataWriterType* pTempDataWriter = DataWriterInstance;

#define WRITE_STRING_DATA(varname, expression) \
	pTempDataWriter->writeStringData(DAO_HELPER_SYMBOL_STRINGIZE(varname), expression);

#define WRITE_SIMPLE_STRING_DATA(varname) WRITE_STRING_DATA(varname, varname)

#define WRITE_INT_DATA(varname, expression) \
	pTempDataWriter->writeIntData(DAO_HELPER_SYMBOL_STRINGIZE(varname), expression);

#define WRITE_SIMPLE_INT_DATA(varname) WRITE_INT_DATA(varname, varname)

#define WRITE_REAL_DATA(varname, expression) \
	pTempDataWriter->writeRealData(DAO_HELPER_SYMBOL_STRINGIZE(varname), expression);

#define WRITE_SIMPLE_REAL_DATA(varname) WRITE_REAL_DATA(varname, varname)

#define WRITE_BOOL_DATA(varname, expression) \
	pTempDataWriter->writeBoolData(DAO_HELPER_SYMBOL_STRINGIZE(varname), expression);

#define WRITE_SIMPLE_BOOL_DATA(varname) WRITE_BOOL_DATA(varname, varname)

#define END_WRITE_DATA }


// 数据读取辅助宏
#define BEGIN_READ_DATA(DataReaderType, DataReaderInstance) \
	{ \
	DataReaderType* pTempDataReader = DataReaderInstance;

#define READ_STRING_DATA(varname, var) \
	pTempDataReader->readStringData(DAO_HELPER_SYMBOL_STRINGIZE(varname), var);

#define READ_SIMPLE_STRING_DATA(varname) READ_STRING_DATA(varname, varname)

#define READ_INT_DATA(varname, var) \
	pTempDataReader->readIntData(DAO_HELPER_SYMBOL_STRINGIZE(varname), var);

#define READ_SIMPLE_INT_DATA(varname) READ_INT_DATA(varname, varname)

#define READ_REAL_DATA(varname, var) \
	pTempDataReader->readRealData(DAO_HELPER_SYMBOL_STRINGIZE(varname), var);

#define READ_SIMPLE_REAL_DATA(varname) READ_REAL_DATA(varname, varname)

#define READ_BOOL_DATA(varname, var) \
	pTempDataReader->readBoolData(DAO_HELPER_SYMBOL_STRINGIZE(varname), var);

#define READ_SIMPLE_BOOL_DATA(varname) READ_BOOL_DATA(varname, varname)

#define END_READ_DATA }