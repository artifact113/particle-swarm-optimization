#include "FormEncode.h"
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QProgressBar>
#include <QInputDialog>
#include <QMap>
#include <QTextStream>


const unsigned int MaxSelectClauseSize = 256;

FormEncode::FormEncode( QWidget *parent ) : QDialog( parent )
{
	initialiseDlg();
}

FormEncode::FormEncode( const QString& strFilePath , QWidget *parent ) : QDialog( parent )
{
	initialiseDlg();
		
	ui.m_pButnOpen->setEnabled( false );
	getFields( strFilePath );
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
	connect( ui.m_pButnConfirm , SIGNAL( clicked() ), this, SLOT( ClickOnButnConfirm() ) );
	connect( ui.m_pButnLoad , SIGNAL( clicked() ), this, SLOT( ClickOnButnLoad() ) );
	connect( ui.m_pButnSave , SIGNAL( clicked() ), this, SLOT( ClickOnButnSave() ) );
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
	if( m_pointList.size() < 2 || ui.m_pTableWidget->rowCount() == 0 || m_pointList.size() - 1 != ui.m_pTableWidget->rowCount() )
	{
		QMessageBox::information(0, tr("警告"), tr("区间编码出错"), QMessageBox::Ok);
		return ;
	}
     
	bool OK;
	double dbLower =   m_pointList.at( m_pointList.size() - 2 );
	double dbUpper =    m_pointList.at( m_pointList.size() - 1 ) ;
	double dbVlaue = QInputDialog::getDouble( this , tr("获取分割点") , tr("分割点取值：") ,dbLower , dbLower , dbUpper  , 4 , &OK);

	if( OK && dbVlaue > dbLower && dbVlaue < dbUpper )
	{
		m_pointList.insert( m_pointList.size() -1 , dbVlaue );
		ui.m_pTableWidget->item( ui.m_pTableWidget->rowCount() - 1 , 0 )->setText( generateRange( dbLower , dbVlaue ) );
		ui.m_pTableWidget->setRowCount( ui.m_pTableWidget->rowCount() + 1 );
		ui.m_pTableWidget->setItem( ui.m_pTableWidget->rowCount() - 1  , 0 , new QTableWidgetItem( generateRange( dbVlaue , dbUpper ) ) );
		ui.m_pTableWidget->setItem( ui.m_pTableWidget->rowCount() - 1  , 1 , new QTableWidgetItem( tr( "" ) ) );
		setTableWidgetPropertyAfterGetRanges();
	}
}

void FormEncode::ClickOnButnDelete()
{
	if( m_pointList.size() < 2 || ui.m_pTableWidget->rowCount() == 0 || m_pointList.size() - 1 != ui.m_pTableWidget->rowCount() )
	{
		QMessageBox::information(0, tr("警告"), tr("区间编码出错"), QMessageBox::Ok);
		return ;
	}

	if( m_pointList.size() == 2  )
	{
		return;
	}

	m_pointList.removeAt( m_pointList.size() - 2 );
	int nListSize = m_pointList.size();

	ui.m_pTableWidget->removeRow( ui.m_pTableWidget->rowCount() - 1 );

	ui.m_pTableWidget->item( ui.m_pTableWidget->rowCount() - 1 , 0 )->setText( generateRange(  m_pointList.at( nListSize - 2 ) , m_pointList.at( nListSize - 1 ) ) );
	ui.m_pTableWidget->item( ui.m_pTableWidget->rowCount() - 1 , 1 )->setText( tr( "" ) );
	setTableWidgetPropertyAfterGetRanges();
}

void FormEncode::ClickOnButnConfirm()
{
	if( !checkOutputFieldName() )
	{
		return;
	}

	if( ui.m_pCBoxEncodeType->currentText() == tr( "唯一值编码" ) )
	{
		writeUValueEncode();
	}
	else if( ui.m_pCBoxEncodeType->currentText() == tr( "区间编码" )  )
	{
		writeRangeEncode();
	}
}

void FormEncode::ClickOnButnSave()
{
	if( ui.m_pCBoxEncodeType->currentText() == tr( "唯一值编码" ) )
	{
		saveUValueEncode();
	}
	else if( ui.m_pCBoxEncodeType->currentText() == tr( "区间编码" ) )
	{
		saveRangeEncode();
	}

}

