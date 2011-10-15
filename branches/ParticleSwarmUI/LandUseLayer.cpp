#include "LandUseLayer.h"
#include <vector>
#include <algorithm>
#include "ogrsf_frmts.h"

using namespace std;


/*********************************LandUsePolygon****************************/
LandUsePolygon::LandUsePolygon(int id, double area, int landUseCode)
{
	ID = id;
	Area = area;
	LandUseCode = landUseCode;
}


LandUsePolygon::~LandUsePolygon()
{

}


/*********************************LandUseLayer******************************/
LandUseLayer::LandUseLayer(int useCodeNum, vector<LandUsePolygon*> &polygons)
{
	Polygons = polygons;

	_polygonsCount = CalPolygonsCount();
	_useAreas = CalUseAreas();
	_totalArea = CalTotalArea();

	_avgBenefits.assign(_useCodeNum,0);
	_maxBenefit = 0;
	_minBenefit = 0;

	_avgChangeCosts.assign(_useCodeNum * _useCodeNum, 0);
	_maxChangeCost = 0;
	_minChangeCost = 0;

	_avgSuitabilities.assign(_polygonsCount * _useCodeNum, 0);
	_maxSuitability = 0;
	_minSuitability = 0;

	for (int i=0; i != _polygonsCount; ++i)
	{
		vector<int> a(1,i);
		_avgCompactnesses.push_back(a);

	}
	_maxCompactness = 0;
	_minCompactness = 0;
}


LandUseLayer::~LandUseLayer()
{
	vector<LandUsePolygon*>::iterator iter;
	for (iter=_polygons.begin(); iter != _polygons.end(); ++iter)
	{
		delete *iter;
	}
}


 int LandUseLayer::UseCodeNum()
{
	return _useCodeNum;
}


 vector<LandUsePolygon*> &LandUseLayer::Polygons()
{
	return _polygons;
}


int LandUseLayer::PolygonsCount()
{
	return _polygonsCount;
}


vector<double> &LandUseLayer::UseAreas()
{
	return _useAreas;
}


double LandUseLayer::TotalArea()
{
	return _totalArea;
}


void LandUseLayer::SetAvgBenefits(vector<double> &avgBenefits)
{
	_avgBenefits.assign(avgBenefits.begin(), avgBenefits.end());
	_maxBenefit = CalMaxBenefit();
	_minBenefit = CalMinBenefit();
}


 vector<double> &LandUseLayer::GetAvgBenefits()
{
	return _avgBenefits;
}


 double LandUseLayer::MaxBenefit()
{
	return _maxBenefit;
}


 double LandUseLayer::MinBenefit()
{
	return _minBenefit;
}


void LandUseLayer::SetAvgChangeCosts(vector<double> &avgChangeCosts)
{
	_avgChangeCosts.assign(avgChangeCosts.begin(), avgChangeCosts.end());
	_maxChangeCost = CalMaxChangeCost();
	_minChangeCost = CalMinChangeCost();
}


 vector<double> &LandUseLayer::GetAvgChangeCosts()
{
	return _avgChangeCosts;
}


 double LandUseLayer::MaxChangeCost()
{
	return _maxChangeCost;
}


 double LandUseLayer::MinChangeCost()
{
	return _minChangeCost;
}


void LandUseLayer::SetAvgSuitabilities(vector<double> &avgSuitabilities)
{
	_avgSuitabilities.assign(avgSuitabilities.begin(), avgSuitabilities.end());
	_maxSuitability = CalMaxSuitability();
	_minSuitability = CalMinSuitability();
}


 vector<double> &LandUseLayer::GetAvgSuitabilities()
{
	return _avgSuitabilities;
}


 double LandUseLayer::MaxSuitability()
{
	return _maxSuitability;
}


 double LandUseLayer::MinSuitability()
{
	return _minSuitability;
}


