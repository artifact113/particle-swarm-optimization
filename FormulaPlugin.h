#ifndef _FORMULAPLUGIN_H_
#define _FORMULAPLUGIN_H_

#include <vector>
#include <string>
#include "HPGCToolboxGlobal.h"

using namespace std;


class FormulaPlugin
{
public:
	/// 公式类型
	enum FORMULATYPE
	{
		VAR1 = 1,		// 带1个固定参数的公式
		VAR2,			// 带2个固定参数的公式
		VAR3,			// 带3个固定参数的公式
		VAR4,			// 带4个固定参数的公式
		VAR5,			// 带5个固定参数的公式
		VARS,			// 带可变参数的公式

	};

	/// 构造函数
	FormulaPlugin()
	{

	}

	/// 析构函数
	virtual ~FormulaPlugin()
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

	/// 公式类型
	virtual FORMULATYPE formulaType()
	{
		return VARS;
	}

	/// 类型
	PLUGINTYPE type()
	{
		return FORMULA;
	}

	/// 算法入口
	virtual double pluginMain(vector<double> params) = 0;
};
#endif // _FORMULAPLUGIN_H_