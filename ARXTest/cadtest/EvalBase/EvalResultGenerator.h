#pragma once

#include "Eval.h"
#include "EvalMethod.h"

namespace PJ
{
	// 评价结果汇总
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
		bool bInitState; // 初始化状态(如果规程和条款有实现为true)
		EvalMethod* m_pEvalMethod;
		Eval* pEval;
		EvalData* pEvalData;
		EvalDataExtractor* pEvalDataExtractor;

		int clauseNum;                   // 条款编号
	};
}