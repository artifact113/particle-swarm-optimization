#ifndef _PLUGIN_H_
#define _PLUGIN_H_


#include <QtCore/qglobal.h>
#include <QObject>
#include <QString>
#include <QAction>
#include "qgisplugin.h"
#include "qgisinterface.h"
#include "HPGCToolbox.h"


class Plugin : public QObject, public QgisPlugin	/// 所有的插件必须继承QgisPlugin类
{
	Q_OBJECT

public:
	/// 构造函数；
	Plugin(QgisInterface* iface);

	/// 析构函数；
	virtual ~Plugin();

	/// 将插件连接到图形用户界面;
	void initGui();

	/// 将插件从图形用户界面上卸载;
    void unload();
	
public slots:
	/// 入口函数，对应插件点击事件；
	void pluginMain();

private:
	QgisInterface* _iface;
	QAction* _action;
	HPGCToolbox* _toolbox;
	bool _loadSuceess;
};

#endif // _PLUGIN_H_