#include "Cartesian.h"
#include <cctype>
#include <map>
#include <vector>
#include <set>
#include "EvalData.h"


namespace CGP {


	Cartesian::Cartesian(string functionSet)
	{
		name_ = "Cartesian";
		this->functionSetName = functionSet;
	}


	Cartesian::~Cartesian(void)
	{}


	Cartesian* Cartesian::copy()
	{
		Cartesian *newObject = new Cartesian(*this);

		// no deep copy code needed

		return newObject;
	}


	vector<CrossoverOpP> Cartesian::getCrossoverOp()
	{
		vector<CrossoverOpP> crxOps;
		crxOps.push_back((CrossoverOpP)(new CartesianCrxOnePoint));
		crxOps.push_back((CrossoverOpP)(new CartesianCrxTwoPoint));
		crxOps.push_back((CrossoverOpP)(new CartesianCrxUniform));

		return crxOps;
	}


	vector<MutationOpP> Cartesian::getMutationOp()
	{
		vector<MutationOpP> mutOps;

		mutOps.push_back((MutationOpP)(new CartesianMutOnePoint));
		mutOps.push_back((MutationOpP)(new CartesianMutNonSilent));
		mutOps.push_back((MutationOpP)(new CartesianMutSilent));
		mutOps.push_back((MutationOpP)(new CartesianMutNewParameterLess));

		return mutOps;
	}


	void Cartesian::registerParameters(StateP state)
	{
		registerParameter(state, "numoutputs", (voidP)(new uint(1)), ECF::UINT, "number of functional outputs (default: 1)");
		registerParameter(state, "numrows", (voidP)(new uint(1)), ECF::UINT, "number of rows (default: 1)");
		registerParameter(state, "numcols", (voidP)(new uint(100)), ECF::UINT, "number of columns (default: 100)");
		registerParameter(state, "levelsback", (voidP)(new uint(1)), ECF::UINT, "number of previous columns to be used as possible inputs (default: 1)");
		registerParameter(state, "numvariables", (voidP)(new uint(1)), ECF::UINT, "number of input variables (default: 1)");
		registerParameter(state, "keysize", (voidP)(new uint(5)), ECF::UINT, "number of bytes in key (default: 5)");
		registerParameter(state, "functionsetkey", (voidP)(new string), ECF::STRING, "set of functions that use key");
		registerParameter(state, "functionsetnokey", (voidP)(new string), ECF::STRING, "set of functions that dont use key");
	}


	bool Cartesian::initialize(StateP state)
	{
		state_ = state;

		stringstream ss;
		string names, name;
		voidP sptr;

		// create and initialize the function set
		functionSet = static_cast<FunctionSetP> (new FunctionSet);
		functionSet->initialize(state_);

		uint number;
		//Simple parameters
		sptr = getParameterValue(state, "numvariables");
		number = *((uint*)sptr.get());
		if (number <= 0) {
			cerr << "Genotype initialization error: Number of variables is lesser than 1 or can not be parsed into a number." << endl;
			return false;
		}
		nVariables = number;

		sptr = getParameterValue(state, "numoutputs");
		number = *((uint*)sptr.get());
		if (number <= 0) {
			cerr << "Genotype initialization error: Number of outputs is lesser than 1 or can not be parsed into a number." << endl;
			return false;
		}
		nOutputs = number;

		sptr = getParameterValue(state, "numrows");
		number = *((uint*)sptr.get());
		if (number <= 0) {
			cerr << "Genotype initialization error: Number of rows is lesser than 1 or can not be parsed into a number." << endl;
			return false;
		}
		nRows = number;

		sptr = getParameterValue(state, "numcols");
		number = *((uint*)sptr.get());
		if (number <= 0) {
			cerr << "Genotype initialization error: Number of columns is lesser than 1 or can not be parsed into a number." << endl;
			return false;
		}
		nCols = number;

		sptr = getParameterValue(state, "levelsback");
		number = *((uint*)sptr.get());
		if (number <= 0) {
			cerr << "Genotype initialization error: Number of columns is lesser than 1 or can not be parsed into a number." << endl;
			return false;
		}
		nLevelsBack = number;

		sptr = getParameterValue(state, "keysize");
		number = *((uint*)sptr.get());
		if (number <= 0) {
			cerr << "Genotype initialization error: Key size must be specified." << endl;
			return false;
		}
		keySize = number;

		nConstants = 0;
		nInputs = nVariables;

		//Functionset parametri su malo složeniji
		sptr = getParameterValue(state, functionSetName);
		names = *((string*)sptr.get());
		ss.str("");
		ss.clear();
		ss << names;
		name = "";
		number = 0;

		while (ss >> name) {
			functionSet->addFunction(name);
			++number;
		}
		nFunctions = number;

		map<string, FunctionP>::iterator it;
		maxArity = 0;
		for (it = functionSet->mFunctionSet.begin(); it != functionSet->mFunctionSet.end(); it++) {
			uint nArgs = it->second->getNumberOfArguments();
			if (nArgs > maxArity)
			{
				maxArity = nArgs;
			}
		}

		buildRandomGenome();

		stringstream sValue;
		// write genome to sValue
		std::vector<uint>& genome = *this;
		for (uint i = 0; i < genome.size(); i++)
			sValue << genome[i] << " ";
		//cout << sValue.str().c_str() << " ";
		//cout << endl;

		if (isAlice)
			generateRandomKey();

		return true;
	}

