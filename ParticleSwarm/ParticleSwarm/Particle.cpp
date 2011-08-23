#include "PSO.h"
#include <vector>
#include <cmath>

using namespace std;

Particle::Particle()
{	
	_bestCost = 1.0e+100;
}


Particle::~Particle()
{
	
}


void Particle::UpdateHistory()
{
	if(Cost < _bestCost)
	{
		_bestCost = Cost;
		BestPosition = Position;	/// ����ǰλ�ÿ�������ʷ���λ��
	}

}

void Particle::UpdateVelocityAndPosition(vector<double> bestPositionOfSwarm)
{
	if (BestPosition.empty())
	{
		UpdateHistory();
	}				

	
    ///	�趨����ٶ�
	double xmax = 3;	//abs(Max(Position));
	/*if ( abs(Min(Position)) > abs(Max(Position)) )
	{
		xmax = abs(Min(Position));
	}*/
	double vmax = Swarm->PercentMaximumVelocityOfSearchSpace * xmax;


	for (int i = 0; i != Velocity.size(); i++)
	{
		///	�����ٶȵ�һЩ����
		double c1 = Swarm->TendencyToOwnBest;
		double r1 = _rnd.NextDouble();
		double c2 = Swarm->TendencyToGlobalBest;
		double r2 = _rnd.NextDouble();
		double m = Swarm->Momentum;

		/// �����µ��ٶȣ�
		double newVelocity =m * Velocity[i] +
				c1 * r1 * (BestPosition[i] - Position[i]) +
				c2 * r2 * (bestPositionOfSwarm[i] - Position[i]);

		///	�����ٶȲ�����vmax��
		if (newVelocity > vmax)
		{
			newVelocity = vmax;
		}
		if (newVelocity < -vmax)
		{
			newVelocity = -vmax;
		}

		/// �����µ��ٶȺ�λ��
		Velocity[i] = newVelocity;
		Position[i] += Velocity[i];
	}

}


double& Particle::operator [](int i)
{
	return Position.at(i);
}



double Particle::Max(vector<double> position)
{	
	vector<double>::iterator iter = position.begin();
	double max = *iter;
	for ( ; iter != position.end() ; ++iter)
	{
		if (*iter > max)
		{
			max = *iter;
		}
	}
	return max;

}


double Particle::Min(vector<double> position)
{
	vector<double>::iterator iter = position.begin();
	double min = *iter;
	for ( ; iter != position.end() ; ++iter)
	{
		if (*iter < min)
		{
			min = *iter;
		}
	}
	return min;

}


