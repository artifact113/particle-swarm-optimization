#ifndef _ALGORITHMPLUGIN_H_
#define _ALGORITHMPLUGIN_H_

#include <vector>
#include <QObject>
#include <QString>
#include "HPGCToolboxMacro.h"

class AlgorithmPlugin : public QObject
{
	Q_OBJECT

public:
	/// 构造函数
	AlgorithmPlugin(QString const &name = "",
		QString const &version = "",
		QString const &description = "",
		QString const &help = "",
		PLUGINTYPE const & mytype = ALGORITHM)
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
	QString mName;

	QString mVersion;

	QString mDescription;

	QString mHelp;

	PLUGINTYPE mType;
};

#endif // _ALGORITHMPLUGIN_H_