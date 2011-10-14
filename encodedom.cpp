#include "encodedom.h"
#include <QDomDocument>
#include <QStringList>
#include <QString>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QDomText>
#include <QFile>
#include <QTextStream>

EncodeDom::EncodeDom()
{
	m_pEncodingDom = new QDomDocument();
}

EncodeDom::EncodeDom( const QString& strPath )
{
	m_pEncodingDom = new QDomDocument();
	
	if( strPath != NULL )
	{
		QFile xmlFile( strPath );
			
		if( xmlFile.open( QIODevice::ReadOnly ))
		{
			if( m_pEncodingDom->setContent( &xmlFile , false ) )
			{
				QDomElement rootElement = m_pEncodingDom->documentElement();
				if( rootElement.tagName() != "Encode" )
				{
					m_pEncodingDom->clear();
				}
			}
			
			xmlFile.close();
		}
	}
}


EncodeDom::~EncodeDom()
{
	delete m_pEncodingDom;
}

void EncodeDom::loadFromFile( const QString& strPath)
{
	QFile xmlFile( strPath );

	if( !xmlFile.open( QIODevice::ReadOnly ) )
	{
		m_pEncodingDom->clear();
		return;
	}

	m_pEncodingDom->clear();

	if( m_pEncodingDom->setContent( &xmlFile , false ) )
	{
		QDomElement rootElement = m_pEncodingDom->documentElement();
		if( rootElement.tagName() != "Encode" )
		{
			m_pEncodingDom->clear();
		}
	}
	
	xmlFile.close();
}

EncodeDom::EncodeType EncodeDom::encodeType()
{
	if( m_pEncodingDom->isNull() )
	{
		return ENONE;
	}

	QDomElement eleEncodeType =  ( m_pEncodingDom->documentElement() ).firstChildElement();

	if( eleEncodeType.attribute( "Type" ) == "UValues")
	{
		return EUValues;
	}
	else
	{
		return ERange;
	}
}

EncodeDom::CodeType EncodeDom::codeType()
{
	if( m_pEncodingDom->isNull() )
	{
		return CNONE;
	}

	QDomElement eleCodeType =  ( ( m_pEncodingDom->documentElement() ).firstChildElement() ).nextSiblingElement();

	if( eleCodeType.attribute( "Type" ) == "Binary")
	{
		return CBinary;
	}
	else
	{
		return CReal;
	}
}

QStringList EncodeDom::codes()
{
	QStringList strlistCodes;
	if( m_pEncodingDom->isNull() )
	{
		return strlistCodes;
	}

	if( codeType() == CBinary )
	{
		strlistCodes.append( "0" );
		strlistCodes.append( "1" );
	}
	else
	{
		QDomElement eleCodeType =  ( ( m_pEncodingDom->documentElement() ).firstChildElement() ).nextSiblingElement();
		QDomElement eleCode = eleCodeType.firstChildElement();
		while( !eleCode.isNull() )
		{
			strlistCodes.append( eleCode.attribute("code") );
			eleCode = eleCode.nextSiblingElement();
		}


	}
	
	return strlistCodes;
}

