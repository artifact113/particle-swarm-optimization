#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "algorithm.h"
#include "problem.h"


class Program {
public:
	int GetGeneration();

	vector<double> GetSolution();

	double GetFitness();

private:
	Algorthm* _algorithm;
	Problem* _problem;	
};

#endif // PROGRAM_H_