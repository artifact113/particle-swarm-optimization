#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <QtCore/qglobal.h>
#include <QString>
#include "AlgorithmPlugin.h"
#include "Plugin.h"


#ifdef PARTICLESWARMOPTIMIZE_LIB
# define PARTICLESWARMOPTIMIZE_EXPORT Q_DECL_EXPORT
#else
# define PARTICLESWARMOPTIMIZE_EXPORT Q_DECL_IMPORT
#endif

 
class Plugin : public QObject, public AlgorithmPlugin	/// 所有的插件必须继承AlgorithmPlugin类
{
	Q_OBJECT

public:
	/// 构造函数；
	Plugin();

	/// 析构函数；
	~Plugin();

	/// 入口函数，插件所要实现的功能
	void pluginMain();

private:

};



#endif // _PLUGIN_H_