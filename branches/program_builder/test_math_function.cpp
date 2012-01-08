#include "test_math_function.h"


/***********************************SphereFunction***************************/
double SphereFunction::GetFitness(vector<double> &solution) {
	double result = 0.0;

	vector<double>::iterator iter;
	for (iter = solution.begin(); iter != solution.end(); ++iter)
	{
		result += (*iter) * (*iter);
	}

	return result;
}