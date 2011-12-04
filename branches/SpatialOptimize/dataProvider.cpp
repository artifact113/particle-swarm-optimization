#include "dataProvider.h"

#include <QByteArray>

const unsigned int nMaxPathSize = 256;

ShapefileProvider::ShapefileProvider() : m_pDataSource( NULL ) , m_pLayer( NULL ) , m_Path( "" ) , m_hasOpened( false )
{
	OGRRegisterAll();
}

ShapefileProvider::~ShapefileProvider()
{
	OGRDataSource::DestroyDataSource( m_pDataSource );
}

bool ShapefileProvider::openFile( const QString& strPath )
{
	closeFile();
	
	QByteArray aryTemp( nMaxPathSize , '0');
	aryTemp = strPath.toUtf8();

	const char *szPath = aryTemp.data();

	m_pDataSource = OGRSFDriverRegistrar::Open( szPath,true );

	if( m_pDataSource == NULL )
	{
		return false;
	}

	m_pLayer = m_pDataSource->GetLayer( 0 );

	if( m_pLayer == NULL)
	{
		OGRDataSource::DestroyDataSource( m_pDataSource );
		m_pDataSource = NULL ;
		
		return false;
	}
	
	m_Path = strPath;
	m_hasOpened = true;

	return true;
}

void ShapefileProvider::closeFile()
{
	OGRDataSource::DestroyDataSource( m_pDataSource );
	m_pDataSource = NULL ;
	m_pLayer = NULL ;
	m_Path = "" ;
	m_hasOpened = false;
}

QString ShapefileProvider::getPath() const
{
	return m_Path;
}

OGRDataSource* ShapefileProvider::getDataSource() const
{
	return m_pDataSource;
}

OGRLayer* ShapefileProvider::getLayer() const
{
	return m_pLayer;
}

bool ShapefileProvider::opened() const
{
	return m_hasOpened;
}
