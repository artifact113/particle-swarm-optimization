#ifndef ALGORITHM_H_
#define ALGORITHM_H_

class Algorthm {
public:
	virtual int GetGeneration() = 0;

	virtual double GetFitness() = 0;

	virtual vector<double> GetSolution() = 0;
};


#endif // ALGORITHM_H_