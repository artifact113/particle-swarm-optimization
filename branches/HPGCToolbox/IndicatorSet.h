#ifndef _INDICATORSET_H_
#define _INDICATORSET_H_

#include <vector>

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


#endif // _INDICATORSET_H_