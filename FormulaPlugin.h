#ifndef _FORMULAPLUGIN_H_
#define _FORMULAPLUGIN_H_

#include <vector>
#include <QString>
#include "HPGCToolboxMacro.h"


class FormulaPlugin
{
public:
	/// 公式类型
	enum FORMULATYPE
	{
		VAR1 = 1,		// 带1个固定参数的公式
		VAR2			// 带2个固定参数的公式
		VAR3			// 带3个固定参数的公式
		VAR4			// 带4个固定参数的公式
		VAR5			// 带5个固定参数的公式
		VARS			// 带可变参数的公式

	};

	/// 构造函数
	FormulaPlugin(QString const &name = "",
		QString const &version = "",
		QString const &description = "",
		QString const &help = "",
		FORMULATYPE const & formulaType = VARS,
		PLUGINTYPE const & mytype = FORMULA)
		: mName(name),
		mDescription(description),
		mVersion(version),
		mHelp(help)
		mType(mytype)
		mFormulaType()
	{

	}

	/// 析构函数
	virtual ~FormulaPlugin()
	{

	}

	/// 名称
	QString const & name() const
	{
		return mName;
	}

	QString & name()
	{
		return mName;
	}

	/// 版本
	QString const & version() const
	{
		return mVersion;
	}

	QString & version()
	{
		return mVersion;
	}

	/// 描述
	QString const & description() const
	{
		return mDescription;
	}

	QString & description()
	{
		return mDescription;
	}

	/// 帮助
	QString const & help() const
	{
		return mHelp;
	}

	QString & help()
	{
		return mHelp;
	}

	/// 公式类型
	FORMULATYPE const & formulaType() const
	{
		return mFormulaType;
	}

	FORMULATYPE & formulaType()
	{
		return mFormulaType;
	}

	/// 类型
	PLUGINTYPE const & type() const
	{
		return mType;
	}

	PLUGINTYPE & type()
	{
		return mType;
	}


	/// 算法入口
	virtual double pluginMain(vector<double> params) = 0;

private:
	QString mName;

	QString mVersion;

	QString mDescription;

	QString mHelp;

	FORMULATYPE mFormulaType;

	PLUGINTYPE mType;	
};
#endif // _FORMULAPLUGIN_H_