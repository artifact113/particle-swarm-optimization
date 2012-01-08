#include "c_pso.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "tool.h"
using namespace std;


/*******************************************CParticle*************************************************/
/// ���캯��
CParticle::CParticle()
{	
	local_best_cost = MAX_DOUBLE;
}


/// ��������
CParticle::~CParticle()
{
	
}


///	�������ӵ���ʷ���λ��
void CParticle::UpdateBest()
{
	if(cost < local_best_cost)
	{		
		local_best_position = position;	// ����ǰλ�ÿ�������ʷ���λ��
		local_best_cost = cost;
	}
}


/// �������ӵ�λ�����ٶ�
void CParticle::UpdateVelocityAndPosition(vector<double> &global_best_position)
{
	if (local_best_position.empty())
	{
		this->UpdateBest();
	}

	//	�����ٶȵ�һЩ����
	double m = swarm->momentum;
	double c1 = swarm->tendency_own_best;
	double r1 = swarm->_rnd.NextDouble();
	double c2 = swarm->tendency_global_best;
	double r2 = swarm->_Rnd.NextDouble();

	int dimension = this->position.size();
	for (int i = 0; i != dimension; ++i)
	{
		// ֵ���Ͻ�
		double position_high = this->GetValueRange(i).at(1);

		// ֵ���½�
		double position_low = this->GetValueRange(i).at(0);

		// ��ȡ����ٶ�
		double xmax = position_high - position_low;        
        double vmax = swarm->search_percentage * xmax;        

		// �����µ��ٶ�
		double new_velocity = m * velocity.at(i) +
				c1 * r1 * (local_best_position.at(i) - position.at(i)) +
				c2 * r2 * (global_best_position.at(i) - position.at(i));

		//�����ٶȲ�����vmax
		if (new_velocity > vmax)
		{
			new_velocity = vmax;
		}
		if (new_velocity < -vmax)
		{
			new_velocity = -vmax;
		}

		// �����µ��ٶ�
		velocity.at(i) = new_velocity;
        
       	// �����µ�λ��
        double new_position = position.at(i) + velocity.at(i);
        
        // ����λ�ò��������½�
        if (new_position > position_high)
        {
            new_position = position_high;
        }
        if (new_position < position_low)
        {
            new_position = position_low;
        }

		// ��λ��
		position.at(i) = new_position;
	}
}


/*******************************************CParticleSwarm************************************/
/// ���캯��
CParticleSwarm::CParticleSwarm()
{
	search_percentage = 0.1;
	use_global_optimum = true;
	momentum = 1;
	tendency_own_best = 2;
	tendency_global_best = 2;
	global_best_cost = MAX_DOUBLE;
}


/// ��������
CParticleSwarm::~CParticleSwarm()
{
	vector<CParticle*>::iterator iter;
	for (iter = particles.begin(); iter != particles.end(); ++iter)
	{
		delete *iter;
	}
}


/// ����Ⱥ�ĵ�ǰ��������ӵ�λ�ã���һ�����ӵ�λ�ã�
vector<double> CParticleSwarm::CurrentBestPosition()
{
	return particles.at(0)->position;
}


/// Cost������Ⱥ�ĵ�ǰ��������ӵ�����������һ�����ӵ���������
double CParticleSwarm::CurrentBestCost()
{
	return particles.at(0)->cost;
}


/// ����Ⱥ�������һ��
void CParticleSwarm::Iteration()
{
	//����ÿһ�����ӵ��������������¸��Ե���ʷ���λ��	
	int count = particles.size();

	for (int i = 0; i != count; ++i)
	{
		this->particles.at(i)->CalculateCost();
		this->particles.at(i)->UpdateBest();
	}

	// ����
	SortParticles();

    //��������Ⱥ����ʷȫ�����λ��
	if (CurrentBestCost() < global_best_cost)
	{
		global_best_position = CurrentBestPosition();
		global_best_cost = CurrentBestCost();
	}

	//�����µ�λ�ú��ٶ�
	for (int j = 0; j != count; ++j)
	{
		if (use_global_optimum)
		{
			this->particles.at(j)->UpdateVelocityAndPosition(global_best_position);
		}
		else
		{
			this->particles.at(j)->UpdateVelocityAndPosition(this->CurrentBestPosition());
		}
	}
} 


/// ���������������ӽ������򣬴�С����
void CParticleSwarm::SortParticles()
{
	sort(particles.begin(),particles.end(),CompareTo);
}



bool CompareTo(const CParticle* p1, const CParticle* p2)
{
	return (p1->cost) < (p2->cost);
}


/************************************ContinusFunctionBase*************************************/
