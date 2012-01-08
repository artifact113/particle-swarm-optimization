#ifndef C_PSO_H_
#define C_PSO_H_

#include <vector>
#include "tool.h"
using namespace std;

class Random;
class CParticleSwarm;

/// 粒子类
class CParticle {
public:
	vector<double> position;			// 粒子的当前位置;

	double cost;						// 粒子的消费量,值越低越好;

	vector<double> local_best_position;	// 粒子的历史最佳位置;

	double local_best_cost;				// 粒子的历史最佳消费量;

	vector<double> velocity;			// 粒子的运动速度;

	CParticleSwarm* swarm;				// 粒子所属的种群


	/// 构造函数;
	CParticle();

	/// 析构函数;
	virtual ~CParticle();

	/// 获取粒子在每一维度上的值域；
	virtual vector<double> GetValueRange(int dimension) = 0;

	///	消费函数，计算粒子的消费值;
	virtual void CalculateCost() = 0;

	///	更新粒子的历史最佳位置;
	void UpdateBest();

	/// 更新粒子的位置与速度;
	void UpdateVelocityAndPosition(vector<double> &globebestposition);
};



/// 粒子群类
class CParticleSwarm
{
public:
	vector<CParticle*> particles;		// 粒子群所拥有的粒子；
    
    double search_percentage; 			// 搜索粒度，vmax=k*xmax,k值；

	bool use_global_optimum;			// 是否采用全局优化策略，默认true；

	double momentum;					// 惯性系数；

	Random _rnd;						// 随机数生成器1;

	double tendency_own_best;			/// 认知系数；

	Random _Rnd;						// 随机数生成器2

	double tendency_global_best;		// 社会系数；

	vector<double> global_best_position;// 粒子群的全局最好粒子的位置；

	double global_best_cost;			// 粒子群的全局最好粒子的消费量


	/// 构造函数；
	CParticleSwarm();

	/// 析构函数；
	virtual ~CParticleSwarm();

	/// 粒子群的当前代最好粒子的位置，第一个粒子的位置；
	vector<double> CurrentBestPosition();

	/// 粒子群的当前代最好粒子的消费量，第一个粒子的消费量；
	double CurrentBestCost();

	/// 粒子群整体迭代一次(可重定义)；
	virtual void Iteration();

	/// 根据消费量对粒子进行排序，从小到大；
	void SortParticles();
};


bool CompareTo(const CParticle* p1, const CParticle* p2);


class CFunctionBase {
public:
	virtual vector<double> GetValueRange(int dimension) = 0;

	virtual double GetFitness(vector<double> &solution) = 0;
};

#endif // C_PSO_H_
