#include "Plugin.h"
#include "AlgorithmPlugin.h"
#include "FormObject.h"



Plugin::Plugin()
{

}


Plugin::~Plugin()
{

}

void Plugin::pluginMain()
{
	FormObject myForm;
	myForm.exec();
}


/************************************由应用程序调用的函数，无视之**********************************/
/// 该函数由应用程序调用，返回插件的实例
extern "C" PARTICLESWARMOPTIMIZE_EXPORT AlgorithmPlugin* classFactory()
{
	Plugin* p  = new Plugin();

	return (AlgorithmPlugin*)p;
}


/// 该函数由应用程序调用，销毁插件
extern "C" PARTICLESWARMOPTIMIZE_EXPORT void unload(AlgorithmPlugin* thePlugin)
{
	delete thePlugin;
}

/*************************************************************************************************/