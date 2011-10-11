#include "SpatialPSO.h"
#include <vector>
#include "PSO.h"

using namespace std;

/************************************************SpatialParticle****************************************/
SpatialParticle::SpatialParticle(FunctionBase* function, 
								 ParticleSwarm* swarm, 
								 vector<double> &position, 
								 vector<double> &velocity)
{
	_function = function;
	Swarm = swarm;
	Position = position;
	Velocity = velocity;

	CalculateCost();
}


SpatialParticle::~SpatialParticle()
{
	delete _function;
}


void SpatialParticle::CalculateCost()
{
	Cost = _function->Function(Position);
}



/************************************************SpatialParticleSwarm***********************************/
SpatialParticleSwarm::SpatialParticleSwarm(FunctionBase *function, int swarmSize, int dimension, int range)
{
	InitSwarm(function,swarmSize, dimension, range);

	SortParticles();
}


void SpatialParticleSwarm::InitSwarm(FunctionBase *function, int swarmSize, int dimension, int range)
{
	for (int i=0; i != swarmSize; ++i)
	{
		/// 初始化位置和速度
		vector<double> position;
		vector<double> velocity;
		for (int j=0; j != dimension; ++j)
		{
			double x = _rnd.NextDouble() * (range-1);
			double vx = _rnd.NextDouble() * (range-1);
			position.push_back(x);
			velocity.push_back(vx);
		}

		Particles.push_back(new SpatialParticle(function,this,position,velocity));
	} 
    
    PositionHigh.assign(dimension,range-1); 
    PositionLow.assign(dimension,0);
}



/************************************************SpatialFunctionBase************************************/