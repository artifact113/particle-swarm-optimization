#include "XmlOperator.h"
#include <QString>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


/// 读Xml
QDomDocument XmlOperator::XmlRead(const QString &filename)
{
	QDomDocument domDocument;

	// 打开文件
	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
        return domDocument;
	}

	// 读取文件
    QString errorStr;
    int errorLine;
    int errorColumn; 
    if (!domDocument.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) 
	{
		file.close();
		return domDocument;
    }
	file.close();

	return domDocument;
}


/// 写Xml
bool XmlOperator::XmlWrite(const QDomDocument& document, const QString& filename)
{
	// 打开文件
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
        return false;
	}

	// 写入文件
	QTextStream out(&file);
	document.save(out, 2);
	file.close();
	return true;
}


/// 验证Xml
bool XmlOperator::XmlVerify(const QString& filename, const QString& templatefilename)
{
	return true;
}


/// 返回当前id的节点指针
QDomElement* XmlOperator::elementByID(QDomElement &element, const QString &id, const QString &tagName)
{
	if (element.isNull())
	{
		return NULL;
	}

	if (element.hasAttribute("id"))
	{
		if (element.attribute("id") == id)
		{
			return &element;
		}
	}

	QDomNodeList allChildNodes(element.elementsByTagName(tagName));
	for (int i=0; i != allChildNodes.count(); ++i)
	{
		QDomElement* childElement = new QDomElement(allChildNodes.at(i).toElement());
		if (childElement->hasAttribute("id"))
		{
			if (childElement->attribute("id") == id)
			{
				return childElement;
			}
		}
	}

	return NULL;
}