void FormEncode::ClickOnButnLoad()
{
	if( ui.m_pCBoxEncodeType->currentText() == tr( "唯一值编码" )  )
	{
		loadUValueEncode();
	}
	else if( ui.m_pCBoxEncodeType->currentText() == tr( "区间编码" )  )
	{
		loadRangeEncode();
	}
	else 
	{
		QMessageBox::information(0, tr("警告"), tr("还未开始编码"), QMessageBox::Ok);
		return;
	}
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
		initialiseList();
		setTableWidgetPropertyAfterGetRanges();
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
	
void FormEncode::initialiseList()
{
	m_pointList.clear();
	ui.m_pTableWidget->clear();

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
	
	m_pointList.append( pFeature->GetFieldAsDouble( 0 ) - 1 );
	m_pointList.append( pFeature->GetFieldAsDouble( 1 ) );
	
	m_shapefile.getDataSource()->ReleaseResultSet( pSelectLayer );
	OGRFeature::DestroyFeature( pFeature );

	ui.m_pTableWidget->setRowCount( 1 );
	ui.m_pTableWidget->setColumnCount( 2 );
	ui.m_pTableWidget->setItem( 0 , 0 , new QTableWidgetItem( generateRange( m_pointList.at( 0 ) , m_pointList.at( 1 ) )  ) );
	ui.m_pTableWidget->setItem( 0 , 1 , new QTableWidgetItem( tr( "" )  ) );
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
		QMessageBox::information(0, tr("警告"), tr("指定的字段不是数值型"), QMessageBox::Ok);
		return false;
	}
	
	return true;
}                              


void FormEncode::setTableWidgetPropertyAfterGetRanges()
{
	int nRowCount = ui.m_pTableWidget->rowCount();
	if( nRowCount == 0 || ui.m_pTableWidget->columnCount() !=2 )
	{
		return;
	}

	if( m_pointList.size() < 2 || ui.m_pTableWidget->rowCount() == 0 || m_pointList.size() - 1 != ui.m_pTableWidget->rowCount() )
	{
		return;
	}
	
	QList<QString> strList;

	QString str = tr( "区间( 字段的取值范围[" );
	str.append( QString::number( m_pointList.at( 0 ) + 1  , 'f' ,4 )  );
	str.append( tr( "," ) );
	str.append( QString::number( m_pointList.at( m_pointList.size() - 1 ) , 'f' , 4  )  );
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

bool FormEncode::checkOutputFieldName()
{
	QString strOutputField = ui.m_pLEOutputFieldName->text();
	if( strOutputField.isEmpty() )
	{
		QMessageBox::information(0, tr("警告"), tr("输出字段名不能为空"), QMessageBox::Ok);
		return false;
	}

	QString strFieldName = ui.m_pCBoxFieldName->currentText();
	QString strEncodeType = ui.m_pCBoxEncodeType->currentText();
	if( strFieldName.isEmpty() || strEncodeType.isEmpty() || ui.m_pTableWidget->rowCount() == 0  )
	{
		QMessageBox::information(0, tr("警告"), tr("当前未编码"), QMessageBox::Ok);
		return false;
	}

	int nRowCount = ui.m_pTableWidget->rowCount();
	for( int i = 0 ; i != nRowCount ; ++i )
	{
		if( ui.m_pTableWidget->item( i , 1 )->text().isEmpty() )
		{
			QMessageBox::information(0, tr("警告"), tr("编码值不能为空值"), QMessageBox::Ok);
			return false;
		}
	}

	if( !m_shapefile.opened() )
	{
		QMessageBox::information(0, tr("警告"), tr("文件未打开"), QMessageBox::Ok);
		return false;
	}
	
	QByteArray aryTemp( MaxSelectClauseSize , '0');
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( strOutputField );

	int nFieldIndex = m_shapefile.getLayer()->GetLayerDefn()->GetFieldIndex( aryTemp.data() );

	if( nFieldIndex != -1 )
	{
		QMessageBox::information(0, tr("警告"), tr("结果字段名与文件中已有字段名冲突"), QMessageBox::Ok);
		return false;
	}

	return true;
}

void FormEncode::writeUValueEncode()
{
	OGRLayer* pLayer = m_shapefile.getLayer();

	QByteArray aryTemp( MaxSelectClauseSize , '0');
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pLEOutputFieldName->text() );
	OGRFieldDefn FieldDefn(  aryTemp.data() , OFTString );
	
	if( pLayer->CreateField( &FieldDefn ) != OGRERR_NONE)
	{
		QMessageBox::information(0, tr("警告"), QString( tr("创建字段不成功！") ), QMessageBox::Ok);
		return;
	}
	
	QMap< QString ,QString > map;
	int nRowCount = ui.m_pTableWidget->rowCount();
	for( int i = 0 ; i != nRowCount ; ++i )
	{
		map.insert( ui.m_pTableWidget->item( i , 0 )->text() , ui.m_pTableWidget->item( i , 1 )->text() );
	}

	aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pCBoxFieldName->currentText() );
	int nOldFieldIndex = pLayer->GetLayerDefn()->GetFieldIndex( aryTemp.data() );
	
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pLEOutputFieldName->text() );
	int nNewFieldIndex = pLayer->GetLayerDefn()->GetFieldIndex( aryTemp.data() );

	int nFeatureCount = pLayer->GetFeatureCount();
	
	QProgressBar progressBar(this);
	progressBar.setRange( 0 , nFeatureCount );
	progressBar.setGeometry( ui.line->pos().x() + 10  , ui.line->pos().y() + 10  ,ui.m_pTableWidget->size().width() / 2 , 20 );
	progressBar.show();

	for (int i=0; i != nFeatureCount; ++i)
	{
		OGRFeature* pFeature = pLayer->GetFeature( i );
		
		QString strKey = pFeature->GetFieldAsString( nOldFieldIndex );
		aryTemp = QTextCodec::codecForLocale()->fromUnicode( map.value( strKey  ) );
		
		pFeature->SetField( nNewFieldIndex , aryTemp.data()  );
		pLayer->SetFeature( pFeature ); 
		OGRFeature::DestroyFeature(pFeature);

		progressBar.setValue( i );
	}
	QMessageBox::information(0, tr("成功"), QString( tr("已写入编码数据") ), QMessageBox::Ok);
}

