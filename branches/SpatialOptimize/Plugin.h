#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <QtCore/qglobal.h>

#ifdef SPATIALOPTIMIZE_LIB
# define SPATIALOPTIMIZE_EXPORT Q_DECL_EXPORT
#else
# define SPATIALOPTIMIZE_EXPORT Q_DECL_IMPORT
#endif

#include <string>
#include "AlgorithmPlugin.h"
#include "HPGCToolboxGlobal.h"

using namespace std;

 
class Plugin : public AlgorithmPlugin	/// 所有的插件必须继承AlgorithmPlugin类
{
public:
	/// 构造函数；
	Plugin();


	/// 析构函数；
	~Plugin();


	/// 名称
	string name()
	{
		return "";
	}

	/// 版本
	string version()
	{
		return "";
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

	/// 入口函数，插件所要实现的功能
	void pluginMain();
};

#endif // _PLUGIN_H_