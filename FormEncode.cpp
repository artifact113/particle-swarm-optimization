#include "FormEncode.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QProgressBar>


const unsigned int MaxSelectClauseSize = 256;

FormEncode::FormEncode( QWidget *parent ) : QDialog( parent )
{
	initialiseDlg();
}

FormEncode::FormEncode( const QString& strFilePath , QWidget *parent ) : QDialog( parent )
{
	initialiseDlg();
	
	getFields( strFilePath );
	ui.m_pButnOpen->setEnabled( false );
}


FormEncode::~FormEncode()
{

}

void FormEncode::initialiseDlg()
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));

	ui.setupUi(this);
	ui.m_pLEPath->setReadOnly( true );

	ui.m_pCBoxEncodeType->addItem( tr( "" ) );
	ui.m_pCBoxEncodeType->addItem( tr( "唯一值编码" ) );
	ui.m_pCBoxEncodeType->addItem( tr( "区间编码" ) );
	
	connect( ui.m_pButnOpen , SIGNAL( clicked() ), this, SLOT( ClickOnButnOpen() ) );

	connect( ui.m_pCBoxFieldName , SIGNAL( activated( int ) ), this, SLOT( ActivateCBoxField( int ) ) );
	connect( ui.m_pCBoxEncodeType , SIGNAL( activated( int ) ), this, SLOT( ActivateCBoxEncodeType( int ) ) );
}

void FormEncode::getFields( const QString& strPath )
{
	ui.m_pLEPath->clear();
	ui.m_pCBoxFieldName->clear();
	
	if( !m_shapefile.openFile( strPath ) )
	{
		QMessageBox::information(0, tr("失败"), tr("打开SHAPE文件失败"), QMessageBox::Ok);
		return;
	}
	
	OGRFeatureDefn *pFDefn = m_shapefile.getLayer()->GetLayerDefn();
	
	int nFieldsCount = pFDefn->GetFieldCount();

	QStringList strListName;
	strListName.append( "" );

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

void FormEncode::ActivateCBoxField( int nIndex )
{
	ui.m_pTableWidget->clear();
	ui.m_pCBoxEncodeType->setCurrentIndex( 0 );
}
	
void FormEncode::ActivateCBoxEncodeType( int nIndex )
{
	if( nIndex == 0 )
	{
		ui.m_pTableWidget->clear();
		ui.m_pButnAdd->setEnabled( false );
		ui.m_pButnDelete->setEnabled( false );
	}
	else if( nIndex == 1 )
	{
		ui.m_pTableWidget->clear();
		ui.m_pButnAdd->setEnabled( false );
		ui.m_pButnDelete->setEnabled( false );
		getUValues();
	}
	else
	{
		ui.m_pTableWidget->clear();
		ui.m_pButnAdd->setEnabled( true );
		ui.m_pButnDelete->setEnabled( true );
	}
}

bool FormEncode::isReadyToGetUValues()
{
	if( ui.m_pCBoxFieldName->currentText().isEmpty() )
	{
		QMessageBox::information(0, tr("警告"), tr("未指定字段"), QMessageBox::Ok);
		return false;
	}

	if( ui.m_pCBoxEncodeType->currentText() != tr( "唯一值编码" ) )
	{
		QMessageBox::information(0, tr("警告"), tr("当前不是唯一值编码"), QMessageBox::Ok);
		return false;
	}

	if( !m_shapefile.opened() )
	{
		QMessageBox::information(0, tr("警告"), tr("文件未打开"), QMessageBox::Ok);
		return false;
	}
	return true;
}

void FormEncode::setTableWidgetPropertyAfterGetUValues()
{
	int nRowCount = ui.m_pTableWidget->rowCount();
	if( nRowCount == 0 || ui.m_pTableWidget->columnCount() !=2 )
	{
		return;
	}

	QList<QString> strList;
	strList.append( tr( "唯一值" ) );
	strList.append( tr( "编码值" ) );
	ui.m_pTableWidget->setHorizontalHeaderLabels( strList );
	
	ui.m_pTableWidget->resizeColumnsToContents();
	for( int i = 0 ; i != nRowCount ; ++i )
	{
		ui.m_pTableWidget->item( i , 0 )->setFlags( Qt::NoItemFlags );
	}
}

void FormEncode::getUValues()
{
	ui.m_pTableWidget->clear();

	if( !isReadyToGetUValues() )
	{
		return;
	}

	QString strFieldName = ui.m_pCBoxFieldName->currentText();
	QString strLayerName = 	( QTextCodec::codecForName("UTF-8") )->toUnicode( m_shapefile.getLayer()->GetName() );
	QString strSelect = QString("SELECT  DISTINCT  \"%1\"  FROM  %2 ").arg( strFieldName ).arg( strLayerName );

	QByteArray aryTemp( MaxSelectClauseSize , '0');
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( strSelect );

	OGRLayer  *pSelectLayer = m_shapefile.getDataSource()->ExecuteSQL( aryTemp.data()  , NULL , NULL );
	if( pSelectLayer == NULL )
	{
		QMessageBox::information(0, tr("警告"), tr("获取唯一值失败"), QMessageBox::Ok);
		return ;
	}
	
	QProgressBar progressBar(this);
	progressBar.setRange( 0 , pSelectLayer->GetFeatureCount() );
	progressBar.setGeometry( ui.line->pos().x() + 10  , ui.line->pos().y() + 10  ,ui.m_pTableWidget->size().width() / 2 , 20 );
	progressBar.show();

	int nPosition = 0;
	ui.m_pTableWidget->setRowCount( pSelectLayer->GetFeatureCount() );
	ui.m_pTableWidget->setColumnCount( 2 );

	OGRFeature *pFeature;
	pSelectLayer->ResetReading();
	while( ( pFeature = pSelectLayer->GetNextFeature() ) != NULL )
	{
		ui.m_pTableWidget->setItem( nPosition , 0 , new QTableWidgetItem( pFeature->GetFieldAsString( 0 ) ) );
		ui.m_pTableWidget->setItem( nPosition , 1 , new QTableWidgetItem( "" ) );
		OGRFeature::DestroyFeature( pFeature ); 

		progressBar.setValue( nPosition );
		++nPosition;
	}

	m_shapefile.getDataSource()->ReleaseResultSet( pSelectLayer );

	setTableWidgetPropertyAfterGetUValues();
}
	
