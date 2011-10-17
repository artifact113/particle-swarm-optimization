#include "Render.h"

#include "qgssymbol.h"
#include "qgsuniquevaluerenderer.h"
#include "qgsmapcanvas.h" 
#include "qgsmaplayerregistry.h"


#include <QColor>
#include <QList>
#include <QMessageBox>
#include <QVariant>
#include <QTextCodec>

#include "stdlib.h"

#include "ogrsf_frmts.h"


Render::Render( QgisInterface* iface ) : m_iface( iface )
{
}

Render::~Render()
{}

QColor Render::randomColor()
{
  QColor thecolor;

  //insert a random color
  int red = 1 + ( int )( 255.0 * rand() / ( RAND_MAX + 1.0 ) );
  int green = 1 + ( int )( 255.0 * rand() / ( RAND_MAX + 1.0 ) );
  int blue = 1 + ( int )( 255.0 * rand() / ( RAND_MAX + 1.0 ) );
  thecolor.setRgb( red, green, blue );

  return thecolor;
}

QList<QString> Render::returnUValues( const QString& strPath , const QString& strFieldName )
{
	QList<QString> strList;

	OGRRegisterAll();  

	QByteArray aryTemp( 256, '0');
	
	aryTemp = strPath.toUtf8();

	const char *szPath = aryTemp.data(); 

	OGRDataSource  *pDS = OGRSFDriverRegistrar::Open( szPath,false );

	if( pDS == NULL )
	{
		return strList;
	}
	
	OGRLayer  *pLayer = pDS->GetLayer( 0 );

	if( pLayer == NULL )
	{
		OGRDataSource::DestroyDataSource( pDS );
		return strList;
	}
	
	QString strLayerName = 	( QTextCodec::codecForName("UTF-8") )->toUnicode( pLayer->GetName() );

	QString strSelect = QString("SELECT  DISTINCT  \"%1\"  FROM  %2 ").arg( strFieldName )
		                                                              .arg( strLayerName );

	aryTemp = ( QTextCodec::codecForLocale() )->fromUnicode( strSelect );

	OGRLayer  *pSelectLayer = pDS->ExecuteSQL( aryTemp.data()  , NULL , NULL );

	OGRFeature *pFeature;
	
	QTextCodec * pTextCodec = QTextCodec::codecForCStrings();
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));

	pSelectLayer->ResetReading();
	while( ( pFeature = pSelectLayer->GetNextFeature() ) != NULL )
	{
		strList.append( pFeature->GetFieldAsString( 0 ) ); 
		OGRFeature::DestroyFeature( pFeature ); 
	}

	OGRDataSource::DestroyDataSource( pDS );  
	QTextCodec::setCodecForCStrings( pTextCodec );
	return strList;
}

bool Render::check( const QString& str , const QList<QString>& rStrList )
{
	int i;
	int nCount = rStrList.count();

	for( i = 0 ; i != nCount ; ++i)
	{
		if( rStrList.at( i ) ==  str )
		{
			break;
		}
	}

	if( i == nCount )
	{
		return false;
	}
	else
	{
		return true;
	}
}

QString Render::BaseName(const QString& strPath)
{
	if( strPath == "" )
	{
		return QString("");
	}

	const QChar *pChar = strPath.unicode();              //ÓÐ/0

	int i;
	int nIndexDot = -1;
	int nIndexIncline = -1;
	
	int nNum = strPath.size();                                      
	
	for( i = 0 ; i < nNum ; i++ )                       
	{
		if( pChar[i] == '.' )
		{
			nIndexDot = i;
			continue;
		}

		if( pChar[i] == '\\' || pChar[i] == '/' )
		{
			nIndexIncline = i;
		}
	}
	
	if( nIndexDot == -1 || nIndexIncline == -1 )
	{
		return QString("");
	}

	if( nIndexDot < nIndexIncline  )
	{
		return QString("");
	}

	const int nBaseNameNum = nIndexDot - nIndexIncline - 1 ;                        //×Ö·ûÊý

	QChar *pNewChar = new QChar[nBaseNameNum + 1];                    //°üº¬\0

	for( i = 0 ; i != nBaseNameNum ; ++i )
	{
		pNewChar[i] = pChar[ i + nIndexIncline + 1 ];
	}

	pNewChar[nBaseNameNum] = '\0';


	QString strResult( pNewChar );

	delete []pNewChar;

	return strResult;
}

