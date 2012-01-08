#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "algorithm.h"
#include "problem.h"


class Program {
public:
	int GetGeneration();

	double GetTime();	

	double GetFitness();

	int GetStallGeneration();

	vector<double> GetSolution();

private:
	Algorthm* _algorithm;
	Problem* _problem;	
};

#endif // PROGRAM_H_