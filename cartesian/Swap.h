#ifndef Swap_h
#define Swap_h
#include "Function.h"

namespace CGP
{
	template <class T>
	class SwapT : public Function
	{
	public:
		SwapT();
		~SwapT();

		void evaluate(EvalDataP evalData);
	};

	typedef SwapT<bool> Swap;

	template <class T>
	SwapT<T>::SwapT()
	{
		name_ = "swap";
		nArguments_ = 1;
	}


	template <class T>
	SwapT<T>::~SwapT() {}

	template <class T>
	void SwapT<T>::evaluate(EvalDataP evalData)
	{
		if (evalData->isSwaped) 
		{
			int firstIndex = evalData->swapIndex;
			int secondIndex = evalData->currentBit;

			int firstByteIndex = firstIndex / 8;
			int secondByteIndex = secondIndex / 8;

			char c1 = evalData->message[firstByteIndex];
			int firstBit = ((c1 >> (7 - (firstIndex % 8))) & 1);

			char c2 = evalData->message[secondByteIndex];
			int secondBit = ((c2 >> (7 - (secondIndex % 8))) & 1);

			int position = (7 - (firstIndex % 8));
			int mask = 1 << position;
			c1 = (c1 & ~mask) | ((secondBit << position) & mask);
			evalData->message[firstByteIndex] = c1;

			c2 = evalData->message[secondByteIndex];
			position = (7 - (secondIndex % 8));
			mask = 1 << position;
			c2 = (c2 & ~mask) | ((firstBit << position) & mask);
			evalData->message[secondByteIndex] = c2;

			evalData->isSwaped = FALSE;
		}
		else 
		{
			evalData->swapIndex = evalData->currentBit;
			evalData->isSwaped = TRUE;
		}
		evalData->nextIndex = 0;
	}

}

#endif /* Swap_h */

