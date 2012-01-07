#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <vector>
using namespace std;


class Problem {
public:
	virtual double GetFitness(vector<double> &solution) { return 0.0; }
};

#endif