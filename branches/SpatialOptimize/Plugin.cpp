#include "Plugin.h"
#include "AlgorithmPlugin.h"
#include "FormObject.h"


/// 构造函数
Plugin::Plugin()
{

}


/// 析构函数
Plugin::~Plugin()
{

}


/// 显示参数设置界面
void Plugin::showUI(const QString &config, const QString &toolName, QWidget* parent)
{
	FormObject myForm(config, toolName, parent);
	myForm.exec();
}

/// 算法入口
int Plugin::pluginMain(const QString &config)
{
	return 0;
}


/************************************由应用程序调用的函数，无视之**********************************/
/// 该函数由应用程序调用，返回插件的实例
extern "C" SPATIALOPTIMIZE_EXPORT AlgorithmPlugin* classFactory()
{
	Plugin* p  = new Plugin();

	return (AlgorithmPlugin*)p;
}


/// 该函数由应用程序调用，销毁插件
extern "C" SPATIALOPTIMIZE_EXPORT void unload(AlgorithmPlugin* thePlugin)
{
	delete thePlugin;
}
/*************************************************************************************************/