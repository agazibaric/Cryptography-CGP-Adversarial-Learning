#ifndef Xor0_h
#define Xor0_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class Xor0T : public Function
	{
	public:
		Xor0T();
		~Xor0T();

		void evaluate(EvalDataP evalData);
	};

	typedef Xor0T<bool> Xor0;

	template <class T>
	Xor0T<T>::Xor0T()
	{
		name_ = "xor0";
		nArguments_ = 1;
	}


	template <class T>
	Xor0T<T>::~Xor0T() {}

	template <class T>
	void Xor0T<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];
		int bit = 0;

		m ^= bit << (7 - (evalData->currentBit % 8));
		evalData->message[byteIndex] = m;
		evalData->nextIndex = 0;
	}

}

#endif /* Xor0_h */

