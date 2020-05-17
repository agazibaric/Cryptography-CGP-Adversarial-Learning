#ifndef AliceEvalOp_h
#define AliceEvalOp_h

class AliceEvalOp : public EvaluateOp
{
public:
	void registerParameters(StateP state);
	FitnessP evaluate(IndividualP individual);
	bool initialize(StateP);
	string random_string(size_t length);

	uint nMessages;
	uint messageLength;
	vector<string> messages;
	uint includeBob;
	uint includeEve;
	
	uint useBijection;
	uint useKeyUtil;
	uint useCipherDiffer;

	uint bijectionFit;
	uint keyUtilFit;
	uint cipherDifferFit;
};

typedef boost::shared_ptr<AliceEvalOp> AliceEvalOpP;

#endif // AliceEvalOp_h
