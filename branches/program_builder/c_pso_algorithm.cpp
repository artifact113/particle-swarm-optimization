#include "c_pso_algorithm.h"


CPSO::CPSO(ParticleSwarm* swarm)
: _swarm(swarm) {

}


int CPSO::GetGeneration() {

}


double CPSO::GetFitness() {
	return _swarm->global_best_cost;
}


vector CPSO::GetSolution() {
	return _swarm->global_best_position;
}