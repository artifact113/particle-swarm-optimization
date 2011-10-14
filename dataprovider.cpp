#include "dataprovider.h"

#include <QByteArray>

ShapeDriver::ShapeDriver() : m_pDataSource( NULL ) , m_pLayer( NULL ) , m_Path( "" ) , HasOpened( false )
{
	OGRRegisterAll();
}

ShapeDriver::~ShapeDriver()
{
	OGRDataSource::DestroyDataSource( m_pDataSource );
}

bool ShapeDriver::OpenFile(QString strPath)
{
	SetPath( strPath );
	if( m_Path == "" )
	{
		m_pDataSource = NULL ;
		m_pLayer = NULL ;

		HasOpened = false;
		
		return false;
	}

	QByteArray aryTemp( 256, '0');
	
	aryTemp = m_Path.toUtf8();

	const char *szPath = aryTemp.data(); 

	m_pDataSource = OGRSFDriverRegistrar::Open( szPath,true );

	if( m_pDataSource == NULL )
	{
		m_pLayer = NULL ;
		m_Path = "";

		HasOpened = false;

		return false;
	}

	m_pLayer = m_pDataSource->GetLayer( 0 );

	if( m_pLayer == NULL)
	{
		m_pDataSource = NULL ;
		m_Path = "" ;
		
		HasOpened = false;

		return false;
	}

	HasOpened = true;

	return true;
}

void ShapeDriver::CloseFile()
{
	OGRDataSource::DestroyDataSource( m_pDataSource );
	m_pDataSource = NULL ;
	m_pLayer = NULL ;
	m_Path = "" ;

	HasOpened = false;
}

QString ShapeDriver::GetPath() const
{
	return m_Path;
}
void ShapeDriver::SetPath(QString strPath)
{
	m_Path = strPath;
}

OGRDataSource* ShapeDriver::GetDataSource() const
{
	return m_pDataSource;
}

OGRLayer* ShapeDriver::GetLayer() const
{
	return m_pLayer;
}

bool ShapeDriver::Opened() const
{
	return HasOpened;
}

/*******************************************/

GdalDriver::GdalDriver() : m_pDataset( NULL ) , m_Path( "" ) , HasOpened( false )
{
	GDALAllRegister();
}

GdalDriver::~GdalDriver()
{
	GDALClose( m_pDataset );
}

bool GdalDriver::OpenFile(QString strPath)
{
	SetPath( strPath );
	if( m_Path == "" )
	{
		m_pDataset = NULL ;
		HasOpened = false;
		
		return false;
	}

	QByteArray aryTemp( 256, '0');
	
	aryTemp = m_Path.toUtf8();

	const char *szPath = aryTemp.data(); 

	m_pDataset = static_cast< GDALDataset * > ( GDALOpen( szPath, GA_ReadOnly ) );

	if( m_pDataset == NULL )
	{
		m_Path = "";
		HasOpened = false;

		return false;
	}

	HasOpened = true;

	return true;
}

void GdalDriver::CloseFile()
{
	GDALClose( m_pDataset );
	m_pDataset = NULL ;
	m_Path = "" ;

	HasOpened = false;
}

QString GdalDriver::GetPath() const
{
	return m_Path;
}
void GdalDriver::SetPath(QString strPath)
{
	m_Path = strPath;
}

GDALDataset  * GdalDriver::GetDateSet() const
{
	return m_pDataset;
}

bool GdalDriver::Opened() const
{
	return HasOpened;
}