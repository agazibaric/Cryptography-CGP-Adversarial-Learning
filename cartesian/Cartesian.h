#ifndef Cartesian_h_h
#define Cartesian_h_h

// genotype
#include "Cartesian_c.h"

// function set - put all avaliable function names and objects there
#include "FunctionSet.h"

// available functions that use key
#include "Xor.h"
#include "XorByte.h"

// available functions that dont use key
#include "Xor1.h"
#include "Xor0.h"
#include "If.h"
#include "Swap.h"
#include "Skip.h"
#include "RotL.h"
#include "RotR.h"

// crossover operators
#include "CartesianCrxOnePoint.h"
#include "CartesianCrxTwoPoint.h"
#include "CartesianCrxUniform.h"

// mutation operators
#include "CartesianMutOnePoint.h"
#include "CartesianMutNonSilent.h"
#include "CartesianMutSilent.h"
#include "CartesianMutNewParameterLess.h"


#endif /* Cartesian_h_h */