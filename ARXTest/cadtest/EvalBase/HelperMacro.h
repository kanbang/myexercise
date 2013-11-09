#pragma once

// 将符号转变成字符串
#define EVAL_SYMBOL_STRINGIZE(symbol) DO_SYMBOL_STRINGIZE(symbol)
#define DO_SYMBOL_STRINGIZE(symbol) _T(#symbol)

// 将2个符号连接起来
#define SYMBOL_JOIN2(symbol1, symbol2) DO_SYMBOL_JOIN2(symbol1, symbol2)
#define DO_SYMBOL_JOIN2(symbol1, symbol2) DO_SYMBOL_JOIN2_IMPL(symbol1, symbol2)
#define DO_SYMBOL_JOIN2_IMPL(symbol1, symbol2) symbol1##symbol2

// 将2个符号用分隔符连接起来
#define SYMBOL_JOIN2_WITH_DELIMITER(symbol1, symbol2, delimiter) SYMBOL_JOIN2(SYMBOL_JOIN2(symbol1, delimiter), symbol2)

// 将3个符号用分隔符连接起来
#define SYMBOL_JOIN3_WITH_DELIMITER(symbol1, symbol2, symbol3, delimiter) SYMBOL_JOIN2_WITH_DELIMITER(SYMBOL_JOIN2_WITH_DELIMITER(symbol1, symbol2, delimiter), symbol3, delimiter)

// 定义下划线作为分隔符
#define DELIMITER _

// 导出函数前缀宏
#define FUNCTION_EXPORT_PREFIX extern "C" _declspec(dllexport)

// 评价创建函数名称
#define EVAL_PLUGIN_CREATE_FUNCTION_NAME CreateEvalPlugin

// 评价创建函数实现宏
#define EVAL_PLUGIN_CREATE_FUNCTION_IMPL(EvalPluginImpl) \
	FUNCTION_EXPORT_PREFIX PJ::EvalPlugin* EVAL_PLUGIN_CREATE_FUNCTION_NAME(void) \
	{ \
	static EvalPluginImpl plugin; \
	return &plugin; \
	}

// 评价工厂创建函数主要名称
#define EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME CreateEvalFactory

// 评价工厂创建函数实现宏
#define EVAL_FACTORY_CREATE_FUNCTION_IMPL(ClauseNum, EvalFactoryImpl) \
	PJ::EvalFactory* SYMBOL_JOIN2_WITH_DELIMITER(EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME, ClauseNum, DELIMITER)(void) \
	{ \
	static EvalFactoryImpl factory; \
	return &factory; \
	}

// 评价工厂注册宏
#define BEGIN_REGISTER_EVAL_FACTORY(EvalMethodType, EvalMethodInstance) \
	{ \
	EvalMethodType* pTempEvalMethod = EvalMethodInstance;

#define REGISTER_EVAL_FACTORY(ClauseNum) \
	extern PJ::EvalFactory* SYMBOL_JOIN2_WITH_DELIMITER(EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME, ClauseNum, DELIMITER)(void); \
	pTempEvalMethod->regEvalFactory(ClauseNum,  SYMBOL_JOIN2_WITH_DELIMITER(EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME, ClauseNum, DELIMITER)());

#define END_REGISTER_EVAL_FACTORY }

// 评价工厂反注册宏
#define BEGIN_UNREGISTER_EVAL_FACTORY(EvalMethodType, EvalMethodInstance) \
{ \
	EvalMethodType* pTempEvalMethod = EvalMethodInstance;

#define UNREGISTER_EVAL_FACTORY(ClauseNum) \
	pTempEvalMethod->unRegEvalFactory(ClauseNum);

#define END_UNREGISTER_EVAL_FACTORY }