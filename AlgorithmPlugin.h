#ifndef _ALGORITHMPLUGIN_H_
#define _ALGORITHMPLUGIN_H_

#include <QString>
#include <QWidget>
#include "HPGCToolboxGlobal.h"

using namespace std;

class AlgorithmPlugin
{
public:
	/// 构造函数
	AlgorithmPlugin()
	{

	}

	/// 析构函数
	virtual ~AlgorithmPlugin()
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
		return ALGORITHM;
	}

	/// 显示参数设置界面
	virtual void showUI(const QString &config, const QString &toolName, QWidget* parent) = 0;

	/// 算法入口
	virtual int pluginMain(const QString &config) = 0;

};

#endif // _ALGORITHMPLUGIN_H_