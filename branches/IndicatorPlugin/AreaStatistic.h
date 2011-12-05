#ifndef _AREASTATISTIC_H_
#define _AREASTATISTIC_H_

#include <vector>
#include <QString>

using namespace std;


class AreaStatistic
{
public:
	/// 统计选定字段值的面积
	static double sumArea(const QString &filename, const QString &value, vector<QString> &results);

};

#endif // _AREASTATISTIC_H_