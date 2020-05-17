#include <cmath>
#include "ECF/ECF.h"
#include "EveEvalOp.h"
#include "cartesian/Cartesian.h"

bool EveEvalOp::initialize(StateP)
{
	return true;
}


FitnessP EveEvalOp::evaluate(IndividualP individual)
{
	FitnessP fitness(new FitnessMin);
	CGP::Cartesian* bob = (CGP::Cartesian*) individual->getGenotype().get();

	double fitnessValue = 0.0;

	bob->eveDecryptedMessages = vector<string>(bob->nMessages);
	for (int i = 0; i < bob->nMessages; i++)
	{
		// Get encrypted message
		string message = bob->encryptedMessages[i];
		vector<char> encryptedMessage(message.begin(), message.end());
		vector<char> decryptedMessage(bob->messageLength);

		// Decrypt message
		bob->evaluate(encryptedMessage, decryptedMessage);

		// Store decrypted message
		string str = *new string(encryptedMessage.begin(), encryptedMessage.end());
		bob->eveDecryptedMessages[i] = str;

		string originalMessageStr = bob->messages[i];
		vector<char> originalMessage(originalMessageStr.begin(), originalMessageStr.end());
		for (int j = 0; j < bob->messageLength; j++)
		{
			char c1 = originalMessage[j];
			char c2 = decryptedMessage[j];
			for (int bitIndex = 0; bitIndex < 8; bitIndex++)
			{
				int bit1 = ((c1 >> bitIndex) & 1);
				int bit2 = ((c2 >> bitIndex) & 1);
				if (bit1 != bit2) {
					fitnessValue += 1.0;
				}
			}
		}
	}

	cout << "Messages:" << endl;
	for (string s : bob->messages) {
		cout << s << endl;
	}

	cout << "Decrypted Messages by eve:" << endl;
	for (string s : bob->eveDecryptedMessages) {
		cout << s << endl;
	}

	bob->fitness = fitnessValue;
	fitness->setValue(fitnessValue);
	return fitness;
}
