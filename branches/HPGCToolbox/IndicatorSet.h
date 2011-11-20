#ifndef _INDICATORSET_H_
#define _INDICATORSET_H_

#include <vector>
#include <string>

class IndicatorSet
{
public:
	/// 构造函数
	IndicatorSet();

	/// 析构函数
	~IndicatorSet();

	/// 指标数目
	int count();

	/// 获取所有指标
	vector<Indicator*> getIndicators();

	/// 获取指定ID的指标
	Indicator* getIndicatorByID(int id);

private:
	vector<Indicator*> _indicators;
};


class Indicator
{
public:
	/// 构造函数
	Indicator();

	/// 析构函数
	~Indicator();

	/// 指标得分
	double score();

	/// ID
	void setID(int id);
	int getID();

    
	/// 标签
	string label();

	/// 描述
	string description();

	/// 帮助
	string help();


private:
	int _id;
};
#endif // _INDICATORSET_H_