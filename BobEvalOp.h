#ifndef BobEvalOp_h
#define BobEvalOp_h

class BobEvalOp : public EvaluateOp
{
public:
	FitnessP evaluate(IndividualP individual);
	bool initialize(StateP);
};

typedef boost::shared_ptr<BobEvalOp> BobEvalOpP;

#endif // BobEvalOp_h