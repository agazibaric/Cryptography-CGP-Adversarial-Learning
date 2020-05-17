#include <cmath>
#include "ECF/ECF.h"
#include "AliceEvalOp.h"
#include "BobEvalOp.h"
#include "EveEvalOp.h"
#include "cartesian/Cartesian.h"


void AliceEvalOp::registerParameters(StateP state)
{
	state->getRegistry()->registerEntry("include.bob", (voidP)(new uint(1)), ECF::UINT);
	state->getRegistry()->registerEntry("include.eve", (voidP)(new uint(1)), ECF::UINT);
	state->getRegistry()->registerEntry("nmessages", (voidP)(new uint(1)), ECF::UINT);
	state->getRegistry()->registerEntry("message.length", (voidP)(new uint(1)), ECF::UINT);

	state->getRegistry()->registerEntry("use.bijection", (voidP)(new uint(0)), ECF::UINT);
	state->getRegistry()->registerEntry("use.keyutil", (voidP)(new uint(0)), ECF::UINT);
	state->getRegistry()->registerEntry("use.cipherdiffer", (voidP)(new uint(0)), ECF::UINT);

	state->getRegistry()->registerEntry("bijection.fit", (voidP)(new uint(0)), ECF::UINT);
	state->getRegistry()->registerEntry("keyutil.fit", (voidP)(new uint(0)), ECF::UINT);
	state->getRegistry()->registerEntry("cipherdiffer.fit", (voidP)(new uint(0)), ECF::UINT);
}

string AliceEvalOp::random_string(size_t length)
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


bool AliceEvalOp::initialize(StateP state)
{
	// Read parameters
	voidP sptr = state->getRegistry()->getEntry("include.bob");
	includeBob = *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("include.eve");
	includeEve = *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("nmessages");
	nMessages = *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("message.length");
	messageLength = *((uint*)sptr.get());

	sptr = state->getRegistry()->getEntry("use.bijection");
	useBijection = *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("use.keyutil");
	useKeyUtil= *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("use.cipherdiffer");
	useCipherDiffer = *((uint*)sptr.get());

	sptr = state->getRegistry()->getEntry("bijection.fit");
	bijectionFit = *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("keyutil.fit");
	keyUtilFit = *((uint*)sptr.get());
	sptr = state->getRegistry()->getEntry("cipherdiffer.fit");
	cipherDifferFit = *((uint*)sptr.get());

	
	messages = vector<string>(nMessages);
	for (int i = 0; i < nMessages; i++)
	{
		messages[i] = random_string(messageLength);
	}

	return true;
}


