//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, �人��ѧ��Դ������ѧѧԺ
///     All rights reserved.
///  
/// @file MyPSO.h  
/// @brief ΢��Ⱥ�Ż��㷨
///  
/// ʵ����Particle��ParticleSwarm��FunctionBase����ļ̳���Ķ���
///  
/// @version 1.0
/// @author jingsam
/// @date 2011-07-30
///  
///  
///		�޶�˵��������汾
///		�������Ǹ���G��nther M. FOIDL��C#�汾Particle swarm optimization
///		for function optimization��д,���CDDL-1.0Э��
///		�����http://www.codeproject.com/KB/recipes/particleswarmoptimization.aspx
//////////////////////////////////////////////////////////////////////////

#ifndef MYPSO_H
#define MYPSO_H

#include <vector>
#include "PSO.h"

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
		/// ���캯����
		FunctionMinimizingParticle(FunctionBase* function,ParticleSwarm* swarm,vector<double> position,vector<double> velocity);
		
		//------------------------------------------------------
		/// ����������
		~FunctionMinimizingParticle();

		//------------------------------------------------------
		/// ���������㺯����
		void CalculateCost();

};



class FunctionMinimizingParticleSwarm : public ParticleSwarm
{
	private:
	
	protected:

	public:
		//------------------------------------------------------
		/// ���캯����
		FunctionMinimizingParticleSwarm(FunctionBase* fuction,int swarmSize, int dimension);

		//------------------------------------------------------
		/// ��ʼ��������
		void InitSwarm(FunctionBase* fuction,int swarmSize, int dimension);
};



class SphereFunction : public FunctionBase
{
	public:
		double Function(vector<double> position);

};

#endif