#ifndef _ALGORITHMPLUGIN_H_
#define _ALGORITHMPLUGIN_H_

#include <string>
#include "HPGCToolboxMacro.h"


class AlgorithmPlugin
{
public:
	/// 构造函数
	AlgorithmPlugin(const string &name = "",
		const string &version = "",
		const string &description = "",
		const string &help = "",
		const PLUGINTYPE &mytype = ALGORITHM)
		: mName(name),
		mDescription(description),
		mVersion(version),
		mHelp(help),
		mType(mytype)
	{

	}

	/// 析构函数
	virtual ~AlgorithmPlugin()
	{

	}

	/// 名称
	string const & name() const
	{
		return mName;
	}

	string & name()
	{
		return mName;
	}

	/// 版本
	string const & version() const
	{
		return mVersion;
	}

	string & version()
	{
		return mVersion;
	}

	/// 描述
	string const & description() const
	{
		return mDescription;
	}

	string & description()
	{
		return mDescription;
	}

	/// 帮助
	string const & help() const
	{
		return mHelp;
	}

	string & help()
	{
		return mHelp;
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
	virtual void pluginMain() = 0;

private:
	string mName;

	string mVersion;

	string mDescription;

	string mHelp;

	PLUGINTYPE mType;
};

#endif // _ALGORITHMPLUGIN_H_