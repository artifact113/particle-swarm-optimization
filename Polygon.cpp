#include "Polygon.h"
#include <vector>
#include <algorithm>
#include "ogrsf_frmts.h"

using namespace std;


/*********************************LandUsePolygon****************************/
LandUsePolygon::LandUsePolygon(int id, OGRPolygon* polygon,double area, int landUseCode, LandUseLayer* layer)
{
	_id = id;
	_polygon = polygon;
	_area = area;
	_landUseCode = landUseCode;
	_layer = layer;
}


LandUsePolygon::~LandUsePolygon()
{

}


inline int LandUsePolygon::ID()
{
	return _id;
}


inline OGRPolygon* LandUsePolygon::Polygon()
{
	return _polygon;
}


inline double LandUsePolygon::Area()
{
	return _area;
}


inline int LandUsePolygon::LandUseCode()
{
	return _landUseCode;
}


inline LandUseLayer* LandUsePolygon::Layer()
{
	return _layer;
}



/*********************************LandUseLayer******************************/
LandUseLayer::LandUseLayer(int useCodeNum, vector<LandUsePolygon*> polygons)
{
	_useCodeNum = useCodeNum;
	_polygons = polygons;

	_polygonsCount = CalPolygonsCount();
	_useAreas = CalUseAreas();
	_totalArea = CalTotalArea();

	_avgBenefits(_useCodeNum,0);
	_maxBenefit = 0;
	_minBenefit = 0;

	_avgChangeCosts(_useCodeNum * _useCodeNum, 0);
	_maxChangeCost = 0;
	_minChangeCost = 0;

	_avgSuitabilities(_polygonsCount * _useCodeNum, 0);
	_maxSuitability = 0;
	_minSuitability = 0;

	_avgCompactnesses = CalAvgCompactnesses();
	_maxCompactness = CalMaxCompactness();
	_minCompactness = CalMinCompactness();
}


LandUseLayer::~LandUseLayer()
{

}


inline int LandUseLayer::UseCodeNum()
{
	return _useCodeNum;
}


inline vector<LandUsePolygon*> &LandUseLayer::Polygons()
{
	return _polygons;
}


inline int LandUseLayer::PolygonsCount()
{
	return _polygonsCount;
}


inline vector<double> &LandUseLayer::UseAreas()
{
	return _useAreas;
}


inline double LandUseLayer::TotalArea()
{
	return _totalArea;
}


void LandUseLayer::SetAvgBenefits(vector<double> &avgBenefits)
{
	_avgBenefits.assign(avgBenefits.begin(), avgBenefits.end());
	_maxBenefit = CalMaxBenefit();
	_minBenefit = CalMinBenefit();
}


inline vector<double> &LandUseLayer::GetAvgBenefits()
{
	return _avgBenefits;
}


inline double LandUseLayer::MaxBenefit()
{
	return _maxBenefit;
}


inline double LandUseLayer::MinBenefit()
{
	return _minBenefit;
}


void LandUseLayer::SetAvgChangeCosts(vector<double> &avgChangeCosts)
{
	_avgChangeCosts.assign(avgChangeCosts.begin(), avgChangeCosts.end());
	_maxChangeCost = CalMaxChangeCost();
	_minChangeCost = CalMinChangeCost();
}


inline vector<double> &LandUseLayer::GetAvgChangeCosts()
{
	return _avgChangeCosts;
}


inline double LandUseLayer::MaxChangeCost()
{
	return _maxChangeCost;
}


inline double LandUseLayer::MinChangeCost()
{
	return _minChangeCost;
}


void LandUseLayer::SetAvgSuitabilities(vector<double> &avgSuitabilities)
{
	_avgSuitabilities.assign(avgSuitabilities.begin(), avgSuitabilities.end());
	_maxSuitability = CalMaxSuitability();
	_minSuitability = CalMinSuitability();
}


inline vector<double> &LandUseLayer::GetSuitabilities()
{
	return _avgSuitabilities;
}


inline double LandUseLayer::MaxSuitability()
{
	return _maxSuitability;
}


inline double LandUseLayer::MinSuitability()
{
	return _minSuitability;
}


inline vector<vector<int>> &LandUseLayer::GetAvgCompactnesses()
{
	return _avgCompactnesses;
}


inline double LandUseLayer::MaxCompactness()
{
	return _maxCompactness;
}


inline double LandUseLayer::MinCompactness()
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
	vector<LandUsePolygon*>::iterator iter;
	for (iter = _useAreas.begin(); iter != _useAreas.end(); ++iter)
	{
		result += *iter;
	}
	return result;
}


double LandUseLayer::CalMaxBenefit()
{
	double max = max_element(_avgBenefits.begin(), _avgBenefits.end());
	return max * _totalArea;
}


