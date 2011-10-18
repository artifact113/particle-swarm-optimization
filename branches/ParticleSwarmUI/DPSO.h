﻿#ifndef _DPSO_H_
#define _DPSO_H_

#include <vector>

using namespace std;

class DiscreteParticleSwarm;
class Random;
class FunctionBase;


/// 粒子类
class DiscreteParticle
{
public:
	//-----------------------------------------------------------------------------------
	vector<int> Position;			///	Position,粒子的当前位置

	//-----------------------------------------------------------------------------------
	double Cost;					///	Cost,粒子的消费量,值越低越好

	//-----------------------------------------------------------------------------------
	vector<int> BestPosition;		///	BestPosition,粒子的历史最佳位置

	//-----------------------------------------------------------------------------------
	double BestCost;				///	BestCost,粒子的历史最佳消费量

	//-----------------------------------------------------------------------------------
	vector<int> Velocity;			///	Velocity,粒子的运动速度

	//-----------------------------------------------------------------------------------
	DiscreteParticleSwarm* Swarm;	///	Swarm,粒子所属的粒子群



	//-----------------------------------------------------------------------------------
	/// 构造函数;
	Particle();

	//-----------------------------------------------------------------------------------
	/// 析构函数;
	~Particle();

	//-----------------------------------------------------------------------------------
	///	消费函数，计算粒子的消费值;
	virtual void CalculateCost() = 0;

	//-----------------------------------------------------------------------------------
	///	更新粒子的历史最佳位置;
	void UpdateBest();

	//-----------------------------------------------------------------------------------
	/// 更新粒子的位置与速度;
	void UpdateVelocityAndPosition(vector<int> &bestPositionOfSwarm);

};



/// 粒子群类
class DiscreteParticleSwarm
{
public:
	//-----------------------------------------------------------------------------------	
	vector<DiscreteParticle*> Particles;/// Particles，粒子群所拥有的粒子

    //-----------------------------------------------------------------------------------    
    vector<vector<int>> RestirctDomain;	/// RestirctDomain,粒子在每维位置上的限制域

	//-----------------------------------------------------------------------------------	
	bool UseGlobalOptimum;				/// UseGlobalOptimum，是否采用全局优化策略，默认true
    
	//-----------------------------------------------------------------------------------	
	double Momentum;					/// Momentum，惯性系数

	//-----------------------------------------------------------------------------------	
	Random _rnd;						/// 随机数生成器1

	//-----------------------------------------------------------------------------------	
	double TendencyToOwnBest;			/// TendencyToOwnBest，认知系数；

	//-----------------------------------------------------------------------------------	
	Random _Rnd;						/// 随机数生成器2

	//-----------------------------------------------------------------------------------	
	double TendencyToGlobalBest;		/// TendencyToGlobalBest，社会系数

	//-----------------------------------------------------------------------------------	
	vector<int> BestPosition;			/// BestPosition，粒子群的全局最好粒子的位置

	//-----------------------------------------------------------------------------------	
	double BestCost;					/// BestCost，粒子群的全局最好粒子的消费量



	//-----------------------------------------------------------------------------------
	/// 构造函数；
	ParticleSwarm();

	//-----------------------------------------------------------------------------------
	/// 析构函数；
	~ParticleSwarm();

	//-----------------------------------------------------------------------------------
	/// SwarmSize，粒子群的大小；
	int SwarmSize();

	//-----------------------------------------------------------------------------------
	/// CurrentBestPosition,粒子群的当前代最好粒子的位置，第一个粒子的位置；
	vector<double> CurrentBestPosition();

	//-----------------------------------------------------------------------------------
	/// Cost，粒子群的当前代最好粒子的消费量，第一个粒子的消费量；
	double CurrentBestCost();

	//-----------------------------------------------------------------------------------
	/// 粒子群前进一步；
	virtual void Iteration();

};


bool CompareTo(const DiscreteParticle* p1, const DiscreteParticle* p2);


#endif //_DPSO_H_