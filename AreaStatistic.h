#ifndef _AREASTATISTIC_H_
#define _AREASTATISTIC_H_

#include <vector>
#include <QString>
#include <QDomElement>
using namespace std;


class AreaStatistic
{
public:
	/// 统计选定字段值的面积
	static double sumArea(const QString &dataSource, const QString &fieldName, const QDomElement &parameter, const vector<QString> &values);

};

#endif // _AREASTATISTIC_H_