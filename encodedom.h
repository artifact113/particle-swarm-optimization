#ifndef ENCODEDOM_H
#define ENCODEDOM_H

#include <QtCore/qglobal.h>
#include <QMap>
#include <QDomDocument>

class QStringList;
class QString;

struct Range 
{
	Range(){}
	Range( double lower ,double upper ){ this->lower = lower ; this->upper = upper; }
	double lower;
	double upper;
};

class EncodeDom
{
public:
	enum CodeType { CNONE , CBinary , CReal };

	enum EncodeType { ENONE , EUValues , ERange };

	EncodeDom();
	EncodeDom( const QString& strPath );

	void loadFromFile( const QString& strPath);
	
	EncodeType encodeType();

	CodeType codeType();

	QStringList codes();
	
	QString queryCodeByUValue( const QString& strUValue );          //根据唯一值查唯一值编码，返回空字符串说明查询不成功

	QStringList queryUValueByCode( const QString& strCode );    //stringlist内的数目为0说明查询不成功

	QMap<QString ,QString> returnUValuesAndCodes();           //返回所有的唯一值及对应编码值
	
	QStringList uniqueValues();                  //stringlist内的数目为0来判断

	QString queryCodeByValue( double dbValue );        //返回空字符串说明VALUE不在任何区间内

	Range* queryRangeByValue( double dbValue );         //注意使用DELETE，返回值为NULL表示不成功

	Range* RangesByCode( const QString&  strCode , int* pRangeCount  );     //返回值为NULL表示不成功，此时*pRangeCount = 0 ，注意使用DELETE   delete []pRange2

	void clear();

	bool isEmpty();

	~EncodeDom();

private:
	QDomDocument *m_pEncodingDom;
};

#endif // ENCODEDOM_H
