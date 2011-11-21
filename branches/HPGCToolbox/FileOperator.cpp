#include "FileOperator.h"
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QMessageBox> 


/// 验证文件
bool FileOperator::VerifyFile(const QString &filename, const PLUGINTYPE &type)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	return true;
}

/// 验证算法包
bool FileOperator::VerifyAlgorithmFile(const QString &filename)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	


	return true;
}


/// 验证指标计算包
bool FileOperator::VerifyIndicatorFile(const QString &filename)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	return true;
}


/// 验证公式包
bool FileOperator::VerifyFormulaFile(const QString &filename)
{
	QFile myfile(filename);

	if (!myfile.exists())
	{
		return false;
	}

	return true;
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