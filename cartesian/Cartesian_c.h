#ifndef Cartesian_h
#define Cartesian_h
#include "ecf/ECF_base.h"
#include "ecf/Genotype.h"
#include <set>

typedef unsigned int uint;
using namespace std;


namespace CGP {

	class Info
	{
	public:
		vector<string> messages;
		vector<string> encryptedMessages;
		vector<string> decryptedMessagesBob;
		vector<string> decryptedMessagesEve;
		double bestFitness;
		double bobFitness;
		double eveFitness;
		bool isFitnessInitialized = FALSE;
		double bijectionFitness;
		double cipherdifferFitness;
		double keyutilFitness;
	};

	class FunctionSet;
	typedef boost::shared_ptr<FunctionSet> FunctionSetP;

	class Cartesian : public vector<uint>, public Genotype
	{
	public:
		Cartesian(string functionSet);
		~Cartesian(void);

		/**
		 * Initialize a genotype object (read parameters, perform sanity check, build data)
		 */
		bool initialize(StateP state);

		/**
		 * Create an identical copy of the genotype object
		 */
		Cartesian* copy();

		/**
		 * Create and return a vector of crossover operators
		 */
		vector<CrossoverOpP> getCrossoverOp();

		/**
		 * Create and return a vector of mutation operators
		 */
		vector<MutationOpP> getMutationOp();

		/**
		 * Register genotype's parameters (called before Genotype::initialize)
		 */
		void registerParameters(StateP state);

		/**
		 * Read genotype data from XMLNode
		 */
		void read(XMLNode &xCart);

		/**
		 * Write genotype data to XMLNode
		 */
		void write(XMLNode &xCart);

		uint getGenomeSize();

		/**
		 * Build random genotype choosing input connections, outputs and functions
		 */
		void buildRandomGenome();

		/**
		 * Return result for required inputs (optional: from node with index funcNum)
		 */
		void evaluate(vector<char>& inputData, vector<char>& results);

		/**
		 * Returns random input to a gate given the nonnegative column.
		 */
		uint randomInput(uint column);

		/**
		 * Returns random gate output.
		 */
		uint randomOutput();

		/**
		 * Generates ranodm key with size keySize given in parameters 
		 */
		void generateRandomKey();

		string random_string(size_t length);

		FunctionSetP functionSet;		//!< function nodes

	public:
		StateP state_;					//!< local copy of state

		// user defined parameters
		uint nVariables;				//!< number of input variables
		uint nConstants;				//!< number of input constants
		uint nOutputs;					//!< number of final outputs
		uint nRows;						//!< number of rows
		uint nCols;						//!< number of columns
		uint nLevelsBack;				//!< levels back parameter		
		uint nFunctions;				//!< number of functions

		// derived parameters
		uint nInputs;					//!< total number of inputs (including constants)
		uint maxArity;					//!< max number of inputs for all function nodes (gates)

		vector<string> messages;			 //!< messages that will be encrypted by Alice
		vector<string> encryptedMessages;	 //!< ecrypted messages produced by Alice
		vector<string> decryptedMessages;	 //!< messages that are decrypted by Bob 
		vector<string> eveDecryptedMessages; //!< messages that are decrypted by Eve
		uint nMessages = 4;				     //!< number of messages
		uint messageLength = 5;			     //!< number of chars in message string
		uint keySize = 8;					 //!< key size in bytes
		vector<char> key;					 //!< key used in encryption
		double fitness = 0.0;
		bool isAlice = TRUE;
		set<int> usedKeyBits;
		set<int> usedKeyBytes;

		// helper arguments
		int argc;
		char **argv;
		string name;
		Info info;
		string functionSetName;
		
	};

}
typedef boost::shared_ptr<CGP::Cartesian> CartesianP;


#endif // Cartesian_h