#include "DPSO.h"
#include <vector>

using namespace std;


/***************************************DiscreteParticle*****************************************/
DiscreteParticle::DiscreteParticle()
{
	BestCost = 1.0E+100;
}


void DiscreteParticle::UpdateBest()
{
	if(Cost < BestCost)
	{
		BestCost = Cost;
		BestPosition = Position;	/// 将当前位置拷贝作历史最佳位置
	}
}


void DiscreteParticle::UpdateVelocityAndPosition(vector<int> &bestPositionOfSwarm)
{
	if (BestPosition.empty())
	{
		UpdateBest();
	}


	///	计算速度的一些参数
	double m = Swarm->Momentum;
	double c1 = Swarm->TendencyToOwnBest;
	double r1 = Swarm->_rnd.NextDouble();
	double c2 = Swarm->TendencyToGlobalBest;
	double r2 = Swarm->_Rnd.NextDouble();

	for (int i=0; i != Position.size(); ++i)
	{

		vector<double> oldVelocity = Velocity.at(i);


		
		 
		


	}
}

/***************************************DiscreteParticleSwarm************************************/





