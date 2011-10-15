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
	UseCodeNum = useCodeNum;
	Polygons = polygons;

	AvgBenefits.assign(UseCodeNum,0);

	AvgChangeCosts.assign(UseCodeNum * UseCodeNum, 0);

	AvgSuitabilities.assign(PolygonsCount() * UseCodeNum, 0);

	vector<int> temp;
	AvgCompactnesses.assign(PolygonsCount(), temp);
}


LandUseLayer::~LandUseLayer()
{
	vector<LandUsePolygon*>::iterator iter;
	for (iter=Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		delete *iter;
	}
}


int LandUseLayer::PolygonsCount()
{
	return Polygons.size();
}


vector<double> LandUseLayer::UseAreas()
{
	vector<double> results(UseCodeNum,0);
	vector<LandUsePolygon*>::iterator iter;
	for (iter=Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		results.at((*iter)->LandUseCode) += (*iter)->Area;
	}
	return results;
}


double LandUseLayer::TotalArea()
{
	double result = 0;
	vector<LandUsePolygon*>::iterator iter;
	for (iter=Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		result += (*iter)->Area;
	}
	return result;
}


double LandUseLayer::MaxBenefit()
{
	double max = *max_element(AvgBenefits.begin(), AvgBenefits.end());
	return max * TotalArea();
}


double LandUseLayer::MinBenefit()
{
	double min = *min_element(AvgBenefits.begin(), AvgBenefits.end());
	return min * TotalArea();
}

double LandUseLayer::MaxChangeCost()
{
	vector<double> maxs(UseCodeNum, 0);
	for (int i=0; i != UseCodeNum; ++i)
	{
		vector<double>::iterator iter = AvgChangeCosts.begin();
		vector<double>::iterator iterFirst = iter + i * UseCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * UseCodeNum;
		maxs.at(i) = *max_element(iterFirst, iterLast);
	}

	double result = 0;
	for (int j=0; j != UseCodeNum; ++j)
	{
		result += maxs.at(j) * UseAreas().at(j);
	}
	return result;
}


double LandUseLayer::MinChangeCost()
{
	return 0;
}


