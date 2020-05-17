#include <ecf/ECF.h>
#include "AliceEvalOp.h"
#include "cartesian/Cartesian.h"
#include <cstdlib>
#include <ctime>


int main(int argc, char **argv) 
{
	srand(time(NULL));
	StateP aliceState(new State);

	string functionSetName = "functionsetkey";
	CartesianP alice (new CGP::Cartesian(functionSetName));
	
	argv[1] = "parameters.txt";
	alice->argc = argc;
	alice->argv = argv;
	alice->name = "Alice";

	aliceState->addGenotype(alice);

	aliceState->setEvalOp(new AliceEvalOp);

	aliceState->initialize(argc, argv);
	aliceState->run();

	CGP::Cartesian* bestAlice = (CGP::Cartesian*) aliceState->getHoF()->getBest()[0]->getGenotype().get();

	std::ofstream outfile;

	outfile.open("runinfo.txt", std::ios_base::app);
	
	outfile << "--------------MESSAGES:---------------" << endl;
	cout << "------------------------IFNO:-------------------------------" << endl << endl;
	cout << "--------------MESSAGES:---------------" << endl;
	for (string s : bestAlice->info.messages) {
		cout << s << endl;
		outfile << s << endl;
	}
	cout << endl;
	outfile << endl;

	cout << "--------------ENCRYPTED BY ALICE:---------------" << endl;
	for (string s : bestAlice->info.encryptedMessages) {
		cout << s << endl;
		outfile << s << endl;
	}
	cout << "Fitness: " << bestAlice->info.bestFitness << endl;
	cout << endl;
	outfile << "Fitness: " << bestAlice->info.bestFitness << endl;
	outfile << endl;

	cout << "--------------DECRYPTED BY BOB:---------------" << endl;
	for (string s : bestAlice->info.decryptedMessagesBob) {
		cout << s << endl;
		outfile << s << endl;
	}
	cout << "Fitness: " << bestAlice->info.bobFitness << endl;
	cout << endl;
	outfile << endl;
	outfile << "Fitness: " << bestAlice->info.bobFitness << endl;
	
	cout << "--------------DECRYPTED BY EVE:---------------" << endl;
	for (string s : bestAlice->info.decryptedMessagesEve) {
		cout << s << endl;
		outfile << s << endl;
	}
	cout << "Fitness: " << bestAlice->info.eveFitness << endl;
	cout << endl;
	outfile << endl;
	outfile<< "Fitness: " << bestAlice->info.eveFitness << endl;
	outfile << "-----------------------------------------------" << endl;

	cout << "Bijection: " << bestAlice->info.bijectionFitness << endl;
	cout << "KeyUtil: " << bestAlice->info.keyutilFitness << endl;
	cout << "CipherDiffer: " << bestAlice->info.cipherdifferFitness << endl;

	outfile << "Bijection: " << bestAlice->info.bijectionFitness << endl;
	outfile << "KeyUtil: " << bestAlice->info.keyutilFitness << endl;
	outfile << "CipherDiffer: " << bestAlice->info.cipherdifferFitness << endl;
	
	return 0;
}

