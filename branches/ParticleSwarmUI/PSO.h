//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, 武汉大学资源环境科学学院
///     All rights reserved.
///  
/// @file PSO.h  
/// @brief 微粒群优化算法
///  
/// 实现了Particle、ParticleSwarm、FunctionBase基类的定义
///  
/// @version 1.0
/// @author jingsam
/// @date 2011-07-30
///  
///  
///		修订说明：最初版本
///		本代码是根据Günther M. FOIDL的C#版本Particle swarm optimization
///		for function optimization改写,遵从CDDL-1.0协议
///		详见：http://www.codeproject.com/KB/recipes/particleswarmoptimization.aspx
//////////////////////////////////////////////////////////////////////////

#ifndef PSO_H
#define PSO_H

#include <vector>
#include "Tools.h"

using namespace std;

class Random;
class ParticleSwarm;
class FunctionBase;



/// 粒子类
class Particle
{ 
	private:


	protected:


	public:
		//------------------------------------------------------
		///	Position,粒子的当前位置，浮点型容器;
		vector<double> Position;

		//------------------------------------------------------
		///	Cost,粒子的消费量,值越低越好;
		double Cost;

		//------------------------------------------------------
		///	BestPosition,粒子的历史最佳位置;
		vector<double> BestPosition;

		//------------------------------------------------------
		///	BestCost,粒子的历史最佳消费量;
		double BestCost;

		//------------------------------------------------------
		///	Velocity,粒子的运动速度;
		vector<double> Velocity;

		//------------------------------------------------------
		///	Swarm,粒子所属的粒子群; 
		ParticleSwarm* Swarm;

		//------------------------------------------------------
		///	索引器，返回粒子在某一维度上的位置;
		double& operator[](int i);


		//------------------------------------------------------
		/// 构造函数;
		Particle();

		//------------------------------------------------------
		/// 析构函数;
		~Particle();

		//------------------------------------------------------
		///	消费函数，计算粒子的消费值;
		virtual void CalculateCost() = 0;

		//------------------------------------------------------
		///	更新粒子的历史最佳位置;
		void UpdateBest();

		//------------------------------------------------------
		/// 更新粒子的位置与速度;
		///
		/// @param bestPositionOfSwarm 所属粒子群的最佳位置，用作“社会认知”部分
		void UpdateVelocityAndPosition(vector<double> &bestPositionOfSwarm);

};



/// 粒子群类
class ParticleSwarm
{
	private:

	
	protected:


	public:
		//------------------------------------------------------
		/// Particles，粒子群所拥有的粒子；
		vector<Particle*> Particles;

        //------------------------------------------------------
        /// PositionHigh，粒子位置的上界；
        vector<double> PositionHigh;
    
        //------------------------------------------------------
        /// PositionLow，粒子位置的下界；
        vector<double> PositionLow;
    
        //------------------------------------------------------
        /// PercentMaximumVelocityOfSearchSpace，vmax=k*xmax,k值；
        double PercentMaximumVelocityOfSearchSpace;

		//------------------------------------------------------
		/// UseGlobalOptimum，是否采用全局优化策略，默认true；
		bool UseGlobalOptimum;
    
		//------------------------------------------------------
		/// Momentum，惯性系数；
		double Momentum;

		//------------------------------------------------------
		/// 随机数生成器1
		Random _rnd;

		//------------------------------------------------------
		/// TendencyToOwnBest，认知系数；
		double TendencyToOwnBest;

		//------------------------------------------------------
		/// 随机数生成器2
		Random _Rnd;

		//------------------------------------------------------
		/// TendencyToGlobalBest，社会系数；
		double TendencyToGlobalBest;

		//------------------------------------------------------
		/// BestPosition，粒子群的全局最好粒子的位置；
		vector<double> BestPosition;

		//------------------------------------------------------
		/// BestCost，粒子群的全局最好粒子的消费量；
		double BestCost;

		//------------------------------------------------------
		/// Indexer，索引器，返回某个粒子；
		Particle* operator[](int i);



		//------------------------------------------------------
		/// 构造函数；
		ParticleSwarm();

		//------------------------------------------------------
		/// 析构函数；
		~ParticleSwarm();

		//------------------------------------------------------
		/// SwarmSize，粒子群的大小；
		///
		/// @return 粒子群所拥有的粒子数
		int SwarmSize();

		//------------------------------------------------------
		/// CurrentBestPosition,粒子群的当前代最好粒子的位置，第一个粒子的位置；
		vector<double> CurrentBestPosition();

		//------------------------------------------------------
		/// Cost，粒子群的当前代最好粒子的消费量，第一个粒子的消费量；
		double CurrentBestCost();

		//------------------------------------------------------
		/// 粒子群前进一步；
		virtual void Iteration();

		//------------------------------------------------------
		/// 根据消费量对粒子进行排序，从小到大；
		void SortParticles();

};

bool CompareTo(const Particle* p1, const Particle* p2);



/// 消费函数类
class FunctionBase
{
	public:
		virtual double Function(vector<double> &position) = 0;
};


#endif