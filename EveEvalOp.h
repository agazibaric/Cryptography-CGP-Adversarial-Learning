#ifndef EveEvalOp_h
#define EveEvalOp_h

class EveEvalOp : public EvaluateOp
{
public:
	FitnessP evaluate(IndividualP individual);
	bool initialize(StateP);
};

typedef boost::shared_ptr<EveEvalOp> EveEvalOpP;

#endif // EveEvalOp_h