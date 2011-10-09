﻿#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "ogrsf_frmts.h"

using namespace std;

class LandUseLayer;


// 土地利用图斑
class LandUsePolygon
{
private:
	int _id;
	OGRPolygon* _polygon;
	double _area;
	int _landUseCode;
	LandUseLayer* _layer;

public:
	/*****************************************************************/
	int ID();				// 唯一标识码
	OGRPolygon* Polygon();	// 图形
	double Area();			// 面积
	int LandUseCode();		// 原土地利用类型代码
	LandUseLayer* Layer();	// 所属图层

	/*****************************************************************/
	// 构造函数
	LandUsePolygon(int id, OGRPolygon* polygon,double area, int landUseCode, LandUseLayer* layer);
	~LandUsePolygon();		// 析构函数

};


// 土地利用图层
class LandUseLayer
{
private:
	int _useCodeNum;					
	vector<LandUsePolygon*> _polygons;

	int _polygonsCount;					
	double _totalArea;

	vector<double> _avgBenefits;	
	double _maxBenefit;
	double _minBenefit;

	vector<double> _avgChangeCosts;
	double _maxChangeCost;
	double _minChangeCost;

	vector<double> _avgSuitabilities;
	double _maxSuitability;
	double _minSuitability;

	double _maxCompactness;
	double _minCompactness;

	/****************************************************************/
	int CalPolygonsCount();				
	double CalTotalArea();
	double CalMaxBenefit();
	double CalMinBenefit();
	double CalMaxChangeCost();
	double CalMinChangeCost();
	double CalMaxSuitability();
	double CalMinSuitability();
	double CalMaxCompactness();
	double CalMinCompactness();

public:
	int UseCodeNum();										// 土地利用代码类型个数
	vector<LandUsePolygon*> &Polygons();					// 土地利用图斑集合

	int PolygonsCount();									// 图斑个数
	double TotalArea();										// 总面积

	/**************************************************************************************/
	void AvgBenefits(vector<double> &avgBenefits);			// 各类土地类型效益，元/亩
	double MaxBenefit();									// 最大效益
	double MinBenefit();									// 最小效益

	/**************************************************************************************/
	void AvgChangeCosts(vector<double> &avgChangeCosts);	// 各类土地变更花费，元/亩
	double MaxChangeCost();									// 最大变更费用
	double MinChangeCost();									// 最小变更费用

	/**************************************************************************************/
	void AvgSuitabilities(vector<double> &avgSuitabilities);// 每块图斑的用地类型适宜性
	double MaxSuitability();								// 最大适宜度
	double MinSuitability();								// 最小适宜度

	/**************************************************************************************/
	double MaxCompactness();								// 最大紧凑度
	double MinCompactness();								// 最小紧凑度


	/**************************************************************************************/
	// 构造函数
	LandUseLayer(int useCodeNum, vector<LandUsePolygon*> polygons);	
	~LandUseLayer();					// 析构函数

};


// 图层评价
class LayerAssessor
{
public:
	LandUseLayer* Layer;		// 土地利用图层
	vector<int> NewUseCodes		// 新土地利用类型代码

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

	/****************************************************************/
	// 构造函数
	LayerAssessor(LandUseLayer *layer, double benefitWeight, double changeCostWeight, double suitabilityWeight, double compactnessWeight);
	~LayerAssessor();			// 析构函数

};

#endif //POLYGON_H