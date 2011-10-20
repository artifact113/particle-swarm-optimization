#include "FitnessFunction.h"
#include <vector>
#include <algorithm>
#include "LandUseLayer.h"

using namespace std;


/**************************************FitnessFunction*************************************/
double FitnessFunction::Score(vector<int> &landUseCodes)
{
	double score = 100;
	double diff = MaxValue - MinValue;
	if (diff < 0.000001)
	{
		return score;
	}	

	double value = CurrentValue(landUseCodes);
	if (IsPositive)
	{
		score = 100 * (value - MinValue) / diff;
	}
	else
	{
		score = 100 * (MaxValue - value) / diff;
	}

	return score;
}



/**************************************AreaFunction*********************************************/
AreaFunction::AreaFunction(LandUseLayer *layer, vector<double> &avgAreas, bool isPositive = true) 
{
	Layer = layer;
	AvgAreas = avgAreas;
	IsPositive = isPositive;

	MaxValue = 0;
	MinValue = 0;

	vector<double> minAreas(Layer->UseCodeNum, 0);
	for (int i=0; i != Layer->UseCodeNum; ++i)
	{
		minAreas.at(i) = AvgAreas.at(i * 2);
	}

	sort(minAreas.begin(), minAreas.end());

	if (Layer->TotalArea < minAreas.at(0))
	{
		MaxValue = 0;
		MinValue = 0;
		return;
	}
	else
	{
		MinValue = 1;
	}

	double minArea = 0;
	for (int j=0; j != Layer->UseCodeNum; ++j)
	{
		minArea += minAreas.at(j * 2);
		if (Layer->TotalArea > minArea)
		{
			++MaxValue;
		}
		else
		{
			break;
		}
	}
}


double AreaFunction::CurrentValue(vector<int> &landUseCodes)
{
	int result = 0;

	vector<double> useAreas(Layer->UseCodeNum, 0);
	for (int i=0; i != landUseCodes.size();  ++i)
	{
		useAreas.at(landUseCodes.at(i)) += Layer->Polygons.at(i)->Area;
	}

	for (int j=0; j != Layer->UseCodeNum;  ++j)
	{
		double minArea = AvgAreas.at(j * 2);
		double maxArea = AvgAreas.at(j * 2 + 1);
		double area = useAreas.at(j);
		if (area > minArea && area < maxArea)
		{
			++result;
		}
	}

	return (double)result;

}



/**************************************BenefitFunction******************************************/
BenefitFunction::BenefitFunction(LandUseLayer *layer, vector<double> &avgBenefits, bool isPositive = true) 
{
	Layer = layer;
	AvgBenefits = avgBenefits;
	IsPositive = isPositive;

	double maxBenefit = *max_element(AvgBenefits.begin(), AvgBenefits.end());
	double minBenefit = *min_element(AvgBenefits.begin(), AvgBenefits.end());

	MaxValue = maxBenefit * Layer->TotalArea;
	MinValue = minBenefit * Layer->TotalArea;
}


double BenefitFunction::CurrentValue(vector<int> &landUseCodes)
{
	double result = 0;

	vector<double> useAreas(Layer->UseCodeNum, 0);
	for (int i=0; i != landUseCodes.size(); ++i)
	{
		useAreas.at(landUseCodes.at(i)) += Layer->Polygons.at(i)->Area;
	}

	for (int j=0; j != useAreas.size(); ++j)
	{
		result += AvgBenefits.at(j) * useAreas.at(j);
	}

	return result;
}



/**************************************ChangeCostFunction***************************************/
ChangeCostFunction::ChangeCostFunction(LandUseLayer *layer, vector<double> &avgChangeCosts, bool isPositive = false) 
{
	Layer = layer;
	AvgChangeCosts = avgChangeCosts;
	IsPositive = isPositive;

	vector<double> maxChangeCosts(Layer->UseCodeNum, 0);
	vector<double> minChangeCosts(Layer->UseCodeNum, 0);	
	for (int i=0; i != Layer->UseCodeNum; ++i)
	{
		vector<double>::iterator iter = AvgChangeCosts.begin();
		vector<double>::iterator iterFirst = iter + i * Layer->UseCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * Layer->UseCodeNum;
		maxChangeCosts.at(i) = *max_element(iterFirst, iterLast);
		minChangeCosts.at(i) = *min_element(iterFirst, iterLast);
	}

	MaxValue = 0;
	MinValue = 0;
	for (int j=0; j != Layer->UseCodeNum; ++j)
	{
		MaxValue += Layer->UseAreas.at(j) * maxChangeCosts.at(j);
		MinValue += Layer->UseAreas.at(j) * minChangeCosts.at(j);
	}
}


