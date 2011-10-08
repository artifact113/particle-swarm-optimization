#include "Polygon.h"
#include <vector>
#include <algorithm>
#include "ogrsf_frmts.h"

using namespace std;


/*********************************LandUsePolygon****************************/
LandUsePolygon::LandUsePolygon(long id, double area, int oldUseCode, LandUseLayer* layer)
{
	ID = id;
	Area = area;
	OldUseCode = oldUseCode;
	NewUseCode = oldUseCode;
	Layer = layer;
}


LandUsePolygon::~LandUsePolygon()
{



}


double LandUsePolygon::Benefit()
{
	double result = Area * Layer->AvgBenefits.at(NewUseCode);
	return result;
}


double LandUsePolygon::ChangeCost()
{
	double result = Area * Layer->AvgChangeCosts.at(NewUseCode + OldUseCode * Layer->UseCodeNum);
	return result;

}


double LandUsePolygon::Suitability()
{
	vector<double>::iterator iter =  Layer->AvgSuitabilities.begin();
	vector<double>::iterator iterFirst = iter + ID * Layer->UseCodeNum;
	vector<double>::iterator iterLast = iter + (ID +1) * Layer->UseCodeNum;
	double result = *max_element(iterFirst,iterLast);
	return result;
}


double LandUsePolygon::Compactness()
{
	double result = 0;
	return result;
}




/*********************************LandUseLayer******************************/
LandUseLayer::LandUseLayer(int useCodeNum, vector<LandUsePolygon*> polygons)
{
	UseCodeNum = useCodeNum;
	Polygons = polygons;
}


LandUseLayer::~LandUseLayer()
{
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		delete *iter;
	}
}


int LandUseLayer::PolygonsCount()
{
	return Polygons.size();
}


double LandUseLayer::TotalArea()
{
	double result = 0;
	vector<LandUsePolygon*>::iterator iter;
	for (iter = Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		result += ((*iter)->Area);
	}
	return result;
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
