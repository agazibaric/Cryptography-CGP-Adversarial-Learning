#ifndef Xor1_h
#define Xor1_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class Xor1T : public Function
	{
	public:
		Xor1T();
		~Xor1T();

		void evaluate(EvalDataP evalData);
	};

	typedef Xor1T<bool> Xor1;

	template <class T>
	Xor1T<T>::Xor1T()
	{
		name_ = "xor1";
		nArguments_ = 1;
	}


	template <class T>
	Xor1T<T>::~Xor1T() {}

	template <class T>
	void Xor1T<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];
		int bit = 1;

		m ^= bit << (7 - (evalData->currentBit % 8));
		evalData->message[byteIndex] = m;
		evalData->nextIndex = 0;
	}

}

#endif /* Xor1_h */

