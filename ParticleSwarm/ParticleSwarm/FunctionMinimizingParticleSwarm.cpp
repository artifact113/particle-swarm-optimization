#include "MyPSO.h"
#include <vector>

using namespace std;



FunctionMinimizingParticleSwarm::FunctionMinimizingParticleSwarm(FunctionBase *function, 
																 int swarmSize, 
																 int dimension)
{
	InitSwarm(function,swarmSize, dimension);

	SortParticles();
}


void FunctionMinimizingParticleSwarm::InitSwarm(FunctionBase *function, 
												int swarmSize, 
												int dimension)
{
	vector<Particle*> particles;

	for (int i=0; i != swarmSize; ++i)
	{
		/// ��ʼ��λ�ú��ٶ�
		vector<double> position;
		vector<double> velocity;
		for (int j=0; j != dimension; ++j)
		{
			double x = _rnd.NextDouble() * 6 -3;
			double vx = _rnd.NextDouble() * 6 -3;
			position.push_back(x);
			velocity.push_back(vx);
		}

		particles.push_back(new FunctionMinimizingParticle(function,this,position,velocity));
	} 

	Particles = particles;

}