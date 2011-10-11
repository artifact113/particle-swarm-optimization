#ifndef LANDUSELAYER_H
#define LANDUSELAYER_H

#include <vector>
#include "ogrsf_frmts.h"

using namespace std;

class LandUseLayer;


// 土地利用图斑
class LandUsePolygon
{
private:
	int _id;
	double _area;
	int _landUseCode;

public:
	/*****************************************************************/
	int ID();				// 唯一标识码
	double Area();			// 面积
	int LandUseCode();		// 原土地利用类型代码
	LandUseLayer* Layer;	// 所属图层

	/*****************************************************************/
	// 构造函数
	LandUsePolygon(int id, double area, int landUseCode);
	~LandUsePolygon();		// 析构函数

};


// 土地利用图层
class LandUseLayer
{
private:
	int _useCodeNum;					
	vector<LandUsePolygon*> _polygons;

	int _polygonsCount;
	vector<double> _useAreas;
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

	vector<vector<int>> _avgCompactnesses;
	double _maxCompactness;
	double _minCompactness;

	/****************************************************************/
	int CalPolygonsCount();
	vector<double> CalUseAreas();
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
	vector<double> &UseAreas();								// 分类统计各地类面积
	double TotalArea();										// 总面积

	/**************************************************************************************/
	void SetAvgBenefits(vector<double> &avgBenefits);		// 各类土地类型效益，元/亩
	vector<double> &GetAvgBenefits();
	double MaxBenefit();									// 最大效益
	double MinBenefit();									// 最小效益

	/**************************************************************************************/
	void SetAvgChangeCosts(vector<double> &avgChangeCosts);	// 各类土地变更花费，元/亩
	vector<double> &GetAvgChangeCosts();
	double MaxChangeCost();									// 最大变更费用
	double MinChangeCost();									// 最小变更费用

	/**************************************************************************************/
	void SetAvgSuitabilities(vector<double> &avgSuitabilities);// 每块图斑的用地类型适宜性
	vector<double> &GetAvgSuitabilities();
	double MaxSuitability();								// 最大适宜度
	double MinSuitability();								// 最小适宜度

	/**************************************************************************************/
	void SetAvgCompactnesses(vector<vector<int>> &avgCompactnesses);// 每块图斑的邻接图斑
	vector<vector<int>> &GetAvgCompactnesses();				// 图斑空间邻接表
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
private:
	LandUseLayer* Layer;		// 土地利用图层

public:	
	vector<int> NewUseCodes;	// 新土地利用类型代码

	/****************************************************************/
	double TotalBenefit();		// 总效益
	double BenefitScore();		// 效益得分
	double BenefitWeight;		// 效益权重

	/****************************************************************/
	double TotalChangeCost();	// 总变更费用
	double ChangeCostScore();	// 变更花费得分
	double ChangeCostWeight;	// 变更花费权重

	/****************************************************************/
	double TotalSuitability();	// 总适宜性
	double SuitabilityScore();	// 适宜性得分
	double SuitabilityWeight;	// 适宜性权重

	/****************************************************************/
	double TotalCompactness();	// 总紧凑度
	double CompactnessScore();	// 空间紧凑度得分
	double CompactnessWeight;	// 空间紧凑度权重

	/****************************************************************/
	double TotalScore();		// 总得分

	/****************************************************************/
	// 构造函数
	LayerAssessor(LandUseLayer *layer);
	LayerAssessor(LandUseLayer *layer, vector<int> &newUseCodes,double benefitWeight, double changeCostWeight, double suitabilityWeight, double compactnessWeight);
	~LayerAssessor();			// 析构函数

};

#endif //LANDUSELAYER_H