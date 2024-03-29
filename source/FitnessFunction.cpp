﻿#include "SpatialPSO.h"
#include <vector>

using namespace std;


double FitnessFunction::Function(vector<double> position)
{
	double result = 0;
	vector<double>::iterator iter;
	for (iter = position.begin(); iter != position.end(); ++iter)
	{
		result += (*iter)*(*iter);
	}
	return result;
}