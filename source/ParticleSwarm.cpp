#include "PSO.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;



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