void FormEncode::writeRangeEncode()
{
	if( ( m_pointList.size() < 2 ) || ( m_pointList.size() - 1 != ui.m_pTableWidget->rowCount() )  )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("区间编码出错") ), QMessageBox::Ok);
		return;
	}

	OGRLayer* pLayer = m_shapefile.getLayer();

	QByteArray aryTemp( MaxSelectClauseSize , '0');
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pLEOutputFieldName->text() );
	OGRFieldDefn FieldDefn(  aryTemp.data() , OFTString );
	
	if( pLayer->CreateField( &FieldDefn ) != OGRERR_NONE)
	{
		QMessageBox::information(0, tr("警告"), QString( tr("创建字段不成功！") ), QMessageBox::Ok);
		return;
	}

	aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pCBoxFieldName->currentText() );
	int nOldFieldIndex = pLayer->GetLayerDefn()->GetFieldIndex( aryTemp.data() );
	if( ( pLayer->GetLayerDefn()->GetFieldDefn( nOldFieldIndex )->GetType() != OFTInteger ) && ( pLayer->GetLayerDefn()->GetFieldDefn( nOldFieldIndex )->GetType() != OFTReal )   )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("原字段非数值类型！") ), QMessageBox::Ok);
		return;
	}
	
	aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pLEOutputFieldName->text() );
	int nNewFieldIndex = pLayer->GetLayerDefn()->GetFieldIndex( aryTemp.data() );

	int nFeatureCount = pLayer->GetFeatureCount();
	
	QProgressBar progressBar(this);
	progressBar.setRange( 0 , nFeatureCount );
	progressBar.setGeometry( ui.line->pos().x() + 10  , ui.line->pos().y() + 10  ,ui.m_pTableWidget->size().width() / 2 , 20 );
	progressBar.show();

	for (int i=0; i != nFeatureCount; ++i)
	{
		OGRFeature* pFeature = pLayer->GetFeature( i );
		
		double dbValue = pFeature->GetFieldAsDouble( nOldFieldIndex );
		
		int nRowIndex = 0;
		while( dbValue > m_pointList.at( nRowIndex ) )
		{
			++nRowIndex;
		}
		--nRowIndex;
		
		aryTemp = QTextCodec::codecForLocale()->fromUnicode( ui.m_pTableWidget->item( nRowIndex , 1 )->text() );
		pFeature->SetField( nNewFieldIndex , aryTemp.data()  );
		pLayer->SetFeature( pFeature ); 
		OGRFeature::DestroyFeature(pFeature);

		progressBar.setValue( i );
	}
	QMessageBox::information(0, tr("成功"), QString( tr("已写入编码数据") ), QMessageBox::Ok);
}

