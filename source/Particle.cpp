#include "PSO.h"
#include <vector>

using namespace std;

Particle::Particle()
{	
	BestCost = 1.0e+100;
}


Particle::~Particle()
{
	
}


void Particle::UpdateBest()
{
	if(Cost < BestCost)
	{
		BestCost = Cost;
		BestPosition = Position;	/// 将当前位置拷贝作历史最佳位置
	}

}

void Particle::UpdateVelocityAndPosition(vector<double> bestPositionOfSwarm)
{
	if (BestPosition.empty())
	{
		UpdateBest();
	}


	///	计算速度的一些参数
	double c1 = Swarm->TendencyToOwnBest;
	double r1 = _rnd.NextDouble();
	double c2 = Swarm->TendencyToGlobalBest;
	double r2 = _rnd.NextDouble();
	double m = Swarm->Momentum;

	for (int i = 0; i != Velocity.size(); i++)
	{
        ///	设定最大速度
        double xmax = Swarm->PositionHigh[i] - Swarm->PositionLow[i];
        
        double vmax = Swarm->PercentMaximumVelocityOfSearchSpace * xmax;
        

		/// 计算新的速度；
		double newVelocity = m * Velocity[i] +
				c1 * r1 * (BestPosition[i] - Position[i]) +
				c2 * r2 * (bestPositionOfSwarm[i] - Position[i]);

		///	限制速度不超过vmax；
		if (newVelocity > vmax)
		{
			newVelocity = vmax;
		}
		if (newVelocity < -vmax)
		{
			newVelocity = -vmax;
		}

		/// 计算新的速度
		Velocity[i] = newVelocity;
        
        
        /// 计算新的位置
        double newPosition = Position[i] + Velocity[i];
        
        /// 限制位置不超过上下界
        if (newPosition > Swarm->PositionHigh[i])
        {
            newPosition = Swarm->PositionHigh[i];
        }
        if (newPosition < Swarm->PositionLow[i])
        {
            newPosition = Swarm->PositionLow[i];
        }
        
		Position[i] = newPosition;
	}

}


double& Particle::operator [](int i)
{
	return Position.at(i);
}





