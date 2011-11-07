#include "XmlOperator.h"
#include <QString>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


QDomDocument XmlOperator::XmlRead(const QString &filename)
{
	QDomDocument domDocument;

	// 打开文件
	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Open xml file failed!"));
        return domDocument;
	}

	// 读取文件
    QString errorStr;
    int errorLine;
    int errorColumn; 
    if (!domDocument.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) 
	{
		file.close();
		QMessageBox::warning(NULL, QObject::tr("Warning"),  QObject::tr("Read xml file failed!"));
		return domDocument;
    }
	file.close();

	return domDocument;
}



bool XmlOperator::XmlWrite(const QDomDocument& document, const QString& filename)
{
	// 打开文件
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Open xml file failed!"));
        return false;
	}

	// 写入文件
	QTextStream out(&file);


	document.save(out, 4);
	file.close();
	return true;
}