#ifndef _XMLOPERATOR_H_
#define _XMLOPERATOR_H_

#include <QString>
#include <QDomElement>

class XmlOperator
{
public:
	static QDomDocument XmlRead(const QString& filename);

	static bool XmlWrite(const QDomDocument& document, const QString& filename);
};



#endif // _XMLOPERATOR_H_