	string Cartesian::random_string(size_t length)
	{
		const char alphanum[] =
			"0123456789"
			"!@#$%^&*"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		string str;
		for (unsigned int i = 0; i < length; ++i)
		{
			str += alphanum[rand() % (sizeof(alphanum) - 1)];
		}
		return str;
	}


	void Cartesian::read(XMLNode &xCart)
	{
		string s = xCart.getText(0);
		istringstream ss(s);
		string token;
		uint i = 0;
		while (getline(ss, token, ' '))
		{
			this->at(i++) = stoi(token);
		}
	}


	void Cartesian::write(XMLNode &xCart)
	{
		xCart = XMLNode::createXMLTopNode("Cartesian");
		stringstream sValue;
		sValue << this->size();
		xCart.addAttribute("size", sValue.str().c_str());

		sValue.str("");

		// write genome to sValue
		std::vector<uint>& genome = *this;
		for (uint i = 0; i < genome.size(); i++)
			sValue << genome[i] << " ";

		xCart.addText(sValue.str().c_str());
	}


	uint Cartesian::getGenomeSize()
	{
		return this->size();
	}


	void Cartesian::buildRandomGenome()
	{
		// create genome given user defined parameters
		for (int i = 0; i < nCols; i++)
		{
			for (int j = 0; j < nRows; j++)
			{
				this->push_back((uint)(state_->getRandomizer()->getRandomInteger(nFunctions)));
				for (int k = 0; k < maxArity; ++k)
				{
					uint xZ = randomInput(i);
					this->push_back(xZ);
				}
			}
		}
		for (int i = 0; i < nOutputs; ++i)
		{
			this->push_back(randomOutput());
		}
	}

	uint Cartesian::randomInput(uint column)
	{
		int min = column > nLevelsBack ? nRows * (column - nLevelsBack) + nInputs : 0;
		int max = nRows * column + nInputs;
		return (uint)(state_->getRandomizer()->getRandomInteger(min, max - 1));
	}

	uint Cartesian::randomOutput()
	{
		// input can be output
		int min = nLevelsBack > nCols ? 0 : nRows * (nCols - nLevelsBack);
		int max = nRows * nCols + nInputs;
		return (uint)(state_->getRandomizer()->getRandomInteger(min, max - 1));
	}

	void Cartesian::generateRandomKey()
	{
		string randomKey = random_string(keySize);
		key = *new vector<char>(randomKey.begin(), randomKey.end());
	}

	void Cartesian::evaluate(vector<char>& inputData, vector<char>& results)
	{
		std::vector<uint>& genome = *this;

		int currentBit = 0;
		const uint limit = inputData.size() * 8;
		const uint outputIndex = nRows * nCols * (maxArity + 1);
		uint node;
		uint functionIndex;
		uint function;
		uint nextNodeIndex = outputIndex;


		EvalDataP evalData = (EvalDataP)(new EvalData(vector<char>(inputData)));
		evalData->setKey(key);

		while (!evalData->isOver())
		{
			node = genome[nextNodeIndex];
			if (node < nInputs) {
				nextNodeIndex = outputIndex;
				evalData->increaseIndex();
				continue;
			}

			functionIndex = (node - nInputs) * (maxArity + 1);
			function = genome[functionIndex];
			evalData->nextIndex = 0;
			functionSet->vFunctions[function]->evaluate(evalData);

			nextNodeIndex = functionIndex + 1 + evalData->nextIndex;
			if (genome[nextNodeIndex] < nInputs) {
				nextNodeIndex = outputIndex;
			}
			
			evalData->increaseIndex();
		}

		for (int i = 0, n = inputData.size(); i < n; i++) {
			results[i] = evalData->message[i];
		}

		this->usedKeyBits = evalData->usedKeyBits;
		this->usedKeyBytes = evalData->usedKeyBytes;

	}

}



