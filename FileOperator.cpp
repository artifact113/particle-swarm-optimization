#include "FileOperator.h"
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QMessageBox>
#include <QLibrary>
#include "HPGCToolboxGlobal.h"
#include "AlgorithmPlugin.h"
#include "IndicatorPlugin.h"
#include "FormulaPlugin.h"


/// 验证文件
bool FileOperator::VerifyFile(const QString &filename, const PLUGINTYPE &type)
{
	if (type == ALGORITHM)
	{
		return VerifyAlgorithmFile(filename);
	}
	else if (type == INDICATOR)
	{
		return VerifyIndicatorFile(filename);
	}
	else if (type == FORMULA)
	{
		return VerifyFormulaFile(filename);
	}

	return false;
}

/// 验证算法包
bool FileOperator::VerifyAlgorithmFile(const QString &filename)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	QLibrary myLibrary(filename);
	typedef AlgorithmPlugin* (*TypeClassFactory)();
	typedef void* (*TypeUnload)(AlgorithmPlugin*);
	TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
	TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");

	if (pClassFactory && pUnload)
	{
		AlgorithmPlugin* myPlugin = (*pClassFactory)();
		PLUGINTYPE myType = myPlugin->type();
		(*pUnload)(myPlugin);
		if (myType == ALGORITHM)
		{
			return true;
		}
	}

	return false;
}


/// 验证指标计算包
bool FileOperator::VerifyIndicatorFile(const QString &filename)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	QLibrary myLibrary(filename);
	typedef IndicatorPlugin* (*TypeClassFactory)();
	typedef void* (*TypeUnload)(IndicatorPlugin*);
	TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
	TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");

	if (pClassFactory && pUnload)
	{
		IndicatorPlugin* myPlugin = (*pClassFactory)();
		PLUGINTYPE myType = myPlugin->type();
		(*pUnload)(myPlugin);
		if (myType == INDICATOR)
		{
			return true;
		}
	}

	return false;
}


/// 验证公式包
bool FileOperator::VerifyFormulaFile(const QString &filename)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	QLibrary myLibrary(filename);
	typedef FormulaPlugin* (*TypeClassFactory)();
	typedef void* (*TypeUnload)(FormulaPlugin*);
	TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
	TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");

	if (pClassFactory && pUnload)
	{
		FormulaPlugin* myPlugin = (*pClassFactory)();
		PLUGINTYPE myType = myPlugin->type();
		(*pUnload)(myPlugin);
		if (myType == FORMULA)
		{
			return true;
		}
	}

	return false;
}


/// 复制文件
bool FileOperator::CopyFile(const QString &from, const QString &to)
{
	QFileInfo fromFileInfo(from);

	// 1.检查文件是否存在
	if (!fromFileInfo.exists())
	{
		return false;
	}

	QFile formFile(from);

	// 2.检查是否存在同名dll文件
	QFileInfo toFileInfo(to);
	if (toFileInfo.exists()) // 同名
	{
		// 同名不同位置
		if (toFileInfo != fromFileInfo)
		{
			QMessageBox::StandardButton button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Found a same named file in the program folder!\n") + toFileInfo.absolutePath() + QObject::tr("\nYou can choose YES to overwrite it(NOT Recommended)."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
			if (button == QMessageBox::Yes)
			{				
				QFile toFile(to);
				if (!(toFile.remove() && formFile.copy(to)))
				{
					QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed rewrite the file!\n") + toFileInfo.absolutePath());
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else // 同名同位置
		{
		
		}
	}
	else // 不同名
	{
		if(!formFile.copy(to))
		{
			QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed copy to the file!\n") + toFileInfo.absolutePath());
			return false;
		}
	}

	return true;
}