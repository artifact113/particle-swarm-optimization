#include "Plugin.h"
#include "IndicatorPlugin.h"


/// 构造函数
Plugin::Plugin()
{

}


/// 析构函数
Plugin::~Plugin()
{

}


/// 算法入口
double Plugin::pluginMain()
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