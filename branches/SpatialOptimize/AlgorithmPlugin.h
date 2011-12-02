#ifndef _ALGORITHMPLUGIN_H_
#define _ALGORITHMPLUGIN_H_

#include <string>
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
	virtual string name()
	{
		return "";
	}

	/// 版本
	virtual string version()
	{
		return "";
	}	    

	/// 描述
	virtual string description()
	{
		return "";
	}

	/// 帮助
	virtual string help()
	{
		return "";
	}

	/// 类型
	PLUGINTYPE type()
	{
		return ALGORITHM;
	}

	/// 算法入口
	virtual void pluginMain() = 0;
};

#endif // _ALGORITHMPLUGIN_H_