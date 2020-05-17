#ifndef EvalData_h
#define EvalData_h
#include "Cartesian_c.h"

namespace CGP
{
	class EvalData
	{
	public:
		EvalData(vector<char>& message);
		~EvalData();

		bool isOver();

		void increaseIndex();

		void setKey(vector<char> key);

		vector<char> message;
		vector<char> key;
		int currentBit;
		int nextIndex;
		bool isSwaped;
		int swapIndex;
		int limit;
		std::set<int> usedKeyBits;
		std::set<int> usedKeyBytes;
	};

	typedef boost::shared_ptr<EvalData> EvalDataP;

}

#endif 