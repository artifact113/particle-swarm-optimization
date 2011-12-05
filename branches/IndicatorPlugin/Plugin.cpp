#include "Plugin.h"
#include "IndicatorPlugin.h"
#include "AreaStatisticUI.h"

/// 构造函数
Plugin::Plugin()
{

}


/// 析构函数
Plugin::~Plugin()
{

}


/// 显示参数设置界面
void Plugin::showUI(const QString &dataSource, const QString &fieldName, QDomElement* parameter, QWidget* parent)
{
	AreaStatisticUI myUI(dataSource, fieldName, parameter, parent);
	myUI.show();
}


/// 验证参数合法性
bool verifyParameter(const QDomElement &parameter)
{
	return true;
}


/// 算法入口
double Plugin::pluginMain(const QString &dataSource, const QString &fieldName, const QDomElement &params, const vector<QString> &values)
{
	return 0;
}


/************************************由应用程序调用的函数，无视之**********************************/
/// 该函数由应用程序调用，返回插件的实例
extern "C" INDICATORPLUGIN_EXPORT IndicatorPlugin* classFactory()
{
	Plugin* p  = new Plugin();

	return (IndicatorPlugin*)p;
}


/// 该函数由应用程序调用，销毁插件
extern "C" INDICATORPLUGIN_EXPORT void unload(IndicatorPlugin* thePlugin)
{
	delete thePlugin;
}

/*************************************************************************************************/