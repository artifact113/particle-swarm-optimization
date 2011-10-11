//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, 武汉大学资源环境科学学院
///     All rights reserved.
///  
/// @file MyPSO.h  
/// @brief 微粒群优化算法
///  
/// 实现了Particle、ParticleSwarm、FunctionBase基类的继承类的定义,以进行空间布局优化
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
#ifndef SPATIALPSO_H
#define SPATIALPSO_H

#include <vector>
#include "PSO.h"
#include "LandUseLayer.h"

using namespace std;


class Particle;
class ParticleSwarm;
class FunctionBase;

class SpatialParticle : public Particle
{
	private:
		FunctionBase* _function;
	
	protected:

	public:
		//------------------------------------------------------
		/// 构造函数；
		SpatialParticle(FunctionBase* function,ParticleSwarm* swarm,vector<double> &position,vector<double> &velocity);
		
		//------------------------------------------------------
		/// 析构函数；
		~SpatialParticle();

		//------------------------------------------------------
		/// 消费量计算函数；
		void CalculateCost();

};


class SpatialParticleSwarm : public ParticleSwarm
{
	private:
		static Random _rnd;
	
	protected:

	public:
		//------------------------------------------------------
		/// 构造函数；
		SpatialParticleSwarm(FunctionBase *function, int swarmSize, int dimension, int range);

		//------------------------------------------------------
		/// 初始化函数；
		void InitSwarm(FunctionBase *function, int swarmSize, int dimension, int range);
};


class SpatialFunctionBase : public FunctionBase
{
	private:
		LayerAssessor* _layerAssessor;

	public:
		double Function(vector<double> position);
};

#endif