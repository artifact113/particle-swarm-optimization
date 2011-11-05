#ifndef _PLUGIN_H_
#define _PLUGIN_H_


#include <QtCore/qglobal.h>	///	QT库的头文件
#include <QObject>
#include "qgisplugin.h"		///	QGIS的头文件
#include "HPGCToolbox.h"

class QAction;
class QgisInterface;		/// 该类定义了插件可访问的接口，即插件可以使用的应用程序功能
class QString;



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
	/// 入口函数，插件所要实现的功能
	void pluginMain();

private:
	QgisInterface* _iface;
	QAction* _action;
	HPGCToolbox* _toolbox;
	bool _loadSuceess;
};

#endif // _PLUGIN_H_