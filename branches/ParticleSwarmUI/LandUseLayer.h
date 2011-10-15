#ifndef LANDUSELAYER_H
#define LANDUSELAYER_H

#include <vector>
#include <cstdlib>
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

	int PolygonsCount();				// 图斑个数
	vector<double> UseAreas();			// 分类统计各地类面积
	double TotalArea();					// 总面积
	

	/**************************************************************************************/
	vector<double> AvgBenefits;			// 各类土地平均效益
	double MaxBenefit();				// 最大效益
	double MinBenefit();				// 最小效益

	/**************************************************************************************/
	vector<double> AvgChangeCosts;		// 各类土地变更平均花费
	double MaxChangeCost();				// 最大变更费用
	double MinChangeCost();				// 最小变更费用

	/**************************************************************************************/
	vector<double> AvgSuitabilities;	// 每块图斑的用地类型适宜性
	double MaxSuitability();			// 最大适宜度
	double MinSuitability();			// 最小适宜度

	/**************************************************************************************/
	vector<vector<int>> AvgCompactnesses;// 每块图斑的邻接图斑
	double MaxCompactness();			// 最大紧凑度
	double MinCompactness();			// 最小紧凑度


	/**************************************************************************************/
	// 构造函数
	LandUseLayer(int useCodeNum, vector<LandUsePolygon*> &polygons);	
	~LandUseLayer();					// 析构函数
};


// 图层评价
class LayerAssessor
{
public:
	LandUseLayer* Layer;		// 土地利用图层
	vector<int> NewUseCodes;	// 新土地利用类型代码

	/****************************************************************/
	double BenefitWeight;		// 效益权重
	double TotalBenefit();		// 总效益
	double BenefitScore();		// 效益得分

	/****************************************************************/
	double ChangeCostWeight;	// 变更花费权重
	double TotalChangeCost();	// 总变更费用
	double ChangeCostScore();	// 变更花费得分

	/****************************************************************/
	double SuitabilityWeight;	// 适宜性权重
	double TotalSuitability();	// 总适宜性
	double SuitabilityScore();	// 适宜性得分	

	/****************************************************************/
	double CompactnessWeight;	// 空间紧凑度权重
	double TotalCompactness();	// 总紧凑度
	double CompactnessScore();	// 空间紧凑度得分

	/****************************************************************/
	double TotalScore();		// 总得分

	/****************************************************************/
	// 构造函数
	LayerAssessor(LandUseLayer *layer);
	LayerAssessor(LandUseLayer *layer, vector<int> &newUseCodes,double benefitWeight, double changeCostWeight, double suitabilityWeight, double compactnessWeight);
	~LayerAssessor();			// 析构函数

};

#endif //LANDUSELAYER_H