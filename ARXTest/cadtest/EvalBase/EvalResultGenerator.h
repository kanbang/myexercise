#pragma once

#include "Eval.h"
#include "EvalMethod.h"

namespace PJ
{
	// ���۽������
	class EvalResultGenerator : public EvalData
	{
	public:
		EvalResultGenerator(EvalMethod* pEvalMethod);
		~EvalResultGenerator(void);

		void startEval();
		void setClauseNum(int clauseNum);
		void initEvalData();
		void initEvalObject();
		void endEval();

	protected:
		virtual void doCreateTable(TableCreator* pTableCreator);
		virtual void doWriteToTable(DataWriter* pDataWriter);

		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	private:
		EvalFactory* getEvalFactory();

	private:
		bool bInitState; // ��ʼ��״̬(�����̺�������ʵ��Ϊtrue)
		EvalMethod* m_pEvalMethod;
		Eval* pEval;
		EvalData* pEvalData;
		EvalDataExtractor* pEvalDataExtractor;

		int clauseNum;                   // ������
	};
}