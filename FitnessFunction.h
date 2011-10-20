#ifndef _FITNESSFUNCTION_H_
#define _FITNESSFUNCTION_H_

#include <vector>

using namespace std;


class FitnessFunction
{
public:
	LandUseLayer* Layer;		// 评价的图层
	double MaxValue;			// 最大适应值
	double MinValue;			// 最大适应值
	bool IsPositive;			// 是否为正向函数（适应值越大越好还是越小越好）

	/***************************************************************************************/
	/// 计算当前适应值
	virtual double CurrentValue(vector<int> &landUseCodes) = 0;

	/***************************************************************************************/
	/// 计算当前得分[0,100]
	double Score(vector<int> &landUseCodes);
}


/// 面积约束
class AreaFunction : FitnessFunction
{
public:
	vector<double> AvgAreas;

	AreaFunction(LandUseLayer* layer, vector<double> &avgAreas, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}



/// 经济效益
class BenefitFunction : FitnessFunction
{
public:
	vector<double> AvgBenefits;

	BenefitFunction(LandUseLayer* layer, vector<double> &avgBenefits, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}


/// 变更费用
class ChangeCostFunction : FitnessFunction
{
public:
	vector<double> AvgChangeCosts;

	ChangeCostFunction(LandUseLayer* layer, vector<double> &avgChangeCosts, bool isPositive = false);
	double Score(vector<int> &landUseCodes);
}


/// 适宜性评价
class SuitabilityFunction : FitnessFunction
{
public:
	vector<double> AvgSuitabilities;

	SuitabilityFunction(LandUseLayer* layer, vector<double> &avgSuitabilities, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}


/// 空间紧凑度
class CompactnessFunction : FitnessFunction
{
public:
	vector<vector<int>> AvgCompactnesses;
  
	CompactnessFunction(LandUseLayer* layer, vector<double> &avgCompactnesses, bool isPositive = true);
	double CurrentValue(vector<int> &landUseCodes);
}

#endif // _FITNESSFUNCTION_H_
