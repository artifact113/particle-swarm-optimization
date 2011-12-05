#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <QtCore/qglobal.h>

#ifdef INDICATORPLUGIN_LIB
# define INDICATORPLUGIN_EXPORT Q_DECL_EXPORT
#else
# define INDICATORPLUGIN_EXPORT Q_DECL_IMPORT
#endif


#include "IndicatorPlugin.h"
#include <string>
using namespace std;

class Plugin : public IndicatorPlugin
{
public:
	/// 构造函数
	Plugin();


	/// 析构函数
	~Plugin();


	/// 名称
	string name()
	{
		return "";
	}

	/// 版本
	string version()
	{
		return "1.0.0";
	}	    

	/// 描述
	string description()
	{
		return "";
	}

	/// 帮助
	string help()
	{
		return "";
	}

	/// 算法入口
	double pluginMain();
};

#endif // _PLUGIN_H_