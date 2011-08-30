#include "SpatialPSO.h"
#include <vector>

using namespace std;



SpatialLayoutParticle::SpatialLayoutParticle(FunctionBase* function, 
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


SpatialLayoutParticle::~SpatialLayoutParticle()
{
	delete _function;

}


void SpatialLayoutParticle::CalculateCost()
{
	Cost = _function->Function(Position);

}