double ChangeCostFunction::CurrentValue(vector<int> &landUseCodes)
{
	double result = 0;

	for (int i=0; i != landUseCodes.size(); ++i)
	{
		int oldUseCode = Layer->Polygons.at(i)->LandUseCode;
		int newUseCode = landUseCodes.at(i);
		result += Layer->Polygons.at(i)->Area * AvgChangeCosts.at(oldUseCode * Layer->UseCodeNum + newUseCode);
	}

	return result;
}


/**************************************SuitabilityFunction**************************************/
SuitabilityFunction::SuitabilityFunction(LandUseLayer *layer, vector<double> &avgSuitabilities, bool isPositive = true) 
{
	Layer = layer;
	AvgSuitabilities = avgSuitabilities;
	IsPositive = isPositive;

	vector<double> maxSuitability(Layer->PolygonsCount, 0);
	vector<double> minSuitability(Layer->PolygonsCount, 0);
	for (int i=0; i != Layer->PolygonsCount; ++i)
	{
		vector<double>::iterator iter = AvgSuitabilities.begin();
		vector<double>::iterator iterFirst = iter + i * Layer->UseCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * Layer->UseCodeNum;
		maxSuitability.at(i) = *max_element(iterFirst, iterLast);
		minSuitability.at(i) = *min_element(iterFirst, iterLast);
	}

	MaxValue = 0;
	MinValue = 0;
	for (int j=0; j != Layer->PolygonsCount; ++j)
	{
		MaxValue += maxSuitability.at(j);
		MinValue += minSuitability.at(j);
	}
}


double SuitabilityFunction::CurrentValue(vector<int> &landUseCodes)
{
	double result = 0;

	for (int i=0; i != landUseCodes.size(); ++i)
	{
		int newUseCode = landUseCodes.at(i);
		result += AvgSuitabilities.at(i * Layer->UseCodeNum + newUseCode);
	}	

	return result;
}


/**************************************CompactnessFunction**************************************/
CompactnessFunction::CompactnessFunction(LandUseLayer *layer, vector<vector<int>> &avgCompactnesses, bool isPositive = false) 
{
	Layer = layer;
	AvgCompactnesses = avgCompactnesses;
	IsPositive = isPositive;

	MinValue = 0;
	for (int i=0; i != AvgCompactnesses.size(); ++i)
	{
		MaxValue += AvgCompactnesses.at(i).size();
	}
}


double CompactnessFunction::CurrentValue(vector<int> &landUseCodes)
{
	int result = 0;

	for (int i=0; i != landUseCodes.size(); ++i)
	{
		int newUseCode = landUseCodes.at(i);
		for (int j=0; j != AvgCompactnesses.at(i).size(); ++j)
		{
			int other = landUseCodes.at(AvgCompactnesses.at(i).at(j));
			if (newUseCode != other)
			{
				++result;
			}
		}
	}

	return (double)result;
}


/**************************************LayerAssessor********************************************/
LayerAssessor::LayerAssessor(vector<FitnessFunction*> &functions, vector<double> &weights)
{
	Functions = functions;
	int functionCount = Functions.size();

	Weights.assign(weights.begin(), weights.begin() + functionCount);

	double sum = 0;
	for (int i=0; i != functionCount; ++i)
	{
		sum += Weights.at(i);
	}

	if (sum != 0)
	{
		for (int j=0; j != functionCount; ++j)
		{
			Weights.at(j) /= sum;
		}
	}
}


double LayerAssessor::TotalScore(vector<int> &landUseCodes)
{
	double score = 0;

	int functionCount = Functions.size();
	for (int i=0; i != functionCount; ++i)
	{	
		score += Functions.at(i)->Score(landUseCodes);
	}

	return (score / functionCount);

}