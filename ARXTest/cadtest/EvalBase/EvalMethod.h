#pragma once

#include <map>
#include "dlimexp.h"

namespace PJ
{
	class EvalFactory;

	class EVAL_BASE_EXPORT_API EvalMethod
	{
	public:
		EvalMethod(const CString& regulation);
		~EvalMethod(void);

		CString getRegulationName() const;
		void regEvalFactory(int clauseNum, EvalFactory* pEvalFactory);
		void unRegEvalFactory(int clauseNum);
		EvalFactory* getEvalFactoryByClauseNum(int clauseNum);

	private:
		CString m_regulation;
		typedef std::map<int, EvalFactory*> EvalFactoryCollection;
		EvalFactoryCollection factories;
	};
}