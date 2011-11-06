#include "XmlOperator.h"
#include <QString>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


QDomElement XmlOperator::XmlRead(const QString &filename)
{
	QDomElement rootElement;

	// 打开文件
	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Open xml file failed!"));
        return rootElement;
	}

	// 读取文件
    QString errorStr;
    int errorLine;
    int errorColumn; 
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) 
	{
		file.close();
		QMessageBox::warning(NULL, QObject::tr("Warning"),  QObject::tr("Read xml file failed!"));
		return rootElement;
    }
	file.close();

	rootElement = domDocument.documentElement();
	return rootElement;
}



bool XmlOperator::XmlWrite(const QDomElement &rootElement, const QString &filename)
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
	rootElement.save(out, 4);
	file.close();
	return true;
}