double LandUseLayer::CalMinBenefit()
{
	double min = min_element(_avgBenefits.begin(), _avgBenefits.end());
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
		maxs.at(i) = max_element(iterFirst, iterLast);
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
		double max = max_element(iterFirst, iterLast);
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
		double min = min_element(iterFirst, iterLast);
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


vector<vector<int>> LandUseLayer::CalAvgCompactnesses()
{
	vector<vector<int>> results;
	for (int i=0; i != _polygonsCount; ++i)
	{
		vector<int> adjacencys;
		OGRPolygons* polygon = _polygons.at(i)->Polygon();
		for (int j=0; j != _polygonsCount; ++j)
		{
			OGRPolygons* other = _polygons.at(j)->Polygon();
			if (polygon->Touches(other))
			{
				adjacencys.push_back(j);
			}
		}

		results.push_back(adjacencys);
	}
	return results;
}


double LandUseLayer::CalMaxCompactness()
{
	double result = 0;
	vector<vector<int>>::iterator iter;
	for (iter=_avgCompactnesses.begin(); iter != _avgCompactnesses.end(); ++iter)
	{
		result += (*iter).size();
	}
	return result;
}


double LandUseLayer::CalMinCompactness()
{
	return _polygonsCount;
}


double LandUseLayer::TotalBenefit()
{
	double result = 0;
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		result += ((*iter)->Benefit());
	}
	return result;
}


double LandUseLayer::TotalChangeCost()
{
	double result = 0;
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		result += ((*iter)->ChangeCost());
	}
	return result;
}


double LandUseLayer::TotalSuitability()
{
	double result = 0;
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		result += ((*iter)->Suitability());
	}
	return result;
}


double LandUseLayer::TotalCompactness()
{
	double result = 0;
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		result += ((*iter)->Compactness());
	}
	return result;
}

/*********************************LayerAssessor*****************************/
LayerAssessor::LayerAssessor(LandUseLayer *layer, double benefitWeight, double changeCostWeight, double suitabilityWeight, double compactnessWeight)
{
	Layer = layer;
	BenefitWeight = benefitWeight;
	ChangeCostWeight = changeCostWeight;
	SuitabilityWeight = suitabilityWeight;
	CompactnessWeight = compactnessWeight;
}


LayerAssessor::~LayerAssessor()
{

}


double LayerAssessor::BenefitScore()
{
	double maxAvgBenefits = *max_element(Layer->AvgBenefits.begin(),Layer->AvgBenefits.end());
	double minAvgBenefits = *min_element(Layer->AvgBenefits.begin(),Layer->AvgBenefits.end());
	double maxTotalBenefit = maxAvgBenefits * Layer->TotalArea();
	double minTotalBenefit = minAvgBenefits * Layer->TotalArea();
	double score = 100 * (Layer->TotalBenefit() - minTotalBenefit)/(maxTotalBenefit - minTotalBenefit);
	return score;
}


double LayerAssessor::ChangeCostScore()
{
	vector<double> maxAvgChangeCosts;
	for (int i=0; i != Layer->UseCodeNum; ++i)
	{
		vector<double>::iterator  iter = Layer->AvgChangeCosts.begin();
		vector<double>::iterator  iterFirst = iter + i * Layer->UseCodeNum;
		vector<double>::iterator  iterLast = iter + (i+1) * Layer->UseCodeNum;
		maxAvgChangeCosts.push_back(*max_element(iterFirst, iterLast));
	}

	vector<double> sumLandUses(Layer->UseCodeNum, 0);
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Layer->Polygons.begin(); iter != Layer->Polygons.end(); ++iter)
	{
		sumLandUses.at((*iter)->OldUseCode) += (*iter)->Area;
	}

	double maxChangeCost = 0;
	for (int j=0; j != Layer->UseCodeNum; ++j)
	{
		maxChangeCost += maxAvgChangeCosts.at(j) * sumLandUses.at(j);
	}


	double score = 100 * (maxChangeCost - Layer->TotalChangeCost()) / maxChangeCost;
	return score;
}


double LayerAssessor::SuitabilityScore()
{
	double maxAvgSuitability = 0;
	double minAvgSuitability = 0;

	vector<double>::iterator iter = Layer->AvgSuitabilities.begin();
	for (int i=0; i != Layer->PolygonsCount(); ++i)
	{
		vector<double>::iterator iterFirst = iter + i * Layer->UseCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * Layer->UseCodeNum;
		double max = *max_element(iterFirst, iterLast);
		double min = *min_element(iterFirst, iterLast);

		maxAvgSuitability += max;
		minAvgSuitability += min;
	}

	double score = 100 * (Layer->TotalSuitability() - minAvgSuitability) / (maxAvgSuitability - minAvgSuitability);
	return score;	
}


double LayerAssessor::CompactnessScore()
{
	double maxCompactness = 100;
	double score = 100 * (maxCompactness - Layer->TotalCompactness())/ maxCompactness;
	return score;
}


double LayerAssessor::TotalScore()
{
	double score = BenefitWeight * BenefitScore() + ChangeCostWeight * ChangeCostScore() + SuitabilityWeight * SuitabilityScore() + CompactnessWeight * CompactnessScore();
	return score/4;
}