void FormEncode::saveUValueEncode()
{
	int nRowCount = ui.m_pTableWidget->rowCount();
	if( ui.m_pCBoxEncodeType->currentText() != tr( "唯一值编码" )  || nRowCount == 0 || ui.m_pTableWidget->columnCount() != 2 )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("保存失败") ), QMessageBox::Ok);
		return ;
	}

	for( int i = 0 ; i != nRowCount  ; ++i )
	{
		if( ui.m_pTableWidget->item( i , 1 )->text().isEmpty() )
		{
			QMessageBox::information(0, tr("警告"), QString( tr("编码值不能为空") ), QMessageBox::Ok);
			return ;
		}
	}
	
	QString strFileName = QFileDialog::getSaveFileName(this, tr("Save As File") , "E:" , tr("Xml (*.xml)"));
	if( strFileName.isEmpty() )
	{
		return;
	}
	
	QFile file( strFileName );
	
	if( !file.open( QIODevice::WriteOnly ) )
	{
		QMessageBox::information(0, tr("失败"), tr("保存编码方案失败!"), QMessageBox::Ok);
		return;
	}

	QDomDocument dom;
	generateUValueEncodeDom( dom );

	QTextStream Stream( &file );

	dom.save( Stream , 8 );
	
	file.flush();
	file.close();
			
	QMessageBox::information(0, tr("成功"), tr("保存编码方案成功!"), QMessageBox::Ok);
}

void FormEncode::saveRangeEncode()
{
	int nRowCount = ui.m_pTableWidget->rowCount();
	if( ( m_pointList.size() < 2 ) || ( m_pointList.size() - 1 != nRowCount ) ||  nRowCount == 0 )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("区间编码出错") ), QMessageBox::Ok);
		return;
	}

	for( int i = 0 ; i != nRowCount  ; ++i )
	{
		if( ui.m_pTableWidget->item( i , 1 )->text().isEmpty() )
		{
			QMessageBox::information(0, tr("警告"), QString( tr("编码值不能为空") ), QMessageBox::Ok);
			return ;
		}
	}
	
	QString strFileName = QFileDialog::getSaveFileName(this, tr("Save As File") , "E:" , tr("Xml (*.xml)"));
	if( strFileName.isEmpty() )
	{
		return;
	}
	
	QFile file( strFileName );
	
	if( !file.open( QIODevice::WriteOnly ) )
	{
		QMessageBox::information(0, tr("失败"), tr("保存编码方案失败!"), QMessageBox::Ok);
		return;
	}

	QDomDocument dom;
	generateRangeEncodeDom( dom );

	QTextStream Stream( &file );

	dom.save( Stream , 8 );
	
	file.flush();
	file.close();
			
	QMessageBox::information(0, tr("成功"), tr("保存编码方案成功!"), QMessageBox::Ok);
}

void FormEncode::generateUValueEncodeDom( QDomDocument& dom )
{
	dom.clear();
	QDomProcessingInstruction DomProcessingInstruction = dom.createProcessingInstruction( "xml" , "version=\"1.0\" " );
	dom.appendChild( DomProcessingInstruction );
	
	QDomElement root = dom.createElement( "UniqueValueEncode" );
	dom.appendChild( root );

	int nRowCount = ui.m_pTableWidget->rowCount();
	for( int i = 0 ; i != nRowCount ; ++i )
	{
		QDomElement eleItem = dom.createElement( "Item" );
		root.appendChild( eleItem );
		eleItem.setAttribute( tr( "uvalue" ) , ui.m_pTableWidget->item( i , 0 )->text() );
		eleItem.setAttribute( tr( "code" ) , ui.m_pTableWidget->item( i , 1 )->text() );
	}
}

