#ifndef _AREASTATISTICUI_H_
#define _AREASTATISTICUI_H_


#include "ui_AreaStatisticUI.h"
#include <QObject>
#include <QWidget>
#include <QDomElement>


class AreaStatisticUI : public QWidget, public Ui::AreaStatisticUI
{
	Q_OBJECT

public:
	explicit AreaStatisticUI(const QString &dataSource, const QString &fieldName, QDomElement* parameter,QWidget* parent = 0);
	~AreaStatisticUI();

	/// 数据初始化
	void initData();

public slots:
	/// 改变字段值
	void changeFieldValue(const QString &fieldValue);

protected:

private:
	const QString mDataSource;		// 数据源
	const QString mFiledName;		// 数据字段
	QDomElement* mParameter;		// 算法参数 

};
#endif // _AREASTATISTICUI_H_