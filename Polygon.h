﻿#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <ogr_geometry>

using namespace std;


// 土地利用图斑
class LandUsePolygon
{
	/*************************************************/
	int ID;				// 唯一标识码
	OGRPolygon Shape;	// 图形
	double Area;		// 面积
	int OldUseCode;		// 原土地利用类型代码
	int NewUseCode;		// 新土地利用类型代码

	/*************************************************/
	double Benefit();		// 土地效益
	double ChangeCost();	// 土地变更花费
	double Suitability();	// 土地适宜性
	double Compactness();	// 空间紧凑度

};


// 土地利用图层
class LandUseLayer
{

	int UseCodeNum;						// 土地利用类型数
	vector<LandUsePolygon> Polygons;	// 土地利用图斑集合
	double TotalArea();					// 总面积

	/*****************************************************************/
	vector<double> AvgBenefits;			// 各类土地类型效益，元/亩
	double TotalBenefit();				// 土地总收益

	/****************************************************************/
	vector<double> AvgChangeCosts;		// 各类土地变更花费，元/亩
	double TotalChangeCost();			// 土地变更总花费

	/****************************************************************/
	vector<int> AvgSuitabilities;		// 每块图斑的用地类型适宜性
	double TotalSuitability();			// 土地适宜性总得分


	/****************************************************************/
	double TotalCompactness();			// 土地紧凑度总得分

}


class LayerAssessor
{
	LandUseLayer* layer;		// 土地利用图层

	/****************************************************************/
	double BenefitScore();		// 效益得分
	double BenefitWeight;		// 效益权重

	/****************************************************************/
	double ChangeCostScore();	// 变更花费得分
	double ChangeCostWeight;	// 变更花费权重

	/****************************************************************/
	double SuitabilityScore();	// 适宜性得分
	double SuitabilityWeight;	// 适宜性权重

	/****************************************************************/
	double CompactnessScore();	// 空间紧凑度得分
	double CompactnessWeight;	// 空间紧凑度权重

	/****************************************************************/
	double TotalScore();		// 总得分

}

#endif //POLYGON_H