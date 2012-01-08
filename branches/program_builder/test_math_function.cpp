#include "test_math_function.h"


/***********************************SphereFunction***************************/
vector<double> SphereFunction::GetValueRange(int dimension) {
	vector<double> value_range;
	value_range.push_back(-10.0);
	value_range.push_back(10.0);

	return value_range;
}


double SphereFunction::GetFitness(vector<double> &solution) {
	double result = 0.0;

	vector<double>::iterator iter;
	for (iter = solution.begin(); iter != solution.end(); ++iter)
	{
		result += (*iter) * (*iter);
	}

	return result;
}