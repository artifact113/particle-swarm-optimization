#ifndef TEST_MATH_FUNCTION_H_
#define TEST_MATH_FUNCTION_H_

#include "problem.h"


class SphereFunction : public Problem {
public:
	double GetFitness(vector<double> &solution);
};


#endif // TEST_MATH_FUNCTION_H_
