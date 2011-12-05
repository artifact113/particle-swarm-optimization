#include "FormEncode.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QProgressBar>
#include <QInputDialog>


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

	connect( ui.m_pButnAdd , SIGNAL( clicked() ), this, SLOT( ClickOnButnAdd() ) );
	connect( ui.m_pButnDelete , SIGNAL( clicked() ), this, SLOT( ClickOnButnDelete() ) );
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

void FormEncode::ClickOnButnAdd()
{
	if( m_rangeSet.size() < 2 )
	{
		QMessageBox::information(0, tr("警告"), tr("区间编码出错"), QMessageBox::Ok);
		return ;
	}

	set<double>::iterator itFirst = m_rangeSet.begin() ;
	set<double>::reverse_iterator itLast = m_rangeSet.rbegin() ;
     
	bool OK;
	double dbVlaue = QInputDialog::getDouble( this , tr("获取分割点") , tr("分割点取值：") ,  *itFirst + 1 , *itFirst + 1 , *itLast , 4 , &OK);

	if( OK && dbVlaue > (  *itFirst + 1 ) && dbVlaue < *itLast )
	{
		m_rangeSet.insert( dbVlaue );
		showRangesFromSet();
		setTableWidgetPropertyAfterGetGetRanges();
	}
}

void FormEncode::ClickOnButnDelete()
{
	if( m_rangeSet.size() < 2 )
	{
		QMessageBox::information(0, tr("警告"), tr("区间编码出错"), QMessageBox::Ok);
		return ;
	}

	if( m_rangeSet.size() == 2 )
	{
		return;
	}

	if( ui.m_pTableWidget->selectedItems().count() == 0 )
	{
		return ;
	}

	int nRowIndex = ui.m_pTableWidget->selectedItems().at( 0 )->row();

	if( nRowIndex == ui.m_pTableWidget->rowCount() - 1 )
	{
		set<double>::reverse_iterator rit = m_rangeSet.rbegin() ;
		++rit;
		m_rangeSet.erase( *rit );
	}
	else
	{
		set<double>::iterator it = m_rangeSet.begin();
		int i = 0 ;
		while( i != nRowIndex )
		{
			++i;
			++it;
		}
		++it;
		m_rangeSet.erase( *it );
	}
	
	showRangesFromSet();
	setTableWidgetPropertyAfterGetGetRanges();
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
		initialiseSet();
		showRangesFromSet();
		setTableWidgetPropertyAfterGetGetRanges();
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
	
void FormEncode::initialiseSet()
{
	m_rangeSet.clear();

	if( !isReadyToGetRanges() )
	{
		return;
	}

	QString strFieldName = ui.m_pCBoxFieldName->currentText();
	QString strLayerName = 	( QTextCodec::codecForName("UTF-8") )->toUnicode( m_shapefile.getLayer()->GetName() );

	QString strSelect = QString("SELECT  MIN( \"%1\" ) , MAX( \"%1\" )  FROM  %2 ").arg( strFieldName ).arg( strLayerName );

	QByteArray aryTemp( MaxSelectClauseSize , '0');
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( strSelect );

	OGRLayer  *pSelectLayer = m_shapefile.getDataSource()->ExecuteSQL( aryTemp.data()  , NULL , NULL );
	if( pSelectLayer == NULL )
	{
		QMessageBox::information(0, tr("警告"), tr("无法获取字段的取值范围"), QMessageBox::Ok);
		return ;
	}

	if( pSelectLayer->GetFeatureCount() == 0 )
	{
		m_shapefile.getDataSource()->ReleaseResultSet( pSelectLayer );
		QMessageBox::information(0, tr("警告"), tr("获取字段的取值范围失败"), QMessageBox::Ok);
		return ;
	}
	
	OGRFeature *pFeature = pSelectLayer->GetFeature( 0 );
	
	m_rangeSet.insert( pFeature->GetFieldAsDouble( 0 ) - 1 );
	m_rangeSet.insert( pFeature->GetFieldAsDouble( 1 ) );
	
	m_shapefile.getDataSource()->ReleaseResultSet( pSelectLayer );
	OGRFeature::DestroyFeature( pFeature ); 
}

bool FormEncode::isReadyToGetRanges()
{
	if( ui.m_pCBoxFieldName->currentText().isEmpty() )
	{
		QMessageBox::information(0, tr("警告"), tr("未指定字段"), QMessageBox::Ok);
		return false;
	}

	if( ui.m_pCBoxEncodeType->currentText() != tr( "区间编码" ) )
	{
		QMessageBox::information(0, tr("警告"), tr("当前不是区间编码"), QMessageBox::Ok);
		return false;
	}

	if( !m_shapefile.opened() )
	{
		QMessageBox::information(0, tr("警告"), tr("文件未打开"), QMessageBox::Ok);
		return false;
	}

	QString strFieldName = ui.m_pCBoxFieldName->currentText();
	
	QByteArray aryTemp( MaxSelectClauseSize , '0');
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( strFieldName );

	int nFieldIndex = m_shapefile.getLayer()->GetLayerDefn()->GetFieldIndex( aryTemp.data() );
	OGRFieldDefn *pFieldDefn = m_shapefile.getLayer()->GetLayerDefn()->GetFieldDefn( nFieldIndex );

	if( pFieldDefn->GetType() != OFTInteger && pFieldDefn->GetType() != OFTReal )
	{
		QMessageBox::information(0, tr("警告"), tr("指定字段不是数值型的"), QMessageBox::Ok);
		return false;
	}
	
	return true;
}                              

void FormEncode::showRangesFromSet()
{
	ui.m_pTableWidget->clear();
	
	if( m_rangeSet.size() < 2 )
	{
		return;
	}

	int nRowCount =m_rangeSet.size() - 1;

	ui.m_pTableWidget->setRowCount( nRowCount );
	ui.m_pTableWidget->setColumnCount( 2 );
	
	set<double>::iterator it = m_rangeSet.begin() ;
	for ( int i = 0  ; i != nRowCount ; ++i )
	{
		QString strItemTxt = tr( "( " );
		strItemTxt.append( QString::number( *it )  );
		strItemTxt.append( tr( " , " ) );
		it++;
		strItemTxt.append( QString::number( *it )  );
		strItemTxt.append( tr( " ] " ) );
		
		ui.m_pTableWidget->setItem(  i , 0 , new QTableWidgetItem( strItemTxt ) );
		ui.m_pTableWidget->setItem(  i , 1 , new QTableWidgetItem( "" ) );
	}
}

void FormEncode::setTableWidgetPropertyAfterGetGetRanges()
{
	int nRowCount = ui.m_pTableWidget->rowCount();
	if( nRowCount == 0 || ui.m_pTableWidget->columnCount() !=2 )
	{
		return;
	}

	if( m_rangeSet.size() < 2 )
	{
		return;
	}
	
	set<double>::iterator itFirst = m_rangeSet.begin() ;
	set<double>::reverse_iterator itLast = m_rangeSet.rbegin() ;

	QList<QString> strList;

	QString str = tr( "区间( 字段的取值范围[" );
	str.append( QString::number( *itFirst + 1 ) );
	str.append( tr( "," ) );
	str.append( QString::number( *itLast )  );
	str.append( tr( "] )" ) );
	
	strList.append( str );
	strList.append( tr( "编码值" ) );
	ui.m_pTableWidget->setHorizontalHeaderLabels( strList );
	
	ui.m_pTableWidget->resizeColumnsToContents();
	for( int i = 0 ; i != nRowCount ; ++i )
	{
		ui.m_pTableWidget->item( i , 0 )->setFlags( Qt::NoItemFlags );
	}
}