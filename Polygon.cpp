#include "Polygon.h"
#include <vector>
#include "ogrsf_frmts.h"

using namespace std;


/*********************************LandUsePolygon****************************/
LandUsePolygon::LandUsePolygon(long id, double area, int useCodeNum, int oldUseCode, LandUseLayer* layer)
{
	ID = id;
	Area = area;
	UseCodeNum = useCodeNum;
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
	double result = Area * Layer->AvgChangeCosts.at(OldUseCode + NewUseCode * UseCodeNum);
	return result;

}


double LandUsePolygon::Suitability()
{
	double result = 0;
	for (int i=0; i != UseCodeNum; ++i)
	{
		double value = Layer->AvgSuitabilities.at(ID * UseCodeNum + i);
		if ( value > result)
		{
			result = value;
		}
	}
	return result;
}


double LandUsePolygon::Compactness()
{
	double result = 0;
	return result;
}




/*********************************LandUseLayer******************************/




/*********************************LayerAssessor*****************************/
