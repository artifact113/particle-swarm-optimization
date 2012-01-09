#ifndef C_PSO_ALGORITHM_H_
#define C_PSO_ALGORITHM_H_

#include "algorithm.h"
#include "my_c_pso.h"

class CPSO : public Algorthm {
public:
	CPSO(ParticleSwarm* swarm);

	int GetGeneration();

	double GetFitness();

	vector<double> GetSolution();

private:
	ParticleSwarm* _swarm;
};


#endif // C_PSO_ALGORITHM_H_