void LandUseLayer::SetAvgCompactnesses(vector<vector<int>> &avgCompactnesses)
{
	_avgCompactnesses.assign(avgCompactnesses.begin(), avgCompactnesses.end());
	_maxCompactness = CalMaxCompactness();
	_minCompactness = CalMinCompactness();
}


 vector<vector<int>> &LandUseLayer::GetAvgCompactnesses()
{
	return _avgCompactnesses;
}


 double LandUseLayer::MaxCompactness()
{
	return _maxCompactness;
}


 double LandUseLayer::MinCompactness()
{
	return _minCompactness;
}


int LandUseLayer::CalPolygonsCount()
{
	return _polygons.size();
}


vector<double> LandUseLayer::CalUseAreas()
{
	vector<double> results(_useCodeNum,0);
	vector<LandUsePolygon*>::iterator iter;
	for ( iter=_polygons.begin(); iter != _polygons.end(); ++iter )
	{
		results.at((*iter)->LandUseCode()) += (*iter)->Area();
	}
	return results;
}


double LandUseLayer::CalTotalArea()
{
	double result = 0;
	vector<double>::iterator iter;
	for (iter = _useAreas.begin(); iter != _useAreas.end(); ++iter)
	{
		result += *iter;
	}
	return result;
}


double LandUseLayer::CalMaxBenefit()
{
	double max = *max_element(_avgBenefits.begin(), _avgBenefits.end());
	return max * _totalArea;
}


double LandUseLayer::CalMinBenefit()
{
	double min = *min_element(_avgBenefits.begin(), _avgBenefits.end());
	return min * _totalArea;
}


double LandUseLayer::CalMaxChangeCost()
{
	vector<double> maxs(_useCodeNum, 0);
	for (int i=0; i != _useCodeNum; ++i)
	{
		vector<double>::iterator iter = _avgChangeCosts.begin();
		vector<double>::iterator iterFirst = iter + i * _useCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * _useCodeNum;
		maxs.at(i) = *max_element(iterFirst, iterLast);
	}

	double result = 0;
	for (int j=0; j != _useCodeNum; ++j)
	{
		result += maxs.at(j) * _useAreas.at(j);
	}
	return result;
}


double LandUseLayer::CalMinChangeCost()
{
	return 0;
}


double LandUseLayer::CalMaxSuitability()
{
	vector<double> maxs(_polygonsCount,0);
	for (int i=0; i != _polygonsCount; ++i)
	{
		vector<double>::iterator iter = _avgSuitabilities.begin();
		vector<double>::iterator iterFirst = iter + i * _useCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * _useCodeNum;
		double max = *max_element(iterFirst, iterLast);
		maxs.at(i) = max;
	}

	double result = 0;
	vector<double>::iterator iter;
	for (iter=maxs.begin(); iter != maxs.end(); ++iter)
	{
		result += *iter;
	}
	return result;
}


double LandUseLayer::CalMinSuitability()
{
	vector<double> mins(_polygonsCount,0);
	for (int i=0; i != _polygonsCount; ++i)
	{
		vector<double>::iterator iter = _avgSuitabilities.begin();
		vector<double>::iterator iterFirst = iter + i * _useCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * _useCodeNum;
		double min = *min_element(iterFirst, iterLast);
		mins.at(i) = min;
	}

	double result = 0;
	vector<double>::iterator iter;
	for (iter=mins.begin(); iter != mins.end(); ++iter)
	{
		result += *iter;
	}
	return result;
}


double LandUseLayer::CalMaxCompactness()
{
	double result = 0;
	vector<vector<int>>::iterator iter;
	for (iter=_avgCompactnesses.begin(); iter != _avgCompactnesses.end(); ++iter)
	{
		result += (*iter).size();
	}
	return result - _polygonsCount;
}


double LandUseLayer::CalMinCompactness()
{
	return 0;
}



/*********************************LayerAssessor*****************************/
LayerAssessor::LayerAssessor(LandUseLayer *layer)
{
	Layer = layer;
	NewUseCodes.assign(Layer->PolygonsCount(),0);
	BenefitWeight = 0;
	ChangeCostWeight = 0;
	SuitabilityWeight = 0;
	CompactnessWeight = 0;
}


