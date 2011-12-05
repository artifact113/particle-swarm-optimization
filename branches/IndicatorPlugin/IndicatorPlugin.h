#ifndef _INDICATORPLUGIN_H_
#define _INDICATORPLUGIN_H_

#include <vector>
#include <QString>
#include <QWidget>
#include <QDomElement>
#include "HPGCToolboxGlobal.h"
using namespace std;

class IndicatorPlugin
{
public:
	/// 构造函数
	IndicatorPlugin()
	{

	}

	/// 析构函数
	virtual ~IndicatorPlugin()
	{

	}

	/// 名称
	virtual QString name()
	{
		return "";
	}

	/// 版本
	virtual QString version()
	{
		return "";
	}	    

	/// 描述
	virtual QString description()
	{
		return "";
	}

	/// 帮助
	virtual QString help()
	{
		return "";
	}

	/// 类型
	PLUGINTYPE type()
	{
		return INDICATOR;
	}

	/// 显示参数设置界面
	virtual void showUI(const QString &dataSource, const QString &fieldName, QDomElement* parameter, QWidget* parent) = 0;

	virtual bool verifyParameter( const QDomElement &parameter ) = 0;

	/// 算法入口
	virtual double pluginMain(const QString &dataSource, const QString &fieldName, const QDomElement &parameter, const vector<QString> &values) = 0;

};

#endif // _INDICATORPLUGIN_H_