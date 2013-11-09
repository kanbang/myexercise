#pragma once

#include "dlimexp.h"
#include "EvalResult.h"
#include "../DAOHelper/DaoData.h"

namespace PJ
{
	class EVAL_BASE_EXPORT_API EvalData : public DaoData
	{
	public:
		virtual ~EvalData() {}

	protected:
		virtual void createTableImpl(TableCreator* pTableCreator);
		virtual void writeToTableImpl(DataWriter* pDataWriter);

	protected:
		EvalData();

	public:
		EvalResult eval_result;   // 评价结果
	};

	// 评价数据提取器
	class EvalDataExtractor
	{
	public:
		virtual ~EvalDataExtractor() {}

		virtual bool hasNext() = 0;
		virtual void doExtract(EvalData* pEvalData) = 0;

	protected:
		EvalDataExtractor() {}
	};

	class Eval
	{
	public:
		virtual ~Eval() {}

		// 根据评价数据EvalData执行评价，并给出评价结果
		virtual bool doEval(EvalData* pEvalData) = 0;
	};

	class EvalFactory
	{
	public:
		virtual ~EvalFactory() {}

		virtual Eval* createEvalObject() = 0;

		virtual EvalData* createEvalDataObject() = 0;

		virtual EvalDataExtractor* createEvalDataExtractorObject() = 0;
	};
}