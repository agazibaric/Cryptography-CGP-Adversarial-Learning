#include "EvalData.h"

namespace CGP {

	EvalData::EvalData(vector<char>& message) 
	{
		this->message = message;
		currentBit = 0;
		swapIndex = 0;
		isSwaped = false;
		nextIndex = 0;
		limit = message.size() * 8;
	}


	EvalData::~EvalData() {}

	void EvalData::increaseIndex() {
		currentBit++;
	}



	bool EvalData::isOver() {
		return currentBit >= limit;
	}

	void EvalData::setKey(vector<char> key) {
		this->key = key;
	}

}