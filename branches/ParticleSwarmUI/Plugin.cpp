#include "Plugin.h"
#include <QAction>			//QT库的头文件
#include <QIcon>
#include "qgisinterface.h"	///QGIS的头文件
#include "MainForm.h"


//关于插件的信息
static const QString sName = QObject::tr( "SpatialPSO" );
static const QString sDescription = QObject::tr( "Apply PSO algorithm in land use spatial layout optimization" );
static const QString sPluginVersion = QObject::tr( "Version 1.0" );
static const QgisPlugin::PLUGINTYPE sPluginType = QgisPlugin::UI;


Plugin::Plugin(QgisInterface* iface)
{
	mIface = iface;
	mAction = new QAction(QIcon(":/Resources/base.ico"),tr("SpatialPSO"), 0);
}


Plugin::~Plugin()
{
	delete mAction;
}


void Plugin::initGui()
{
	if(mIface)
	{
		connect(mAction, SIGNAL(triggered()), this, SLOT(Entrance()));	/// 对点击插件的响应
		mIface->addToolBarIcon(mAction);                                /// 将插件加载到plugins工具条上
		mIface->addPluginToMenu(sName, mAction);                        /// 将插件加载到plugins菜单项下
	}
}


void Plugin::unload()
{
	mIface->removeToolBarIcon(mAction);			/// 将插件从plugins工具条上卸载
	mIface->removePluginMenu(sName, mAction);   /// 将插件从plugins菜单项下卸载
}


void Plugin::Entrance()
{
	MainForm w;
	w.exec();	

}


/************************************由应用程序调用的函数，无视之**********************************/


QGISEXTERN QgisPlugin* classFactory(QgisInterface* iface)	/// 该函数由应用程序调用，返回插件的实例
{
	Plugin* p  = new Plugin(iface);
	
	return (QgisPlugin*)p;
}


QGISEXTERN QString name()	/// 该函数由应用程序调用，返回插件的名称
{
	return sName;
}


QGISEXTERN QString description()	/// 该函数由应用程序调用，返回插件的描述信息
{	
	return sDescription;
}


QGISEXTERN QString version()	/// 该函数由应用程序调用，返回插件的版本信息
{
	
	return sPluginVersion;
}


QGISEXTERN int type()	/// 该函数由应用程序调用，返回插件的类型信息
{
	return sPluginType;
}


QGISEXTERN void unload(QgisPlugin* thePlugin) /// 该函数由应用程序调用，销毁插件
{
	delete thePlugin;
}

/*************************************************************************************************/