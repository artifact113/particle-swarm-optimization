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

	PolygonsCount = Polygons.size();

	UseAreas.assign(UseCodeNum,0);
	vector<LandUsePolygon*>::iterator iter;
	for (iter=Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		UseAreas.at((*iter)->LandUseCode) += (*iter)->Area;
		TotalArea += (*iter)->Area;
	}
}


LandUseLayer::~LandUseLayer()
{
	vector<LandUsePolygon*>::iterator iter;
	for (iter=Polygons.begin(); iter != Polygons.end(); ++iter)
	{
		delete *iter;
	}
}

