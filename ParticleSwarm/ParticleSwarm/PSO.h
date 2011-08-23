//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, �人��ѧ��Դ������ѧѧԺ
///     All rights reserved.
///  
/// @file PSO.h  
/// @brief ΢��Ⱥ�Ż��㷨
///  
/// ʵ����Particle��ParticleSwarm��FunctionBase����Ķ���
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

#ifndef PSO_H
#define PSO_H

#include <vector>
#include "Tools.h"

using namespace std;

class Random;
class ParticleSwarm;



/// ������
class Particle
{ 
	private:
		//------------------------------------------------------
		/// ȡ�����е����ֵ;
		/// 
		/// @return ˫���������е����ֵ
		/// @note ����Ϊ��ʱ������0
		double Max(vector<double> position);

		//------------------------------------------------------
		/// ȡ�����е���Сֵ;
		///
		/// @return ˫���������е���Сֵ
		/// @note ����Ϊ��ʱ������0
		double Min(vector<double> position);


	protected:
		double _bestCost;		/// ���ӵ���ʷ���������
		static Random _rnd;		/// �����������


	public:
		//------------------------------------------------------
		///	Swarm,��������������Ⱥ;
		ParticleSwarm* Swarm;

		//------------------------------------------------------
		///	Position,���ӵĵ�ǰλ�ã�����������;
		vector<double> Position;

		//------------------------------------------------------
		///	Velocity,���ӵ��˶��ٶ�;
		vector<double> Velocity;

		//------------------------------------------------------
		///	Cost,���ӵ�������,ֵԽ��Խ��;
		double Cost;

		//------------------------------------------------------
		///	BestPosition,���ӵ���ʷ���λ��;
		vector<double> BestPosition;

		//------------------------------------------------------
		///	������������������ĳһά���ϵ�λ��;
		double& operator[](int i);


		//------------------------------------------------------
		/// ���캯��;
		Particle();

		//------------------------------------------------------
		/// ��������;
		~Particle();

		//------------------------------------------------------
		///	���Ѻ������������ӵ�����ֵ;
		virtual void CalculateCost() = 0;

		//------------------------------------------------------
		///	�������ӵ���ʷ���λ��;
		void UpdateHistory();

		//------------------------------------------------------
		/// �������ӵ�λ�����ٶ�;
		///
		/// @param bestPositionOfSwarm ��������Ⱥ�����λ�ã������������֪������
		void UpdateVelocityAndPosition(vector<double> bestPositionOfSwarm);

};



/// ����Ⱥ��
class ParticleSwarm
{
	private:

	
	protected:
		static Random _rnd;		/// �����������


	public:
		//------------------------------------------------------
		/// Particles������Ⱥ��ӵ�е����ӣ�
		vector<Particle*> Particles;

		//------------------------------------------------------
		/// Momentum������ϵ����
		double Momentum;

		//------------------------------------------------------
		/// TendencyToOwnBest����֪ϵ����
		double TendencyToOwnBest;

		//------------------------------------------------------
		/// TendencyToGlobalBest�����ϵ����
		double TendencyToGlobalBest;

		//------------------------------------------------------
		/// PercentMaximumVelocityOfSearchSpace��vmax=k*xmax,kֵ��
		double PercentMaximumVelocityOfSearchSpace;

		//------------------------------------------------------
		/// UseGlobalOptimum���Ƿ����ȫ���Ż����ԣ�Ĭ��true��
		bool UseGlobalOptimum;

		//------------------------------------------------------
		/// BestPosition������Ⱥ��ȫ��������ӵ�λ�ã�
		vector<double> BestPosition;

		//------------------------------------------------------
		/// BestCost������Ⱥ��ȫ��������ӵ���������
		double BestCost;

		//------------------------------------------------------
		/// Indexer��������������ĳ�����ӣ�
		Particle& operator[](int i);



		//------------------------------------------------------
		/// ���캯����
		ParticleSwarm();

		//------------------------------------------------------
		/// ����������
		~ParticleSwarm();

		//------------------------------------------------------
		/// SwarmSize������Ⱥ�Ĵ�С��
		///
		/// @return ����Ⱥ��ӵ�е�������
		int SwarmSize();

		//------------------------------------------------------
		/// CurrentBestPosition,����Ⱥ�ĵ�ǰ��������ӵ�λ�ã���һ�����ӵ�λ�ã�
		vector<double> CurrentBestPosition();

		//------------------------------------------------------
		/// Cost������Ⱥ�ĵ�ǰ��������ӵ�����������һ�����ӵ���������
		double CurrentBestCost();

		//------------------------------------------------------
		/// ����Ⱥǰ��һ����
		virtual void Iteration();

		//------------------------------------------------------
		/// ���������������ӽ������򣬴�С����
		void SortParticles();

};

bool CompareTo(const Particle* p1, const Particle* p2);



/// ���Ѻ�����
class FunctionBase
{
	public:
		virtual double Function(vector<double> position) = 0;
};


#endif