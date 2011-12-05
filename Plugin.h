#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <QtCore/qglobal.h>

#ifdef INDICATORPLUGIN_LIB
# define INDICATORPLUGIN_EXPORT Q_DECL_EXPORT
#else
# define INDICATORPLUGIN_EXPORT Q_DECL_IMPORT
#endif


#include "IndicatorPlugin.h"
#include <vector>
#include <QString>
#include <QDomElement>
using namespace std;


class Plugin : public IndicatorPlugin
{
public:
	/// 构造函数
	Plugin();


	/// 析构函数
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
	void showUI(const QString &dataSource, const QString &fieldName, QDomElement* parameter, QWidget* parent);

	/// 算法入口
	double pluginMain(const QString &dataSource, const QString &fieldName, const QDomElement &parameter, const vector<QString> &values);

};

#endif // _PLUGIN_H_