double LandUseLayer::MaxSuitability()
{
	vector<double> maxs(PolygonsCount(),0);
	for (int i=0; i != PolygonsCount(); ++i)
	{
		vector<double>::iterator iter = AvgSuitabilities.begin();
		vector<double>::iterator iterFirst = iter + i * UseCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * UseCodeNum;
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


double LandUseLayer::MinSuitability()
{
	vector<double> mins(PolygonsCount(),0);
	for (int i=0; i != PolygonsCount(); ++i)
	{
		vector<double>::iterator iter = AvgSuitabilities.begin();
		vector<double>::iterator iterFirst = iter + i * UseCodeNum;
		vector<double>::iterator iterLast = iter + (i+1) * UseCodeNum;
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


double LandUseLayer::MaxCompactness()
{
	double result = 0;
	vector<vector<int>>::iterator iter;
	for (iter=AvgCompactnesses.begin(); iter != AvgCompactnesses.end(); ++iter)
	{
		result += (*iter).size();
	}
	return result;
}


double LandUseLayer::MinCompactness()
{
	return 0;
}



/*********************************LayerAssessor*****************************/
LayerAssessor::LayerAssessor(LandUseLayer *layer)
{
	Layer = layer;
	NewUseCodes.assign(Layer->PolygonsCount(),0);

	MaxBenefit = layer->MaxBenefit();
	MinBenefit = layer->MinBenefit();
	BenefitWeight = 0;

	MaxChangeCost = layer->MaxChangeCost();
	MinChangeCost = layer->MinChangeCost();
	ChangeCostWeight = 0;

	MaxSuitability = layer->MaxSuitability();
	MinSuitability = layer->MinSuitability();
	SuitabilityWeight = 0;

	MaxCompactness = layer->MaxCompactness();
	MinCompactness = layer->MinCompactness();
	CompactnessWeight = 0;
}


LayerAssessor::LayerAssessor(LandUseLayer *layer, vector<int> &newUseCodes,double benefitWeight, double changeCostWeight, double suitabilityWeight, double compactnessWeight)
{
	Layer = layer;
	NewUseCodes = newUseCodes;
	BenefitWeight = benefitWeight;
	ChangeCostWeight = changeCostWeight;
	SuitabilityWeight = suitabilityWeight;
	CompactnessWeight = compactnessWeight;

	MaxBenefit = layer->MaxBenefit();
	MinBenefit = layer->MinBenefit();

	MaxChangeCost = layer->MaxChangeCost();
	MinChangeCost = layer->MinChangeCost();

	MaxSuitability = layer->MaxSuitability();
	MinSuitability = layer->MinSuitability();

	MaxCompactness = layer->MaxCompactness();
	MinCompactness = layer->MinCompactness();
}


LayerAssessor::~LayerAssessor()
{

}


double LayerAssessor::TotalBenefit()
{
	vector<double> areas(Layer->UseCodeNum,0);
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		areas.at(NewUseCodes.at(i)) += Layer->Polygons.at(i)->Area;
	}

	double result = 0;
	for (int j=0; j != areas.size(); ++j)
	{
		result += (areas.at(j) * Layer->AvgBenefits.at(j));
	}
	return result;
}


double LayerAssessor::BenefitScore()
{
	double score = 100;
	double diff = MaxBenefit - MinBenefit;
	if (diff > 0)
	{
		score = 100 * (TotalBenefit() - MinBenefit) / diff;
	}
	
	return score;
}


double LayerAssessor::TotalChangeCost()
{
	double result = 0;
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		int newUseCode = NewUseCodes.at(i);
		int oldUseCode = Layer->Polygons.at(i)->LandUseCode;
		result += Layer->AvgChangeCosts.at(oldUseCode * Layer->UseCodeNum + newUseCode);
	}
	return result;
}


double LayerAssessor::ChangeCostScore()
{
	double score = 100;
	double diff = MaxChangeCost - MinChangeCost;
	if (diff > 0)
	{
		score = 100 * (MaxChangeCost - TotalChangeCost()) / diff;
	}
	return score;
}
 

double LayerAssessor::TotalSuitability()
{
	double result = 0;
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		result += Layer->AvgSuitabilities.at(i * Layer->UseCodeNum + NewUseCodes.at(i));
	}
	return result;
}


double LayerAssessor::SuitabilityScore()
{
	double score = 100;
	double diff = MaxSuitability - MinSuitability;
	if (diff > 0)
	{
		score = 100 * (TotalSuitability() - MinSuitability) / diff;
	}
	return score;
}


double LayerAssessor::TotalCompactness()
{
	int result = 0;
	for (int i=0; i != NewUseCodes.size(); ++i)
	{
		int useCode = NewUseCodes.at(i);
		for (int j=0; j != Layer->AvgCompactnesses.at(i).size(); ++j)
		{
			int other = NewUseCodes.at(Layer->AvgCompactnesses.at(i).at(j));
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
	double diff = MaxCompactness - MinCompactness;
	if (diff > 0)
	{
		score = 100 * (MaxCompactness - TotalCompactness())/ diff;
	}
	return score;
}


double LayerAssessor::TotalScore()
{

	double benefitScore = 0;
	if(BenefitWeight > 0.0001)
	{
		benefitScore = BenefitWeight * BenefitScore();
	}

	double changeCostScore = 0;
	if (ChangeCostWeight > 0.0001)
	{
		changeCostScore = ChangeCostWeight * ChangeCostScore();
	}

	double suitabilityScore = 0;
	if (SuitabilityWeight > 0.0001)
	{
		suitabilityScore = SuitabilityWeight * SuitabilityScore();
	}

	double compactnessScore = 0;
	if (CompactnessWeight > 0.0001)
	{
		compactnessScore = CompactnessWeight * CompactnessScore();
	}

	double score = benefitScore + changeCostScore + suitabilityScore + compactnessScore;
	return score/4;
}
