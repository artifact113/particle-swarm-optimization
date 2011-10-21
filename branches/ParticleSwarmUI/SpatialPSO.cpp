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
	Random rnd;
	for (int i=0; i != swarmSize; ++i)
	{
		/// 初始化位置和速度
		vector<double> position;
		vector<double> velocity;
		for (int j=0; j != dimension; ++j)
		{
			double x = rnd.NextDouble() * (range-1);
			double vx = rnd.NextDouble() * (range-1);
			position.push_back(x);
			velocity.push_back(vx);
		}

		Particles.push_back(new SpatialParticle(function,this,position,velocity));
	} 
    
    PositionHigh.assign(dimension,range-1); 
    PositionLow.assign(dimension,0);
}



/************************************************SpatialFunctionBase************************************/
SpatialFunctionBase::SpatialFunctionBase(LayerAssessor* layerAssessor)
{	
	_layerAssessor = layerAssessor;
}


double SpatialFunctionBase::Function(vector<double> &position)
{
	vector<int> newPosition;
	vector<double>::iterator iter;
	for (iter = position.begin(); iter != position.end(); ++iter)
	{
		newPosition.push_back((int)*iter);
	}

	double score = _layerAssessor->TotalScore(newPosition);

	return 100 - score;
}