#ifndef _FITNESSFUNCTION_H_
#define _FITNESSFUNCTION_H_

#include <vector>
#include "LandUseLayer.h"

using namespace std;


/// ��Ӧ�Ⱥ���
class FitnessFunction
{
public:
	LandUseLayer* Layer;		// ���۵�ͼ��
	double MaxValue;			// �����Ӧֵ
	double MinValue;			// �����Ӧֵ
	bool IsPositive;			// �Ƿ�Ϊ����������ӦֵԽ��Խ�û���ԽСԽ�ã�

	/***************************************************************************************/
	/// ���㵱ǰ��Ӧֵ
	virtual double CurrentValue(vector<int> &landUseCodes) = 0;

	/***************************************************************************************/
	/// ���㵱ǰ�÷�[0,100]
	double Score(vector<int> &landUseCodes);
}


/// ���Լ��
class AreaFunction : public FitnessFunction
{
public:
	vector<double> AvgAreas;

	AreaFunction(LandUseLayer* layer, vector<double> &avgAreas, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}



/// ����Ч��
class BenefitFunction : public FitnessFunction
{
public:
	vector<double> AvgBenefits;

	BenefitFunction(LandUseLayer* layer, vector<double> &avgBenefits, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}


/// �������
class ChangeCostFunction : public FitnessFunction
{
public:
	vector<double> AvgChangeCosts;

	ChangeCostFunction(LandUseLayer* layer, vector<double> &avgChangeCosts, bool isPositive = false);
	double CurrentValue(vector<int> &landUseCodes);
}


/// ����������
class SuitabilityFunction : public FitnessFunction
{
public:
	vector<double> AvgSuitabilities;

	SuitabilityFunction(LandUseLayer* layer, vector<double> &avgSuitabilities, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}


/// �ռ���ն�
class CompactnessFunction : public FitnessFunction
{
public:
	vector<vector<int>> AvgCompactnesses;
  
	CompactnessFunction(LandUseLayer* layer, vector<double> &avgCompactnesses, bool isPositive = false);
	double CurrentValue(vector<int> &landUseCodes);
}


/// ͼ������
class LayerAssessor
{
public:
	vector<FitnessFunction*> Functions;
	vector<double> Weights;

	LayerAssessor(vector<FitnessFunction*> &functions, vector<double> &weights);

	double TotalScore(vector<int> &landUseCodes);
}
#endif // _FITNESSFUNCTION_H_
