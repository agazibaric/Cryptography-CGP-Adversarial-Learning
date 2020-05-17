#ifndef Xor_h
#define Xor_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class XorT : public Function
	{
	public:
		XorT();
		~XorT();

		void evaluate(EvalDataP evalData);
	};

	typedef XorT<bool> Xor;

	template <class T>
	XorT<T>::XorT()
	{
		name_ = "xor";
		nArguments_ = 1;
	}


	template <class T>
	XorT<T>::~XorT() {}

	template <class T>
	void XorT<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];
		int size = evalData->key.size();
		char k = evalData->key[byteIndex % size];
		int bit = ((k >> (7 - (evalData->currentBit % 8))) & 1);

		m ^= bit << (7 - (evalData->currentBit % 8));
		evalData->message[byteIndex] = m;
		evalData->nextIndex = 0;

		evalData->usedKeyBits.insert((byteIndex % size) * 8 + (evalData->currentBit % 8));
	}

}

#endif /* Xor_h */

