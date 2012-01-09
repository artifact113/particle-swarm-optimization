#include "my_c_pso.h"
using namespace std;

/******************************************************Particle****************************************/
/// ���캯��
Particle::Particle(CParticleSwarm* swarm, vector<double> &position, vector<double> &velocity)
{
	this->swarm = swarm;
	this->position = position;
	this->velocity = velocity;
}


/// ��������
Particle::~Particle()
{
	
}

void Particle::SetFunctionBase(CFunctionBase* functionbase) {
	_functionbase = functionbase;
	this->CalculateCost();
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
ParticleSwarm::ParticleSwarm(int swarm_size, int dimension)
{
	InitSwarm(functionbase,swarm_size, dimension);	
}


/// ��ʼ������
void ParticleSwarm::InitSwarm(int swarm_size, int dimension)
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

		Particle* my_particle = new Particle(this,position,velocity);
		this->particles.push_back(my_particle);
	}
}


void ParticleSwarm::SetFunctionBase(CFunctionBase* functionbase) {
	vector<Particle*>::iterator iter;
	for (iter = particles.begin(); iter != particles.end(); ++iter)
	{
		(*iter)->SetFunctionBase(functionbase);
	}

	this->SortParticles();
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




