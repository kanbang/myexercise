#pragma once

#include <map>

namespace PJ
{
	class EvalMethod;

	class EvalMethodManager
	{
	public:
		EvalMethodManager(void);
		~EvalMethodManager(void);

		void regEvalMethod(const CString& regulationName);
		EvalMethod* getEvalMethodByName(const CString& regulationName);

	private:
		typedef std::map<CString, EvalMethod*> EvalMethodCollection;
		EvalMethodCollection ems;
	};
}