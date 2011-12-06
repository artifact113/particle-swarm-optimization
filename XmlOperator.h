#ifndef _XMLOPERATOR_H_
#define _XMLOPERATOR_H_

#include <QString>
#include <QDomElement>

class XmlOperator
{
public:
	/// 读Xml
	static QDomDocument XmlRead(const QString& filename);

	/// 写Xml
	static bool XmlWrite(const QDomDocument& document, const QString& filename);

	/// 验证Xml
	static bool XmlVerify(const QString& filename, const QString& templatefilename);

	/// 返回当前id的节点指针
	static QDomElement* elementByID(QDomElement &element, const QString &id, const QString &tagName);
};



#endif // _XMLOPERATOR_H_