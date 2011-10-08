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
	vector<int>::iterator iter = AvgSuitabilities.begin();
	vector<int>::iterator iterFirst = iter + ID * Layer->UseCodeNum;
	vector<int>::iterator iterLast = iter + (ID +1) * Layer->UseCodeNum - 1;
	int result = max_element(iterFirst,iterLast);
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





