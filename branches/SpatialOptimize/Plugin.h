#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <QtCore/qglobal.h>

#ifdef SPATIALOPTIMIZE_LIB
# define SPATIALOPTIMIZE_EXPORT Q_DECL_EXPORT
#else
# define SPATIALOPTIMIZE_EXPORT Q_DECL_IMPORT
#endif

#include <QString>
#include <QWidget>
#include "AlgorithmPlugin.h"
#include "HPGCToolboxGlobal.h"

 
class Plugin : public AlgorithmPlugin	/// 所有的插件必须继承AlgorithmPlugin类
{
public:
	/// 构造函数；
	Plugin();


	/// 析构函数；
	~Plugin();


	/// 名称
	QString name()
	{
		return "";
	}

	/// 版本
	QString version()
	{
		return "";
	}	    

	/// 描述
	QString description()
	{
		return "";
	}

	/// 帮助
	QString help()
	{
		return "";
	}

	/// 显示参数设置界面
	void showUI(const QString &config, const QString &toolName, QWidget* parent);

	/// 算法入口
	int pluginMain(const QString &config);
};

#endif // _PLUGIN_H_