FitnessP AliceEvalOp::evaluate(IndividualP individual)
{

	FitnessP fitness(new FitnessMin);
	CGP::Cartesian* alice = (CGP::Cartesian*) individual->getGenotype().get();

	double fitnessValue = 0.0;
	double bobFitnessValue = 0.0;
	double eveFitnessValue = 0.0;
	const int numOfBits = nMessages * messageLength * 8;

	vector<string> encryptedMessages = vector<string>(nMessages);
	vector<string> bobDecryptedMessages = vector<string>(nMessages);
	vector<string> eveDecryptedMessages = vector<string>(nMessages);

	for (int i = 0; i < nMessages; i++)
	{
		// Get message to encrypt
		string message = messages[i];
		vector<char> inputMessage(message.begin(), message.end());
		vector<char> encryptedMessage(messageLength);

		// Send message to alice for encryption
		alice->evaluate(inputMessage, encryptedMessage);

		// Store encrypted message for Bob and Eve
		string str = *new string(encryptedMessage.begin(), encryptedMessage.end());
		encryptedMessages[i] = str;
	}

	
	//////// BOB ////////
	if (includeBob == 1)
	{
		StateP bobState(new State);
		string functionSetName = "functionsetkey";
		CartesianP bob(new CGP::Cartesian(functionSetName));
		bob->name = "Bob";
		bob->encryptedMessages = encryptedMessages;
		bob->messages = messages;
		bob->nMessages = nMessages;
		bob->messageLength = messageLength;
		bob->isAlice = FALSE;
		bob->key = alice->key;
		bob->keySize = alice->keySize;

		bobState->addGenotype(bob);
		bobState->setEvalOp(new BobEvalOp);
		char* ime = "parameters_bob.txt";
		alice->argv[1] = ime;
		bobState->initialize(alice->argc, alice->argv);
		//bobState->getRegistry()->modifyEntry("log.filename", (voidP) new string("ime.txt"));

		bobState->run();

		vector<IndividualP> best = bobState->getHoF()->getBest();
		CGP::Cartesian* bestBob = (CGP::Cartesian*) best[0]->getGenotype().get();

		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nMessages; i++)
		{
			// Get encrypted message
			string message = encryptedMessages[i];
			vector<char> encryptedMessage(message.begin(), message.end());
			vector<char> decryptedMessage(messageLength);

			// Decrypt message
			bestBob->evaluate(encryptedMessage, decryptedMessage);

			// Store decrypted message
			string str = *new string(decryptedMessage.begin(), decryptedMessage.end());
			bobDecryptedMessages[i] = str;

			string originalMessageStr = messages[i];
			vector<char> originalMessage(originalMessageStr.begin(), originalMessageStr.end());
			for (int j = 0; j < messageLength; j++)
			{
				char c1 = originalMessage[j];
				char c2 = decryptedMessage[j];
				for (int bitIndex = 0; bitIndex < 8; bitIndex++)
				{
					int bit1 = ((c1 >> bitIndex) & 1);
					int bit2 = ((c2 >> bitIndex) & 1);
					if (bit1 != bit2) {
						bobFitnessValue += 1.0;
					}
				}
			}
		}
		
		fitnessValue += bobFitnessValue;
	}
	////////////////////////////////////////////////////////////////

	if (includeEve == 1)
	{
		//////// EVE ////////
		StateP eveState(new State);
		string functionSetNameNoKey = "functionsetnokey";
		CartesianP eve(new CGP::Cartesian(functionSetNameNoKey));
		eve->name = "Eve";
		eve->encryptedMessages = encryptedMessages;
		eve->messages = messages;
		eve->nMessages = nMessages;
		eve->messageLength = messageLength;
		eve->isAlice = FALSE;

		eveState->addGenotype(eve);
		eveState->setEvalOp(new BobEvalOp);
		char* ime = "parameters_eve.txt";
		alice->argv[1] = ime;
		eveState->initialize(alice->argc, alice->argv);
		eveState->run();
		
		CGP::Cartesian* bestEve = (CGP::Cartesian*) eveState->getHoF()->getBest()[0]->getGenotype().get();

		//double bestEveFitness = eveState->getFitnessObject()->getValue();


		////////////////////////////////////////////////////////////////

		for (int i = 0; i < nMessages; i++)
		{
			// Get encrypted message
			string message = encryptedMessages[i];
			vector<char> encryptedMessage(message.begin(), message.end());
			vector<char> decryptedMessage(messageLength);

			// Decrypt message
			bestEve->evaluate(encryptedMessage, decryptedMessage);

			// Store decrypted message
			string str = *new string(decryptedMessage.begin(), decryptedMessage.end());
			eveDecryptedMessages[i] = str;

			string originalMessageStr = messages[i];
			vector<char> originalMessage(originalMessageStr.begin(), originalMessageStr.end());
			for (int j = 0; j < messageLength; j++)
			{
				char c1 = originalMessage[j];
				char c2 = decryptedMessage[j];
				for (int bitIndex = 0; bitIndex < 8; bitIndex++)
				{
					int bit1 = ((c1 >> bitIndex) & 1);
					int bit2 = ((c2 >> bitIndex) & 1);
					if (bit1 != bit2) {
						eveFitnessValue += 1.0;
					}
				}
			}
		}

		double evesPart = 2 * abs(numOfBits / 2 - eveFitnessValue); //pow(numOfBits / 2 - eveWrongBits, 2) / pow(numOfBits / 2, 2);
		fitnessValue += evesPart;
	}

	// Test bijection, key utilization and difference between plane and crypted messages

	double bijectionFitness = 0.0;
	double keyutilFitness = 0.0;
	double cipherdifferFitness = 0.0;

	if (useBijection == 1) {
		set<string> uniqueCiphertexts;
		for (auto cipher : encryptedMessages) {
			uniqueCiphertexts.insert(cipher);
		}
		int nUniqueCiphers = uniqueCiphertexts.size();
		bijectionFitness = bijectionFit * ((nMessages - nUniqueCiphers) / (float)nMessages);
		fitnessValue += bijectionFitness;
	}

	if (useKeyUtil == 1) {
		set<int> usedKeyBytes = alice->usedKeyBytes;
		float keyUtilPerc = 1.0;
		int nBytesUsed = usedKeyBytes.size();

		if (nBytesUsed != alice->keySize) {
			int nBitsUsed = nBytesUsed * 8;
			for (int bit : alice->usedKeyBits) {
				int relatedByte = bit / 8;
				// If used bit is not already used by corresponding byte, the cout it
				if (usedKeyBytes.find(relatedByte) == usedKeyBytes.end()) {
					nBitsUsed++;
				}
			}

			// change key utilization fitness percentage according to number of used bits
			float usedBitsPerc = nBitsUsed / (alice->keySize * 8);
			keyUtilPerc = usedBitsPerc;
		}

		keyutilFitness = keyUtilFit * (1 - keyUtilPerc);
		fitnessValue += keyutilFitness;
	}

	if (useCipherDiffer == 1) {
		int nDifferentBits = 0;
		for (int i = 0; i < nMessages; i++)
		{
			string message = encryptedMessages[i];
			vector<char> encryptedMessage(message.begin(), message.end());
			string originalMessageStr = messages[i];
			vector<char> originalMessage(originalMessageStr.begin(), originalMessageStr.end());

			for (int j = 0; j < messageLength; j++)
			{
				char c1 = originalMessage[j];
				char c2 = encryptedMessage[j];
				for (int bitIndex = 0; bitIndex < 8; bitIndex++)
				{
					int bit1 = ((c1 >> bitIndex) & 1);
					int bit2 = ((c2 >> bitIndex) & 1);
					if (bit1 != bit2) {
						nDifferentBits++;
					}
				}
			}
		}

		float diffBitsPerc = (float)nDifferentBits / (nMessages * messageLength * 8.0);
		if (diffBitsPerc >= 0.2) {
			diffBitsPerc = 1.0;
		}

		cipherdifferFitness = cipherDifferFit * (1.0 - diffBitsPerc);
		fitnessValue += cipherdifferFitness;
	}


	if (!alice->info.isFitnessInitialized || alice->info.bestFitness > fitnessValue)
	{
		alice->info.isFitnessInitialized = TRUE;
		alice->info.bestFitness = fitnessValue;
		alice->info.messages = messages;
		alice->info.encryptedMessages = encryptedMessages;
		alice->info.decryptedMessagesBob = bobDecryptedMessages;
		alice->info.decryptedMessagesEve = eveDecryptedMessages;
		alice->info.bobFitness = bobFitnessValue;
		alice->info.eveFitness = eveFitnessValue;

		alice->info.bijectionFitness = bijectionFitness;
		alice->info.keyutilFitness = keyutilFitness;
		alice->info.cipherdifferFitness = cipherdifferFitness;
	}

	fitness->setValue(fitnessValue);
	return fitness;
}


