#ifndef _AREASTATISTICUI_H_
#define _AREASTATISTICUI_H_


#include "ui_AreaStatisticUI.h"
#include <QObject>
#include <QWidget>
#include <QMap>


class AreaStatisticUI : public Ui::AreaStatisticUI
{
	Q_OBJECT

public:
	explicit AreaStatisticUI(QWidget* parent = 0);
	~AreaStatisticUI();


protected:
private:
	QMap<QString, QString> mapEncode;
};


#endif // _AREASTATISTICUI_H_