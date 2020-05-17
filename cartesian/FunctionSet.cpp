#include "FunctionSet.h"
#include "Cartesian.h"


namespace CGP {


FunctionSet::FunctionSet() {}


bool FunctionSet::initialize(StateP state)
{
	// initialize map with all functions

	state_ = state;

	// register existing functions
	// functions that use key
	FunctionP func = (FunctionP) (new Xor);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new XorByte);
	mAllFunctions[func->getName()] = func;

	// functions that dont use key
	func = (FunctionP)(new Xor0);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new Xor1);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new If);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new Skip);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new Swap);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new RotL);
	mAllFunctions[func->getName()] = func;

	func = (FunctionP)(new RotR);
	mAllFunctions[func->getName()] = func;

	return true;
}


bool FunctionSet::addFunction(std::string name)
{
	// mark given function as active (add to vector of used functions)

	func_iter iter = mAllFunctions.find(name);
	// if not found, return false
	if(iter == mAllFunctions.end())
		return false;

	vFunctions.push_back(iter->second);
	mFunctionSet[iter->first] = iter->second;

	return true;
}

}
