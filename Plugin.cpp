#include "Plugin.h"
#include <QAction>			//QT库的头文件
#include <QIcon>
#include <QDockWidget>
#include "qgisinterface.h"	///QGIS的头文件
#include "HPGCToolbox.h"



//关于插件的信息
static const QString sName = QObject::tr( "HPGCToolbox" );
static const QString sDescription = QObject::tr("High Performance Geographic Computing Toolbox");
static const QString sPluginVersion = QObject::tr( "Version 1.0" );
static const QgisPlugin::PLUGINTYPE sPluginType = QgisPlugin::UI;


Plugin::Plugin(QgisInterface* iface)
{
	_iface = iface;
	_action = new QAction(QIcon(":/toolbox"),tr("HPGCToolbox"), 0);
	_toolbox =new HPGCToolbox(_iface, QString("HPGCToolbox"));
	_toolbox->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

}


Plugin::~Plugin()
{
	delete _toolbox;
	delete _action;	
}


void Plugin::initGui()
{
	if(_iface)
	{
		connect(_action, SIGNAL(triggered()), this, SLOT(pluginMain()));	/// 对点击插件的响应
		_iface->addToolBarIcon(_action);									/// 将插件加载到plugins工具条上
		_iface->addPluginToMenu(sName, _action);							/// 将插件加载到plugins菜单项下
	}
}


void Plugin::unload()
{
	_iface->removeToolBarIcon(_action);										/// 将插件从plugins工具条上卸载
	_iface->removePluginMenu(sName, _action);								/// 将插件从plugins菜单项下卸载
}


void Plugin::pluginMain()
{
	if (_toolbox->isVisible())
	{
		_iface->removeDockWidget(_toolbox);
	}
	else
	{
		_iface->addDockWidget(Qt::RightDockWidgetArea, _toolbox);
	}
}


/************************************由应用程序调用的函数，无视之**********************************/
/// 该函数由应用程序调用，返回插件的实例
QGISEXTERN QgisPlugin* classFactory(QgisInterface* iface)
{
	Plugin* p  = new Plugin(iface);
	
	return (QgisPlugin*)p;
}


/// 该函数由应用程序调用，返回插件的名称
QGISEXTERN QString name()
{
	return sName;
}


/// 该函数由应用程序调用，返回插件的描述信息
QGISEXTERN QString description()
{	
	return sDescription;
}


/// 该函数由应用程序调用，返回插件的版本信息
QGISEXTERN QString version()
{	
	return sPluginVersion;
}


/// 该函数由应用程序调用，返回插件的类型信息
QGISEXTERN int type()
{
	return sPluginType;
}

/// 该函数由应用程序调用，销毁插件
QGISEXTERN void unload(QgisPlugin* thePlugin)
{
	delete thePlugin;
}

/*************************************************************************************************/