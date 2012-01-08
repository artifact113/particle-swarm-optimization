#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <vector>
using namespace std;


class Problem {
public:
	virtual vector<double> GetValueRange(int dimension) = 0;

	virtual double GetFitness(vector<double> &solution) = 0;
};

#endif