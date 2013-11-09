#pragma once

// ������ת����ַ���
#define EVAL_SYMBOL_STRINGIZE(symbol) DO_SYMBOL_STRINGIZE(symbol)
#define DO_SYMBOL_STRINGIZE(symbol) _T(#symbol)

// ��2��������������
#define SYMBOL_JOIN2(symbol1, symbol2) DO_SYMBOL_JOIN2(symbol1, symbol2)
#define DO_SYMBOL_JOIN2(symbol1, symbol2) DO_SYMBOL_JOIN2_IMPL(symbol1, symbol2)
#define DO_SYMBOL_JOIN2_IMPL(symbol1, symbol2) symbol1##symbol2

// ��2�������÷ָ�����������
#define SYMBOL_JOIN2_WITH_DELIMITER(symbol1, symbol2, delimiter) SYMBOL_JOIN2(SYMBOL_JOIN2(symbol1, delimiter), symbol2)

// ��3�������÷ָ�����������
#define SYMBOL_JOIN3_WITH_DELIMITER(symbol1, symbol2, symbol3, delimiter) SYMBOL_JOIN2_WITH_DELIMITER(SYMBOL_JOIN2_WITH_DELIMITER(symbol1, symbol2, delimiter), symbol3, delimiter)

// �����»�����Ϊ�ָ���
#define DELIMITER _

// ��������ǰ׺��
#define FUNCTION_EXPORT_PREFIX extern "C" _declspec(dllexport)

// ���۴�����������
#define EVAL_PLUGIN_CREATE_FUNCTION_NAME CreateEvalPlugin

// ���۴�������ʵ�ֺ�
#define EVAL_PLUGIN_CREATE_FUNCTION_IMPL(EvalPluginImpl) \
	FUNCTION_EXPORT_PREFIX PJ::EvalPlugin* EVAL_PLUGIN_CREATE_FUNCTION_NAME(void) \
	{ \
	static EvalPluginImpl plugin; \
	return &plugin; \
	}

// ���۹�������������Ҫ����
#define EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME CreateEvalFactory

// ���۹�����������ʵ�ֺ�
#define EVAL_FACTORY_CREATE_FUNCTION_IMPL(ClauseNum, EvalFactoryImpl) \
	PJ::EvalFactory* SYMBOL_JOIN2_WITH_DELIMITER(EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME, ClauseNum, DELIMITER)(void) \
	{ \
	static EvalFactoryImpl factory; \
	return &factory; \
	}

// ���۹���ע���
#define BEGIN_REGISTER_EVAL_FACTORY(EvalMethodType, EvalMethodInstance) \
	{ \
	EvalMethodType* pTempEvalMethod = EvalMethodInstance;

#define REGISTER_EVAL_FACTORY(ClauseNum) \
	extern PJ::EvalFactory* SYMBOL_JOIN2_WITH_DELIMITER(EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME, ClauseNum, DELIMITER)(void); \
	pTempEvalMethod->regEvalFactory(ClauseNum,  SYMBOL_JOIN2_WITH_DELIMITER(EVAL_FACTORY_CREATE_FUNCTION_MAIN_NAME, ClauseNum, DELIMITER)());

#define END_REGISTER_EVAL_FACTORY }

// ���۹�����ע���
#define BEGIN_UNREGISTER_EVAL_FACTORY(EvalMethodType, EvalMethodInstance) \
{ \
	EvalMethodType* pTempEvalMethod = EvalMethodInstance;

#define UNREGISTER_EVAL_FACTORY(ClauseNum) \
	pTempEvalMethod->unRegEvalFactory(ClauseNum);

#define END_UNREGISTER_EVAL_FACTORY }