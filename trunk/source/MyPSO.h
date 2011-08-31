//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, 武汉大学资源环境科学学院
///     All rights reserved.
///  
/// @file MyPSO.h  
/// @brief 微粒群优化算法
///  
/// 实现了Particle、ParticleSwarm、FunctionBase基类的继承类的定义
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

#ifndef MYPSO_H
#define MYPSO_H

#include <vector>
#include "PSO.h"

using namespace std;


class Particle;
class ParticleSwarm;
class FunctionBase;

class FunctionMinimizingParticle : public Particle
{
	private:
		FunctionBase* _function;
	
	protected:

	public:
		//------------------------------------------------------
		/// 构造函数；
		FunctionMinimizingParticle(FunctionBase* function,ParticleSwarm* swarm,vector<double> position,vector<double> velocity);
		
		//------------------------------------------------------
		/// 析构函数；
		~FunctionMinimizingParticle();

		//------------------------------------------------------
		/// 消费量计算函数；
		void CalculateCost();

};



class FunctionMinimizingParticleSwarm : public ParticleSwarm
{
	private:
	
	protected:

	public:
		//------------------------------------------------------
		/// 构造函数；
		FunctionMinimizingParticleSwarm(FunctionBase* fuction,int swarmSize, int dimension);

		//------------------------------------------------------
		/// 初始化函数；
		void InitSwarm(FunctionBase* fuction,int swarmSize, int dimension);
};



class SphereFunction : public FunctionBase
{
	public:
		double Function(vector<double> position);

};

#endif