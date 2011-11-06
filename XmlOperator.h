#ifndef _XMLOPERATOR_H_
#define _XMLOPERATOR_H_

#include <QString>
#include <QDomElement>

class XmlOperator
{
public:
	static QDomElement XmlRead(const QString& filename);

	static bool XmlWrite(const QDomElement& rootElement, const QString& filename);
};



#endif // _XMLOPERATOR_H_