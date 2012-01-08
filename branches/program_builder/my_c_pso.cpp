#include "my_c_pso.h"
using namespace std;

/******************************************************Particle****************************************/
/// ���캯��
Particle::Particle(CFunctionBase* functionbase, 
				   CParticleSwarm* swarm, 
				   vector<double> &position, 
				   vector<double> &velocity)
: _functionbase(functionbase)
{
	this->swarm = swarm;
	this->position = position;
	this->velocity = velocity;
	this->CalculateCost();
}


/// ��������
Particle::~Particle()
{
	
}


// ��ȡֵ��
vector<double> Particle::GetValueRange(int dimension)
{
	return _functionbase->GetValueRange(dimension);
}


/// ���������㺯��
void Particle::CalculateCost()
{
	this->cost = _functionbase->GetFitness(this->position);
}



/***********************************************************ParticleSwarm***********************************/
/// ���캯��
ParticleSwarm::ParticleSwarm(CFunctionBase* functionbase, int swarm_size, int dimension)
{
	InitSwarm(functionbase,swarm_size, dimension);

	SortParticles();
}


/// ��ʼ������
void ParticleSwarm::InitSwarm(CFunctionBase* functionbase, int swarm_size, int dimension)
{
	Random rnd;
	for (int i=0; i != swarm_size; ++i)
	{
		/// ��ʼ��λ�ú��ٶ�
		vector<double> position;
		vector<double> velocity;
		for (int j=0; j != dimension; ++j)
		{
			vector<double> value_range(functionbase->GetValueRange(j));
			double range = value_range.at(1) -  value_range.at(0);
			double x = rnd.NextDouble() * range + value_range.at(0);
			double vx = rnd.NextDouble() * range;
			position.push_back(x);
			velocity.push_back(vx);
		}

		Particle* my_particle = new Particle(functionbase,this,position,velocity);
		this->particles.push_back(my_particle);
	}
}



/*********************************************************FunctionBase************************************/
FunctionBase::FunctionBase(Problem* problem) 
: _problem(problem){

}

vector<double> FunctionBase::GetValueRange(int dimension)
{
	return _problem->GetValueRange(dimension);
}

double FunctionBase::GetFitness(vector<double> &position)
{
	return _problem->GetFitness(position);
}