void FormEncode::generateRangeEncodeDom( QDomDocument& dom )
{
	dom.clear();
	QDomProcessingInstruction DomProcessingInstruction = dom.createProcessingInstruction( "xml" , "version=\"1.0\" " );
	dom.appendChild( DomProcessingInstruction );
	
	QDomElement root = dom.createElement( "RangeEncode" );
	dom.appendChild( root );

	int nRowCount = ui.m_pTableWidget->rowCount();
	for( int i = 0  ; i != nRowCount ; ++i  )
	{
		QDomElement eleItem = dom.createElement( "Range" );
		root.appendChild( eleItem );
		eleItem.setAttribute( tr( "lower" ) , QString::number( m_pointList.at( i ) )  ) ;
		eleItem.setAttribute( tr( "upper" ) , QString::number( m_pointList.at( i + 1 )  ) );
		eleItem.setAttribute( tr( "code" ) , ui.m_pTableWidget->item( i , 1 )->text() );
	}
}  


void FormEncode::loadUValueEncode()
{
	if( ui.m_pCBoxEncodeType->currentText() != tr( "唯一值编码" ) )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("当前不是唯一值编码") ), QMessageBox::Ok);
		return;
	}

	if( ui.m_pTableWidget->rowCount() == 0 || ui.m_pTableWidget->columnCount() != 2 )
	{
		return;
	}

	QString strPath = QFileDialog::getOpenFileName( this,tr("Open XML"), "E:", tr("Xml (*.xml)"));
	
	if( strPath.isEmpty() )
	{
		return;
	}
	
	QFile xmlFile( strPath );

	if( !xmlFile.open( QIODevice::ReadOnly ))
	{
		QMessageBox::information(0, tr("失败"), tr("打开文件失败!"), QMessageBox::Ok);
		return;
	}

	QDomDocument dom;
	
	if( !dom.setContent( &xmlFile , false ) )
	{
		xmlFile.close();
		QMessageBox::information(0, tr("失败"), tr("加载编码文件失败!"), QMessageBox::Ok);
		return;
	}

	xmlFile.close();

	if( dom.documentElement().tagName() != tr( "UniqueValueEncode" ) )
	{
		QMessageBox::information(0, tr("失败"), tr("加载了不正确的文件"), QMessageBox::Ok);
		return;
	}

	loadUValueEncodeFromDom( dom );
}             

void FormEncode::loadRangeEncode()
{
	if( ui.m_pCBoxEncodeType->currentText() != tr( "区间编码" ) )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("当前不是区间编码") ), QMessageBox::Ok);
		return;
	}

	if(  m_pointList.size() < 2  ||  m_pointList.size() - 1 != ui.m_pTableWidget->rowCount()  || ui.m_pTableWidget->rowCount() == 0  )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("区间编码出错") ), QMessageBox::Ok);
		return;
	}

	QString strPath = QFileDialog::getOpenFileName( this,tr("Open XML"), "E:", tr("Xml (*.xml)"));
	
	if( strPath.isEmpty() )
	{
		return;
	}
	
	QFile xmlFile( strPath );

	if( !xmlFile.open( QIODevice::ReadOnly ))
	{
		QMessageBox::information(0, tr("失败"), tr("打开文件失败!"), QMessageBox::Ok);
		return;
	}

	QDomDocument dom;
	
	if( !dom.setContent( &xmlFile , false ) )
	{
		xmlFile.close();
		QMessageBox::information(0, tr("失败"), tr("加载编码文件失败!"), QMessageBox::Ok);
		return;
	}

	xmlFile.close();

	if( dom.documentElement().tagName() != tr( "RangeEncode" ) )
	{
		QMessageBox::information(0, tr("失败"), tr("加载了不正确的文件"), QMessageBox::Ok);
		return;
	}

	loadRangeEncodeFromDom( dom );
}

