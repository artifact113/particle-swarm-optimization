#ifndef MY_C_PSO_H_
#define MY_C_PSO_H_

#include "c_pso.h"
using namespace std;


class ParticleSwarm;


class Particle : public CParticle {
public:
	/// 构造函数；
	Particle(CFunctionBase* functionbase, CParticleSwarm* swarm, vector<double> &position, vector<double> &velocity);
		
	/// 析构函数；
	~Particle();

	// 获取值域;
	vector<double> GetValueRange(int dimension);

	/// 消费量计算函数；
	void CalculateCost();

private:
	CFunctionBase* _functionbase;	
};


class ParticleSwarm : public CParticleSwarm {
public:
	/// 构造函数；
	ParticleSwarm(CFunctionBase *function, int swarmSize, int dimension);

private:
	//------------------------------------------------------
	/// 初始化函数；
	void InitSwarm(CFunctionBase *function, int swarmSize, int dimension);
};


class FunctionBase : public CFunctionBase {
public:
	vector<double> GetValueRange(int dimension);

	double GetFitness(vector<double> &position);


private:

};

#endif // MY_C_PSO_H_