QString EncodeDom::queryCodeByUValue( const QString& strUValue )
{
	if( strUValue.isNull() )
	{
		return QString("");
	}

	if( m_pEncodingDom->isNull() )
	{
		return QString("");
	}

	if( encodeType() != EUValues )
	{
		return QString("");
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Item" );
	int nUValueCount = listNode.count();

	int i;
	
	for( i = 0 ; i < nUValueCount ; i++ )
	{
		if( ( ( listNode.at( i ) ).toElement() ).attribute( "UValue" ) ==  strUValue  )
		{
			break;
		}
	}

	if( i == nUValueCount )
	{
		return QString( "" );
	}
	else
	{
		return    ( ( listNode.at( i ) ).toElement() ).attribute("Code") ;
	}
}

QStringList EncodeDom::queryUValueByCode( const QString& strCode )
{
	QStringList strlistUValues;
	if( strCode.isEmpty() )
	{
		return strlistUValues;
	}

	if( m_pEncodingDom->isNull() )
	{
		return strlistUValues;
	}

	if( encodeType() != EUValues )
	{
		return strlistUValues;
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Item" );
	int nItemCount = listNode.count();

	int i;
	QDomElement eleTemp;
	
	for( i = 0 ; i < nItemCount ; i++ )
	{
		eleTemp = ( listNode.at( i ) ).toElement();
		if(  eleTemp.attribute( "Code" ) ==  strCode  )
		{
			strlistUValues.append( eleTemp.attribute( "UValue" ) );
		}
	}
	
	return    strlistUValues;
}

QMap<QString ,QString> EncodeDom::returnUValuesAndCodes()
{
	 QMap<QString ,QString> mapData;
	
	if( m_pEncodingDom->isNull() )
	{
		return mapData;
	}
	
	if( encodeType() != EUValues )
	{
		return mapData;
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Item" );
	int nCount = listNode.count();

	int i;
	
	for( i = 0 ; i != nCount ; ++i )
	{
		mapData.insert( ( ( listNode.at( i ) ).toElement() ).attribute( "UValue" )  , ( ( listNode.at( i ) ).toElement() ).attribute( "Code" ) );
	}

	return mapData;
}


QStringList EncodeDom::uniqueValues()
{
	QStringList strlistUValues;
	
	if( m_pEncodingDom->isNull() )
	{
		return strlistUValues;
	}
	
	if( encodeType() != EUValues )
	{
		return strlistUValues;
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Item" );
	int nUValueCount = listNode.count();

	int i;
	
	for( i = 0 ; i < nUValueCount ; i++ )
	{
		strlistUValues.append( ( ( listNode.at( i ) ).toElement() ).attribute( "UValue" ) );
	}

	return strlistUValues;
}

QString EncodeDom::queryCodeByValue( double dbValue )
{
	if( m_pEncodingDom->isNull() )
	{
		return QString( "" );
	}
	
	if( encodeType() != ERange )
	{
		return QString( "" );
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Range" );
	int nRangeCount = listNode.count();

	QDomElement eleTemp;
	double lower , upper;
	int i;

	for( i = 0 ; i < nRangeCount ; i++ )
	{
		eleTemp = ( listNode.at( i ) ).toElement();
		lower = ( eleTemp.attribute( "Lower" ) ).toDouble();
		upper = ( eleTemp.attribute( "Upper" ) ).toDouble();
		if(  dbValue >= lower && dbValue <  upper )
		{
			return ( ( eleTemp.parentNode() ).toElement() ).attribute( "Code" );
		}
	}
	return QString( "" );
}

Range* EncodeDom::queryRangeByValue( double dbValue )
{
	if( m_pEncodingDom->isNull() )
	{
		return NULL;
	}
	
	if( encodeType() != ERange )
	{
		return NULL;
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Range" );
	int nRangeCount = listNode.count();

	QDomElement eleTemp;
	double lower , upper;
	int i;


	for( i = 0 ; i < nRangeCount ; i++ )
	{
		eleTemp = ( listNode.at( i ) ).toElement();
		lower = ( eleTemp.attribute( "Lower" ) ).toDouble();
		upper = ( eleTemp.attribute( "Upper" ) ).toDouble();
		if(  dbValue >= lower && dbValue <  upper )
		{
			return new Range( lower , upper ) ;
		}
	}
	return NULL ;
}

Range* EncodeDom::RangesByCode( const QString&  strCode , int* pRangeCount  )
{
	if( strCode.isEmpty() )
	{
		*pRangeCount = 0;
		return NULL;
	}

	if( m_pEncodingDom->isNull() )
	{
		*pRangeCount = 0;
		return NULL;
	}
	
	if( encodeType() != ERange )
	{
		*pRangeCount = 0;
		return NULL;
	}

	QDomNodeList listNode = m_pEncodingDom->elementsByTagName( "Item" );
	int nItemCount = listNode.count();

	int i;
	
	for( i = 0 ; i < nItemCount ; i++ )
	{
		if(  ( ( listNode.at( i ) ).toElement() ).attribute( "Code" ) ==  strCode  )
		{
			QDomNodeList listChildNode = ( ( listNode.at( i ) ).toElement() ).childNodes();
			int j;
			*pRangeCount = listChildNode.count();

			if( *pRangeCount == 0 )
			{
				return NULL;
			}

			Range *pRange = new Range[ *pRangeCount ];

			QDomElement eleTemp;

			for( j = 0 ; j < *pRangeCount ; j++ )
			{
				eleTemp = ( listChildNode.at( j ) ).toElement();
				pRange[ j ].lower = ( eleTemp.attribute( "Lower" ) ).toDouble();
				pRange[ j ].upper = ( eleTemp.attribute( "Upper" ) ).toDouble();
			}

			return pRange;
		}
	}
	
	*pRangeCount = 0;
	return NULL;
}

void EncodeDom::clear()
{
	m_pEncodingDom->clear();
}

bool EncodeDom::isEmpty()
{
	return m_pEncodingDom->isNull() ;
}
