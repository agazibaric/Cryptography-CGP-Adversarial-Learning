#ifndef Skip_h
#define Skip_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class SkipT : public Function
	{
	public:
		SkipT();
		~SkipT();

		void evaluate(EvalDataP evalData);
	};

	typedef SkipT<bool> Skip;

	template <class T>
	SkipT<T>::SkipT()
	{
		name_ = "skip";
		nArguments_ = 1;
	}


	template <class T>
	SkipT<T>::~SkipT() {}

	template <class T>
	void SkipT<T>::evaluate(EvalDataP evalData)
	{
		cout << "##############################################" << endl;
		cout << "##############################################" << endl;
		cout << "##############################################" << endl;
		cout << "##############################################" << endl;
		evalData->nextIndex = 0;
	}

}

#endif /* Skip_h */

