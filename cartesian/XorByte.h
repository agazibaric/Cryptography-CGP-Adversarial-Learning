#ifndef Xorbyte_h
#define Xorbyte_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class XorByteT : public Function
	{
	public:
		XorByteT();
		~XorByteT();

		void evaluate(EvalDataP evalData);
	};

	typedef XorByteT<bool> XorByte;

	template <class T>
	XorByteT<T>::XorByteT()
	{
		name_ = "xorbyte";
		nArguments_ = 1;
	}


	template <class T>
	XorByteT<T>::~XorByteT() {}

	template <class T>
	void XorByteT<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];
		int keyByteIndex = byteIndex % evalData->key.size();
		char k = evalData->key[keyByteIndex];
		m ^= k;

		evalData->message[byteIndex] = m;
		evalData->nextIndex = 0;

		evalData->usedKeyBytes.insert(keyByteIndex);
	}

}

#endif /* XorBytebyte_h */