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
	
	QString queryCodeByUValue( const QString& strUValue );          //����Ψһֵ��Ψһֵ���룬���ؿ��ַ���˵����ѯ���ɹ�

	QStringList queryUValueByCode( const QString& strCode );    //stringlist�ڵ���ĿΪ0˵����ѯ���ɹ�

	QMap<QString ,QString> returnUValuesAndCodes();           //�������е�Ψһֵ����Ӧ����ֵ
	
	QStringList uniqueValues();                  //stringlist�ڵ���ĿΪ0���ж�

	QString queryCodeByValue( double dbValue );        //���ؿ��ַ���˵��VALUE�����κ�������

	Range* queryRangeByValue( double dbValue );         //ע��ʹ��DELETE������ֵΪNULL��ʾ���ɹ�

	Range* RangesByCode( const QString&  strCode , int* pRangeCount  );     //����ֵΪNULL��ʾ���ɹ�����ʱ*pRangeCount = 0 ��ע��ʹ��DELETE   delete []pRange2

	void clear();

	bool isEmpty();

	~EncodeDom();

private:
	QDomDocument *m_pEncodingDom;
};

#endif // ENCODEDOM_H
