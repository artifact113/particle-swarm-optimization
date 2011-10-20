#ifndef LANDUSELAYER_H
#define LANDUSELAYER_H

#include <vector>
#include "ogrsf_frmts.h"

using namespace std;

class LandUseLayer;


// 土地利用图斑
class LandUsePolygon
{
public:
	/*****************************************************************/
	int ID;					// 唯一标识码
	double Area;			// 面积
	int LandUseCode;		// 原土地利用类型代码
	LandUseLayer* Layer;	// 所属图层

	/*****************************************************************/
	// 构造函数
	LandUsePolygon(int id, double area, int landUseCode);
	~LandUsePolygon();		// 析构函数
};


// 土地利用图层
class LandUseLayer
{
public:
	int UseCodeNum;						// 土地利用代码类型个数
	vector<LandUsePolygon*> Polygons;	// 土地利用图斑集合

	int PolygonsCount;					// 图斑个数
	vector<double> UseAreas;			// 分类统计各地类面积
	double TotalArea;					// 总面积


	/**************************************************************************************/
	// 构造函数
	LandUseLayer(int useCodeNum, vector<LandUsePolygon*> &polygons);	
	~LandUseLayer();					// 析构函数
};


#endif //LANDUSELAYER_H