bool Render::uniquevalueRender( QgisInterface* iface , const QString& strPath , const QString& strFieldName , const QList<QString>* pUserUValuesList )
{
	QgsMapLayer* pMapLayer;
	QgsMapCanvas* pMapCanvas = iface->mapCanvas();
	int i;
	int nLayerCount =  pMapCanvas->layerCount();



	for( i = 0 ; i != nLayerCount; ++i )
	{
		pMapLayer =  pMapCanvas->layer( i ) ;
		if( pMapLayer->source() == strPath  )
		{
			break;
		}
	}

	if( i != nLayerCount )
	{
		return uniquevalueRender2( iface , dynamic_cast<QgsVectorLayer*> ( pMapLayer ) , strFieldName ,pUserUValuesList );
	}
	
	QgsVectorLayer* pVectorLayer = new QgsVectorLayer( strPath , BaseName( strPath ) , "ogr"  );

	const QGis::GeometryType geometryType =  pVectorLayer->geometryType();

	if( geometryType == QGis::UnknownGeometry || geometryType == QGis::NoGeometry )
	{
		delete pVectorLayer;
		return false;
	}
	else
	{
		const int nIndex = pVectorLayer->fieldNameIndex( strFieldName );

		if( nIndex == -1 )
		{
			QgsMapLayerRegistry::instance()->addMapLayer( pVectorLayer );
			return false;
		}
		else
		{
			QList<QString>  UValuesList = returnUValues( strPath , strFieldName );

			const int nUValuesCount = UValuesList.count();

			QgsSymbol* pSymbol;
				
			int i;

			QString strLabel;
				
			QgsUniqueValueRenderer *pRenderer = new QgsUniqueValueRenderer( geometryType );

			if( pUserUValuesList == NULL )
			{
				for( i = 0 ; i != nUValuesCount ; ++i )
				{
					strLabel = UValuesList.at( i );
					pSymbol = new QgsSymbol( geometryType, "", "" , strLabel , randomColor() );
					pSymbol->setPen( QPen( QColor( 0 ,0 , 0) ) );
					pRenderer->insertValue( strLabel , pSymbol );
				}
			}
			else
			{
				QString defaultStr( "" );
				QColor defaultColor( 240 , 240 , 240 );
				for( i = 0 ; i != nUValuesCount ; ++i )
				{
					strLabel = UValuesList.at( i );
					if( check( strLabel , *pUserUValuesList ) )
					{
						pSymbol = new QgsSymbol( geometryType, "", "" , strLabel , randomColor() );
						pSymbol->setPen( QPen( QColor( 0 ,0 , 0) ) );
						pRenderer->insertValue( strLabel , pSymbol );
					}
					else
					{
						pSymbol = new QgsSymbol( geometryType, "", "" , defaultStr , defaultColor );
						pSymbol->setPen( QPen( QColor( 0 ,0 , 0) ) );
						pRenderer->insertValue( strLabel , pSymbol );
					}
				}
			}
			
			pRenderer->updateSymbolAttributes();
				
			pRenderer->setClassificationField( nIndex );
			
			pVectorLayer->setRenderer( pRenderer );	
			
			QgsMapLayerRegistry::instance()->addMapLayer( pVectorLayer );
			
			return true;
		}
	}
}

bool Render::uniquevalueRender2( QgisInterface* iface , QgsVectorLayer* pVectorLayer  , const QString& strFieldName , const QList<QString>* pUserUValuesList )
{
	QList<QString>  UValuesList = returnUValues( pVectorLayer->source() , strFieldName );

	const int nUValuesCount = UValuesList.count();

	if( nUValuesCount == 0 )
	{
		return false;
	}
	else
	{
		const QGis::GeometryType geometryType =  pVectorLayer->geometryType();

		QgsSymbol* pSymbol;
				
		int i;

		QString strLabel;
				
		QgsUniqueValueRenderer *pRenderer = new QgsUniqueValueRenderer( geometryType );

		if( pUserUValuesList == NULL )
		{
			for( i = 0 ; i != nUValuesCount ; ++i )
			{
				strLabel = UValuesList.at( i );
				pSymbol = new QgsSymbol( geometryType, "", "" , strLabel , randomColor() );
				
				pSymbol->setPen( QPen( QColor( 0 ,0 , 0) ) );
				pRenderer->insertValue( strLabel , pSymbol );
			}
		}
		else
		{
			QString defaultStr( "" );
			QColor defaultColor( 240 , 240 , 240 );
			for( i = 0 ; i != nUValuesCount ; ++i )
			{
				strLabel = UValuesList.at( i );
				if( check( strLabel , *pUserUValuesList ) )
				{
					pSymbol = new QgsSymbol( geometryType, "", "" , strLabel , randomColor() );
					pSymbol->setPen( QPen( QColor( 0 ,0 , 0) ) );
					pRenderer->insertValue( strLabel , pSymbol );
				}
				else
				{
					pSymbol = new QgsSymbol( geometryType, "", "" , defaultStr , defaultColor );
					pSymbol->setPen( QPen( QColor( 0 ,0 , 0) ) );
					pRenderer->insertValue( strLabel , pSymbol );
				}
			}
		}
		pRenderer->updateSymbolAttributes();
		pRenderer->setClassificationField( pVectorLayer->fieldNameIndex( strFieldName ));
		pVectorLayer->setRenderer( pRenderer );

		iface->refreshLegend( pVectorLayer );
		( iface->mapCanvas() )->refresh();

		
		return true;
	}
}
