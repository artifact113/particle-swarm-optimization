#include "PSO.h"
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;


/*******************************************Particle*****************************************/
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



/*******************************************ParticleSwarm************************************/
ParticleSwarm::ParticleSwarm()
{
	Momentum = 1;
	TendencyToOwnBest = 2;
	TendencyToGlobalBest = 2;
	PercentMaximumVelocityOfSearchSpace = 0.1;
	UseGlobalOptimum = true;
	BestCost = 1.0e+100;
}


ParticleSwarm::~ParticleSwarm()
{
	vector<Particle*>::iterator iter;
	for (iter = Particles.begin(); iter != Particles.end(); ++iter)
	{
		delete *iter;
	}
}


int ParticleSwarm::SwarmSize()
{
	return (int)Particles.size();
}


vector<double> ParticleSwarm::CurrentBestPosition()
{
	return Particles[0]->Position;

}


double ParticleSwarm::CurrentBestCost()
{
	return Particles[0]->Cost;

}


Particle& ParticleSwarm::operator [](int i)
{
	return *(Particles.at(i));
}


void ParticleSwarm::Iteration()
{
	//计算每一个粒子的消费量，并更新各自的历史最佳位置
	for (int i = 0; i != Particles.size(); ++i)
	{
		Particles[i]->CalculateCost();
		Particles[i]->UpdateBest();
	}

	SortParticles();

    //更新粒子群的历史全局最佳位置
	if (CurrentBestCost() < BestCost)
	{
		BestPosition = CurrentBestPosition();
		BestCost = CurrentBestCost();
	}

	//计算新的位置和速度
	for (int j = 0; j != Particles.size(); ++j)
	{
		if (UseGlobalOptimum)
		{
			Particles[j]->UpdateVelocityAndPosition(BestPosition);
		}
		else
		{
			Particles[j]->UpdateVelocityAndPosition(CurrentBestPosition());
		}
	}
}


void ParticleSwarm::SortParticles()
{
	sort(Particles.begin(),Particles.end(),CompareTo);
}


bool CompareTo(const Particle* p1, const Particle* p2)
{
	return p1->Cost < p2->Cost;
}


/*******************************************FunctionBase*************************************/