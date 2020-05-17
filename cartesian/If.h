#ifndef If_h
#define If_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class IfT : public Function
	{
	public:
		IfT();
		~IfT();

		void evaluate(EvalDataP evalData);
	};

	typedef IfT<bool> If;

	template <class T>
	IfT<T>::IfT()
	{
		name_ = "if";
		nArguments_ = 2;
	}


	template <class T>
	IfT<T>::~IfT() {}

	template <class T>
	void IfT<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];
		int bit = ((m >> (7 - (evalData->currentBit % 8))) & 1);

		evalData->nextIndex = bit; // or nextIndex = bit == 0 ? 0 : 1;
		//evalData->currentBit--;
	}

}

#endif /* If_h */

