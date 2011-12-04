#include "FormEncode.h"
#include <QFileDialog>
#include <QTextCodec>

FormEncode::FormEncode( QWidget *parent ) : QDialog( parent )
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));

	ui.setupUi(this);
	ui.m_pLEPath->setReadOnly( true );

	ui.m_pCBoxEncodeType->addItem( tr( "唯一值编码" ) );
	ui.m_pCBoxEncodeType->addItem( tr( "区间编码" ) );
	
	connectSignalsAndSlots();
}

FormEncode::FormEncode( const QString& strFilePath , QWidget *parent ) : QDialog( parent )
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));

	ui.setupUi(this);
	
	getFields( strFilePath );

	ui.m_pLEPath->setReadOnly( true );
	ui.m_pButnOpen->setEnabled( false );

	ui.m_pCBoxEncodeType->addItem( tr( "唯一值编码" ) );
	ui.m_pCBoxEncodeType->addItem( tr( "区间编码" ) );

	connectSignalsAndSlots();
}


FormEncode::~FormEncode()
{

}

void FormEncode::connectSignalsAndSlots()
{
	connect( ui.m_pButnOpen , SIGNAL( clicked() ), this, SLOT( ClickOnButnOpen() ) );
}

void FormEncode::getFields( const QString& strPath )
{
	ui.m_pLEPath->clear();
	ui.m_pCBoxFieldName->clear();
	
	if( !m_shapefile.openFile( strPath ) )
	{
		return;
	}
	
	OGRFeatureDefn *pFDefn = m_shapefile.getLayer()->GetLayerDefn();
	
	int nFieldsCount = pFDefn->GetFieldCount();

	QStringList strListName;

	for( int nFieldIndex = 0 ; nFieldIndex !=  nFieldsCount ; ++nFieldIndex )
	{
		OGRFieldDefn *pFieldDefn = pFDefn->GetFieldDefn( nFieldIndex );
        
		strListName.append( pFieldDefn->GetNameRef() );
	}

	ui.m_pLEPath->setText( strPath );
	ui.m_pCBoxFieldName->addItems( strListName );
}

void FormEncode::ClickOnButnOpen()
{
	QString strPath = QFileDialog::getOpenFileName(this,
		tr("Open File"), "E:", tr("Shapefile Files ( *.shp )") );

	if( strPath.isEmpty() )
	{
		return;
	}
	
	getFields( strPath );
}
	
