#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include <ogr_geometry>

using namespace std;


// 土地利用图斑
class LandUsePolygon
{
	/*************************************************/
	string ID;			// 唯一标识码
	OGRPolygon Shape;	// 图形
	double Area;		// 面积
	double Circle;		// 周长

	/*************************************************/
	int OldUseCode;		// 原土地利用类型代码
	int NewUseCode;		// 新土地利用类型代码
	double Benefit;		// 土地效益
	double ChangeCost;	// 土地变更花费
	double Fitness;		// 土地适宜性

};


// 土地利用图层
class LandUseLayer
{

	int STYLE_NUM;						// 土地利用类型数

	vector<LandUsePolygon> Polygons;	// 土地利用图斑

	/*****************************************************************/
	vector<double> AvgBenefits;			// 各类土地类型效益，元/亩
	double TotalBenefit;				// 土地总收益

	/****************************************************************/
	vector<double> AvgChangeCosts;		// 各类土地变更花费，元/亩
	double TotalChangeCost;				// 土地变更总花费

	/****************************************************************/
	vector<int> AvgFitnesses;			// 每块图斑的用地类型适宜性
	double TotalFitness;				// 土地适宜性总得分


	/****************************************************************/
	double TotalCompactness;			// 土地紧凑度总得分

}

#endif //POLYGON_H