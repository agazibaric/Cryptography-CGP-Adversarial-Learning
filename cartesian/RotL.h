#ifndef RotL_h
#define RotL_h
#include "Function.h"
#include <vector>

namespace CGP
{
	template <class T>
	class RotLT : public Function
	{
	public:
		RotLT();
		~RotLT();

		void evaluate(EvalDataP evalData);
	};

	typedef RotLT<bool> RotL;

	template <class T>
	RotLT<T>::RotLT()
	{
		name_ = "rotl";
		nArguments_ = 1;
	}


	template <class T>
	RotLT<T>::~RotLT() {}

	template <class T>
	void RotLT<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];

		vector<int> bits = vector<int>(8);
;
		for (int i = 7; i >= 0; i--)
		{
			//cout << ((m >> i) & 1);
			bits[7 - i] = ((m >> i) & 1);
		}

		std::rotate(bits.begin(), bits.begin() + 1, bits.end());

		char mRL = (char) 0;

		for (int i = 0; i < 8; i++) {
			if (bits[i]) { mRL |= 1 << (7 - i); }
		}
		
	/*	for (int i = 7; i >= 0; i--)
		{
			cout << ((mRL >> i) & 1);
		}*/
		
		evalData->message[byteIndex] = mRL;
		evalData->nextIndex = 0;
	}

}

#endif /* RotL_h */

