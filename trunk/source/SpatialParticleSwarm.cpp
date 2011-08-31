#include "SpatialPSO.h"
#include <vector>

using namespace std;


SpatialLayoutParticleSwarm::SpatialLayoutParticleSwarm(FunctionBase *function, 
																 int swarmSize, 
																 int dimension)
{
	InitSwarm(function,swarmSize, dimension);

	SortParticles();
}


void SpatialLayoutParticleSwarm::InitSwarm(FunctionBase *function, 
												int swarmSize, 
												int dimension)
{
	vector<Particle*> particles;

	for (int i=0; i != swarmSize; ++i)
	{
		/// 初始化位置和速度
		vector<double> position;
		vector<double> velocity;
		for (int j=0; j != dimension; ++j)
		{
			double x = _rnd.NextDouble() * 6 -3;
			double vx = _rnd.NextDouble() * 6 -3;
			position.push_back(x);
			velocity.push_back(vx);
		}

		particles.push_back(new SpatialLayoutParticle(function,this,position,velocity));
	} 

	Particles = particles;
    
    vector<double> high(dimension,1);
    vector<double> low(dimension,-1);
    PositionHigh = high;
    PositionLow = low;

}