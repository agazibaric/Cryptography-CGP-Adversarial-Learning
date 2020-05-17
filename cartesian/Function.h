#ifndef Function_h
#define Function_h
#include "Cartesian_c.h"
#include "EvalData.h"

namespace CGP
{
	class Function
	{
	public:
		Function() {}
		virtual ~Function() {}

		/**
		Evaluate result with given inputs in one of implemented derived classes.
		*/
		virtual void evaluate(EvalDataP evalData) = 0;


		std::string getName()
		{
			return name_;
		}

		uint getNumberOfArguments()
		{
			return nArguments_;
		}

	protected:
		std::string name_;
		uint nArguments_;
	};

	typedef boost::shared_ptr<Function> FunctionP;
}


#endif /* Function_h */