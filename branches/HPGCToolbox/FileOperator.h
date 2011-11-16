#ifndef _FILEOPERATOR_H_
#define _FILEOPERATOR_H_

#include <QFile>
#include <QFileInfo>
#include <QString>

class FileOperator
{
public:
	/// 验证算法包
	static bool VerifyAlgorithmFile(const QString &filename);

	/// 验证指标计算包
	static bool VerifyIndicatorFile(const QString &filename);

	/// 验证公式包
	static bool VerifyFormulaFile(const QString &filename);

	/// 复制文件
	static bool CopyFile(const QString &from, const QString &to);
};



#endif // _FILEOPERATOR_H_