void FormEncode::loadUValueEncodeFromDom( const QDomDocument& dom )
{
	QDomNodeList eleList = dom.elementsByTagName( tr( "Item" ) );

	int nRowCount = ui.m_pTableWidget->rowCount();
	int nElementCount = eleList.count();
	
	QTableWidget *pTableWidget = ui.m_pTableWidget;
	for( int i = 0 ; i != nRowCount ; ++i )
	{
		for( int j = 0 ; j != nElementCount ; ++j )
		{
			if( pTableWidget->item( i , 0 )->text() ==  eleList.at( j ).toElement().attribute( tr( "uvalue" ) )  )
			{
				pTableWidget->item( i , 1 )->setText( eleList.at( j ).toElement().attribute( tr( "code" ) ) ); 
			}
		}
	}
	QMessageBox::information(0, tr("成功"), tr("加载编码方案成功"), QMessageBox::Ok);
}


void FormEncode::loadRangeEncodeFromDom( const QDomDocument& dom )
{
	const int nOffSet = ui.m_pTableWidget->rowCount() - 1;

	//在最后一个区间添加分割点
	QDomNodeList eleList = dom.elementsByTagName( tr( "Range" ) );
	const int nEleListSize = eleList.size();
	for( int i = 0 ; i != nEleListSize ; ++i )
	{
		double dbTemp = eleList.at( i ).toElement().attribute( tr( "lower" ) ).toDouble();                         //每个区间的下限
		if( dbTemp > m_pointList.at( nOffSet ) && dbTemp < m_pointList.at( m_pointList.size() - 1 )  )
		{
			int j;
			for( j = nOffSet ; dbTemp > m_pointList.at( j ) ;++j ){}
			m_pointList.insert( j , dbTemp );
		}
	}
	double dbTemp = eleList.at( nEleListSize - 1 ).toElement().attribute( tr( "upper" ) ).toDouble();             //最后一个区间的上限
	if( dbTemp > m_pointList.at( nOffSet ) && dbTemp < m_pointList.at( m_pointList.size() - 1 )  )
	{
		int j;
		for( j = nOffSet ; dbTemp > m_pointList.at( j ) ;++j ){}
		m_pointList.insert( j , dbTemp );
	}

	//构建TableWidget
	const int nPointCount = m_pointList.size();
	const int nRowCount = nPointCount - 1;                                             
	ui.m_pTableWidget->setRowCount( nRowCount );
	ui.m_pTableWidget->setColumnCount( 2 );
	for( int i = nOffSet ; i != nRowCount ; ++i )
	{
		ui.m_pTableWidget->setItem( i , 0 , new QTableWidgetItem( generateRange( m_pointList.at( i ) , m_pointList.at( i + 1 ) )  ) );
		ui.m_pTableWidget->setItem( i , 1 , new QTableWidgetItem( tr( "" )  ) );
	}
	
	//获取文件里的编码值
	for( int i = 0 ; i != nEleListSize ; ++i )                                            
	{
		QDomElement eleTemp = eleList.at( i ).toElement();
		double dbLower = eleTemp.attribute( tr( "lower" ) ).toDouble();
		double dbUpper = eleTemp.attribute( tr( "upper" ) ).toDouble();
		for( int j = nOffSet ; j != nPointCount - 1 ; ++j )
		{
			double dbPointLower =  m_pointList.at( j ) ;
			double dbPointUpper =  m_pointList.at( j + 1  ) ;
			if( dbLower == dbPointLower && dbUpper == dbPointUpper )
			{
				ui.m_pTableWidget->item( j , 1 )->setText( eleTemp.attribute( tr("code") ) );
			}
		}
	}
	setTableWidgetPropertyAfterGetRanges();
	QMessageBox::information(0, tr("成功"), tr("加载编码方案成功"), QMessageBox::Ok);
} 

QString FormEncode::generateRange( const double min , const double max )
{
	QString strRange = tr( "( " );
	strRange.append( QString::number( min , 'f' , 4 )  );
	strRange.append( tr( " , " ) );
	strRange.append( QString::number( max , 'f' , 4)  );
	strRange.append( tr( " ] " ) );
	return strRange;
}