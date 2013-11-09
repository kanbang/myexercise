#pragma once

// ������ת����ַ���
#define SYMBOL_STRINGIZE(symbol) DO_SYMBOL_STRINGIZE(symbol)
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

#define FUNCTION_EXPORT_PREFIX extern "C" _declspec(dllexport)

// ���������������
#define MINEGE_PLUGIN_CREATE_FUNCTION_NAME CreateMineGEPlugin

//#define MINEGE_DRAW_CREATE_FUNCTION_MAIN_NAME Create

// �����������ʵ�ֺ�
#define MINEGE_PLUGIN_CREATE_FUNC_IMPL(MineGEPluginImpl) \
	FUNCTION_EXPORT_PREFIX MineGEPlugin* MINEGE_PLUGIN_CREATE_FUNCTION_NAME(void) \
{ \
	static MineGEPluginImpl plugin; \
	return &plugin; \
}

// Jig����������Ҫǰ׺����
#define JIG_CREATE_FUNCTION_NAME CreateJig

// Jig��������ʵ�ֺ�
#define MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(GEType, MineGEDrawImpl, MineGEDraw_Jig_Impl) \
	MineGEDraw_Jig* SYMBOL_JOIN2_WITH_DELIMITER(JIG_CREATE_FUNCTION_NAME, MineGEDraw_Jig_Impl, DELIMITER)(void) \
{ \
	static MineGEDraw_Jig_Impl jig(SYMBOL_STRINGIZE(GEType), SYMBOL_STRINGIZE(MineGEDrawImpl)); \
	return &jig; \
}

// ����Ч����ע���--��ʼ
#define BEGIN_REGISTER_MINEGE_DRAW(MineGEDrawManagerType, MineGEDrawManager_Instance) \
{ \
	MineGEDrawManagerType* pTempMineGEDrawManager = MineGEDrawManager_Instance; \

// ����Ч����ע���--ע��
// ֻ����֤�Ƿ�MineGEDraw����(��������)
// ��δ��֤MineGEDraw����Ч��(�Ƿ���MineGEƥ��)
// *****�д��޸�****!!!!
#define REGISTER_MINEGE_DRAW(GEType, MineGEDrawImpl) \
	{ \
	AcRxObject* pObj = acrxClassDictionary->at(SYMBOL_STRINGIZE(MineGEDrawImpl)); \
	if(pObj != 0) \
		{ \
		AcRxClass* pClass = AcRxClass::cast(pObj); \
		MineGEDraw* pGEDraw = MineGEDraw::cast(pClass->create()); \
		if(pGEDraw != 0) \
			{ \
			pTempMineGEDrawManager->regGEDraw(SYMBOL_STRINGIZE(GEType), pGEDraw, 0); \
			} \
		} \
	}

// ����Ч����ע���--ע�ᣨ����jig���ܣ�
#define REGISTER_MINEGE_DRAW_JIG(GEType, MineGEDrawImpl, MineGEDraw_Jig_Impl) \
	{ \
	    AcRxObject* pObj = acrxClassDictionary->at(SYMBOL_STRINGIZE(MineGEDrawImpl)); \
	    if(pObj != 0) \
	    { \
	        AcRxClass* pClass = AcRxClass::cast(pObj); \
			MineGEDraw* pGEDraw = MineGEDraw::cast(pClass->create()); \
			if(pGEDraw != 0) \
	        { \
			extern MineGEDraw_Jig* SYMBOL_JOIN2_WITH_DELIMITER(JIG_CREATE_FUNCTION_NAME, MineGEDraw_Jig_Impl, DELIMITER)(void); \
			MineGEDraw_Jig* pJig = SYMBOL_JOIN2_WITH_DELIMITER(JIG_CREATE_FUNCTION_NAME, MineGEDraw_Jig_Impl, DELIMITER)(); \
			pTempMineGEDrawManager->regGEDraw(SYMBOL_STRINGIZE(GEType), pGEDraw, pJig); \
            } \
     	} \
	}

// ����Ч����ע���--����
#define END_REGISTER_MINEGE_DRAW }

// ����Ч���෴ע���--��ʼ
#define BEGIN_UNREGISTER_MINEGE_DRAW(MineGEDrawManagerType, MineGEDrawManager_Instance) \
{ \
	MineGEDrawManagerType* pTempMineGEDrawManager = MineGEDrawManager_Instance; \

// ����Ч���෴ע���--��ע��
#define UNREGISTER_MINEGE_DRAW(GEType, MineGEDrawImpl) \
	pTempMineGEDrawManager->unRegGEDraw(SYMBOL_STRINGIZE(GEType), SYMBOL_STRINGIZE(MineGEDrawImpl));

// ����Ч���෴ע���--����
#define END_UNREGISTER_MINEGE_DRAW }