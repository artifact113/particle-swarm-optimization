//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, �人��ѧ��Դ������ѧѧԺ
///     All rights reserved.
///  
/// @file MyPSO.h  
/// @brief ΢��Ⱥ�Ż��㷨
///  
/// ʵ����Particle��ParticleSwarm��FunctionBase����ļ̳���Ķ���,�Խ��пռ䲼���Ż�
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
#ifndef SPATIALPSO_H
#define SPATIALPSO_H

#include <vector>
#include "PSO.h"

using namespace std;


class Particle;
class ParticleSwarm;
class FunctionBase;

class SpatialLayoutParticle : public Particle
{
	private:
		FunctionBase* _function;
	
	protected:

	public:
		//------------------------------------------------------
		/// ���캯����
		SpatialLayoutParticle(FunctionBase* function,ParticleSwarm* swarm,vector<double> position,vector<double> velocity);
		
		//------------------------------------------------------
		/// ����������
		~SpatialLayoutParticle();

		//------------------------------------------------------
		/// ���������㺯����
		void CalculateCost();

};


class SpatialLayoutParticleSwarm : public ParticleSwarm
{
	private:
	
	protected:

	public:
		//------------------------------------------------------
		/// ���캯����
		SpatialLayoutParticleSwarm(FunctionBase* fuction,int swarmSize, int dimension);

		//------------------------------------------------------
		/// ��ʼ��������
		void InitSwarm(FunctionBase* fuction,int swarmSize, int dimension);
};


class FitnessFunction : public FunctionBase
{
	public:
		double Function(vector<double> position);

		//------------------------------------------------------
		/// ����Ч��;
		double Benefit(vector<double> position);

		//------------------------------------------------------
		/// �������;
		double ChangCost(vector<double> position);

		//------------------------------------------------------
		/// �ռ���ն�;
		double Compactness(vector<double> position);

};

#endif