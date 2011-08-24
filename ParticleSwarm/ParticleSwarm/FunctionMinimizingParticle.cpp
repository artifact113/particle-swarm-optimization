#include "MyPSO.h"
#include <vector>

using namespace std;



FunctionMinimizingParticle::FunctionMinimizingParticle(FunctionBase* function, 
													   ParticleSwarm* swarm, 
													   vector<double> position, 
													   vector<double> velocity)
{
	_function = function;
	Swarm = swarm;
	Position = position;
	Velocity = velocity;

	CalculateCost();

}


FunctionMinimizingParticle::~FunctionMinimizingParticle()
{
	delete _function;

}


void FunctionMinimizingParticle::CalculateCost()
{
	Cost = _function->Function(Position);

}


