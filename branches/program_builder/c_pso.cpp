#include "c_pso.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "tool.h"
using namespace std;


/*******************************************CParticle*************************************************/
/// 构造函数
CParticle::CParticle()
{	
	local_best_cost = MAX_DOUBLE;
}


/// 析构函数
CParticle::~CParticle()
{
	
}


///	更新粒子的历史最佳位置
void CParticle::UpdateBest()
{
	if(cost < local_best_cost)
	{		
		local_best_position = position;	// 将当前位置拷贝作历史最佳位置
		local_best_cost = cost;
	}
}


/// 更新粒子的位置与速度
void CParticle::UpdateVelocityAndPosition(vector<double> &global_best_position)
{
	if (local_best_position.empty())
	{
		this->UpdateBest();
	}

	//	计算速度的一些参数
	double m = swarm->momentum;
	double c1 = swarm->tendency_own_best;
	double r1 = swarm->_rnd.NextDouble();
	double c2 = swarm->tendency_global_best;
	double r2 = swarm->_Rnd.NextDouble();

	int dimension = this->position.size();
	for (int i = 0; i != dimension; ++i)
	{
		// 值域上界
		double position_high = this->GetValueRange(i).at(1);

		// 值域下界
		double position_low = this->GetValueRange(i).at(0);

		// 获取最大速度
		double xmax = position_high - position_low;        
        double vmax = swarm->search_percentage * xmax;        

		// 计算新的速度
		double new_velocity = m * velocity.at(i) +
				c1 * r1 * (local_best_position.at(i) - position.at(i)) +
				c2 * r2 * (global_best_position.at(i) - position.at(i));

		//限制速度不超过vmax
		if (new_velocity > vmax)
		{
			new_velocity = vmax;
		}
		if (new_velocity < -vmax)
		{
			new_velocity = -vmax;
		}

		// 计算新的速度
		velocity.at(i) = new_velocity;
        
       	// 计算新的位置
        double new_position = position.at(i) + velocity.at(i);
        
        // 限制位置不超过上下界
        if (new_position > position_high)
        {
            new_position = position_high;
        }
        if (new_position < position_low)
        {
            new_position = position_low;
        }

		// 新位置
		position.at(i) = new_position;
	}
}


/*******************************************CParticleSwarm************************************/
/// 构造函数
CParticleSwarm::CParticleSwarm()
{
	search_percentage = 0.1;
	use_global_optimum = true;
	momentum = 1;
	tendency_own_best = 2;
	tendency_global_best = 2;
	global_best_cost = MAX_DOUBLE;
}


/// 析构函数
CParticleSwarm::~CParticleSwarm()
{
	vector<CParticle*>::iterator iter;
	for (iter = particles.begin(); iter != particles.end(); ++iter)
	{
		delete *iter;
	}
}


/// 粒子群的当前代最好粒子的位置，第一个粒子的位置；
vector<double> CParticleSwarm::CurrentBestPosition()
{
	return particles.at(0)->position;
}


/// Cost，粒子群的当前代最好粒子的消费量，第一个粒子的消费量；
double CParticleSwarm::CurrentBestCost()
{
	return particles.at(0)->cost;
}


/// 粒子群整体迭代一次
void CParticleSwarm::Iteration()
{
	//计算每一个粒子的消费量，并更新各自的历史最佳位置	
	int count = particles.size();

	for (int i = 0; i != count; ++i)
	{
		this->particles.at(i)->CalculateCost();
		this->particles.at(i)->UpdateBest();
	}

	// 排序
	SortParticles();

    //更新粒子群的历史全局最佳位置
	if (CurrentBestCost() < global_best_cost)
	{
		global_best_position = CurrentBestPosition();
		global_best_cost = CurrentBestCost();
	}

	//计算新的位置和速度
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


/// 根据消费量对粒子进行排序，从小到大
void CParticleSwarm::SortParticles()
{
	sort(particles.begin(),particles.end(),CompareTo);
}



bool CompareTo(const CParticle* p1, const CParticle* p2)
{
	return (p1->cost) < (p2->cost);
}


/************************************ContinusFunctionBase*************************************/
