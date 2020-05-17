#ifndef RotR_h
#define RotR_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class RotRT : public Function
	{
	public:
		RotRT();
		~RotRT();

		void evaluate(EvalDataP evalData);
	};

	typedef RotRT<bool> RotR;

	template <class T>
	RotRT<T>::RotRT()
	{
		name_ = "rotr";
		nArguments_ = 1;
	}


	template <class T>
	RotRT<T>::~RotRT() {}

	template <class T>
	void RotRT<T>::evaluate(EvalDataP evalData)
	{
		int byteIndex = evalData->currentBit / 8;
		char m = evalData->message[byteIndex];

		vector<int> bits = vector<int>(8);
	
		for (int i = 7; i >= 0; i--)
		{
			bits[7 - i] = ((m >> i) & 1);
		}
		
		std::rotate(bits.begin(), bits.begin() + bits.size() - 1, bits.end());

		char mRL = (char)0;

		for (int i = 0; i < 8; i++) {
			if (bits[i]) { mRL |= 1 << (7 - i); }
		}

		evalData->message[byteIndex] = mRL;
		evalData->nextIndex = 0;
	}

}

#endif /* RotR_h */