LayerAssessor::LayerAssessor(LandUseLayer *layer, vector<int> &newUseCodes,double benefitWeight, double changeCostWeight, double suitabilityWeight, double compactnessWeight)
{
	Layer = layer;
	NewUseCodes.assign(newUseCodes.begin(),newUseCodes.begin() + Layer->PolygonsCount());
	BenefitWeight = benefitWeight;
	ChangeCostWeight = changeCostWeight;
	SuitabilityWeight = suitabilityWeight;
	CompactnessWeight = compactnessWeight;
}


LayerAssessor::~LayerAssessor()
{

}


double LayerAssessor::TotalBenefit()
{
	vector<double> areas(Layer->UseCodeNum(),0);
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		areas.at(NewUseCodes.at(i)) += Layer->Polygons().at(i)->Area();
	}

	double result = 0;
	for (int j=0; j != Layer->UseCodeNum(); ++j)
	{
		result += (areas.at(j) * Layer->GetAvgBenefits().at(j));
	}
	return result;
}


double LayerAssessor::BenefitScore()
{
	double score = 100;
	double maxTotalBenefit = Layer->MaxBenefit();
	double minTotalBenefit = Layer->MinBenefit();
	double diff = maxTotalBenefit - minTotalBenefit;
	if (diff > 0)
	{
		score = 100 * (TotalBenefit() - minTotalBenefit) / diff;
	}
	
	return score;
}


double LayerAssessor::TotalChangeCost()
{
	double result = 0;
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		int newUseCode = NewUseCodes.at(i);
		int oldUseCode = Layer->Polygons().at(i)->LandUseCode();
		result += Layer->GetAvgChangeCosts().at(oldUseCode * Layer->UseCodeNum() + newUseCode);
	}
	return result;
}


double LayerAssessor::ChangeCostScore()
{
	double score = 100;
	double maxChangeCost = Layer->MaxChangeCost();
	double minChangeCost = Layer->MinChangeCost();
	double diff = maxChangeCost - minChangeCost;
	if (diff > 0)
	{
		score = 100 * (maxChangeCost - TotalChangeCost()) / diff;
	}
	return score;
}
 

double LayerAssessor::TotalSuitability()
{
	double result = 0;
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		result += Layer->GetAvgSuitabilities().at(i * Layer->UseCodeNum() + NewUseCodes.at(i));
	}
	return result;
}


double LayerAssessor::SuitabilityScore()
{
	double score = 100;
	double maxSuitability = Layer->MaxSuitability();
	double minSuitability = Layer->MinSuitability();
	double diff = maxSuitability - minSuitability;
	if (diff > 0)
	{
		score = 100 * (TotalSuitability() - minSuitability) / diff;
	}
	return score;	
}


double LayerAssessor::TotalCompactness()
{
	int result = 0;
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		int useCode = NewUseCodes.at(i);
		for (int j=0; j != Layer->GetAvgCompactnesses().at(i).size(); ++j)
		{
			int other = NewUseCodes.at(Layer->GetAvgCompactnesses().at(i).at(j));
			if (useCode != other)
			{
				++result;
			}
		}
	}
	return (double)result;
}


double LayerAssessor::CompactnessScore()
{
	double score = 100;
	double maxCompactness = Layer->MaxCompactness();
	double minCompactness = Layer->MinCompactness();
	double diff = maxCompactness - minCompactness;
	if (diff > 0)
	{
		score = 100 * (maxCompactness - TotalCompactness())/ maxCompactness;
	}
	return score;
}


double LayerAssessor::TotalScore()
{
	double score = BenefitWeight * BenefitScore() + ChangeCostWeight * ChangeCostScore() + SuitabilityWeight * SuitabilityScore() + CompactnessWeight * CompactnessScore();
	return score/4;
}
