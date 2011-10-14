#include "encodingPso.h"

#include <QMessagebox>
#include <QFileDialog>
#include <QChar>
#include <QTextCodec>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QByteArray>
#include <QModelIndex>
#include <QProgressBar>
#include <QAbstractItemDelegate>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QDomText>
#include <QDomNodeList>
#include <QDomAttr>
#include <QModelIndexList>
#include <QTextStream>

#include <set>
using namespace std;


#include "ComboBoxDelegate.h"
#include "RangeDialog.h"


EncodingPso::EncodingPso(  const QString& strPath , const QList<QString>& strList , QWidget * parent , Qt::WindowFlags f )
                       : QDialog(parent ,f ) ,m_ShapeDriver() , m_GdalDriver() , 
					    m_pTableModel( NULL ) , m_pListModel( NULL ) , m_pRangeListModel( NULL ) , 
						m_CType( CBinary ) , m_EType( ENONE ) , m_DType( DNONE ) ,m_strPathForNewField( "" ) ,
						m_encodeDom()
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	
	setupUi(this);

	pStatisticsEdit->setReadOnly( true );


	m_RangeGroup->setEnabled( false );
	m_pButnVerify->setEnabled( false );
	m_UValuesGroup->setEnabled( false );
	m_pButnSave->setEnabled( false );
	m_pButnClear->setEnabled( false );
	m_pButnAddSchema->setEnabled( false );
	m_pButnPre->setEnabled( false );
	m_pButnWriteNewField->setEnabled( false );

	m_pRButtonUValues->setEnabled( false );
	m_pRButtonRange->setEnabled( false );
	m_pRButtonBinary->setChecked( true );
	activateBinaryRButton( true );

	m_pUniqueValueLView->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_pRangeLView->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_pCodesLView->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_pEncodeTView->setEditTriggers( QAbstractItemView::AllEditTriggers );

	connect(m_pFieldBandCombo, SIGNAL( activated( int ) ), this, SLOT( activatedFieldBandCombo( int ) ) );
	connect(m_pFieldBandCombo2, SIGNAL( activated( int ) ), this, SLOT( activateFieldBandCombo2 ( int ) ) );
	connect(m_pNewFieldCombo, SIGNAL( activated( int ) ), this, SLOT( activateNewFieldCombo( int ) ) );
	
	connect(m_pRButtonBinary, SIGNAL( toggled( bool ) ), this, SLOT( activateBinaryRButton( bool ) ) );
	connect(m_pRButtonReal, SIGNAL( toggled( bool ) ), this, SLOT( activateRealRButton( bool ) ) );
	connect(m_pRButtonUValues, SIGNAL( toggled( bool ) ), this, SLOT( activateUValuesRButton( bool ) ) );
	connect(m_pRButtonRange, SIGNAL( toggled( bool ) ), this, SLOT( activateRangeRButton( bool ) ) );
	
	connect(m_pButnAdd, SIGNAL( clicked() ), this, SLOT( addCodes() ) );
	connect(m_pButnDel, SIGNAL( clicked() ), this, SLOT( delCodes() ) );
	connect(m_pButnAddEncode, SIGNAL( clicked() ), this, SLOT( addEncoding() ) );
	connect(m_pButnDelEncode, SIGNAL( clicked() ), this, SLOT( delEncoding() ) );
	connect(m_pButnVerify, SIGNAL( clicked() ), this, SLOT( verifyEncodingForClick() ) );

	connect(m_pButnNext, SIGNAL( clicked() ), this, SLOT( nextPage() ) );
	connect(m_pButnPre, SIGNAL( clicked() ), this, SLOT( previousPage() ) );
	connect(m_pButnSetRange, SIGNAL( clicked() ), this, SLOT( setRange() ) );
	connect(m_pButnExit, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect(m_pButnReset, SIGNAL( clicked() ), this, SLOT( ReSet() ) );
	connect(m_pButnSave, SIGNAL( clicked() ), this, SLOT( saveEncodeSchema() ) );
	connect(m_pButnAddSchema, SIGNAL( clicked() ), this, SLOT( loadEncodeSchema() ) );
	connect(m_pButnClear, SIGNAL( clicked() ), this, SLOT( clearEncodeSchema() ) );

	connect(m_pButnXMLForNewField, SIGNAL( clicked() ), this, SLOT( loadXMLForNewField() ) );
	connect(m_pButnWriteNewField, SIGNAL( clicked() ), this, SLOT( writeNewField() ) );

	initialDialog( strPath , strList );
}

EncodingPso::~EncodingPso()
{
	delete m_pTableModel;
	delete m_pListModel;
	delete m_pRangeListModel; 
}
/*
bool EncodingPso::initialDialog( const QString& strPath , const QList<QString>& strList )
{
	if( ( getSuffix( strPath ) ).toUpper() == "SHP" )
	{
		m_DType = DNONE;
		
		if( !m_ShapeDriver.OpenFile(strPath) )
		{
			return false ;
		}
		m_DType = Vector;
	}
	else
	{
		m_DType = DNONE;
		
		if( !m_GdalDriver.OpenFile(strPath) )
		{
			return false;
		}
		m_DType = Raster;
	}
	
	if( m_DType == Raster)
	{
		showBands( m_GdalDriver , *m_pFieldBandCombo );
	}
	else if( m_DType == Vector )
	{
		showFieldsInfo( m_ShapeDriver , *m_pFieldBandCombo );
	}
	m_pLineEdit->setText( strPath );

	int i;

	int nCount = strList.count();

	m_pNewFieldCombo->addItem( "" );
	for( i = 0 ;  i != nCount ; ++i )
	{
		m_pNewFieldCombo->addItem( strList.at( i ) );
	}
	
	return true;
}
*/

void EncodingPso::initialDialog( const QString& strPath , const QList<QString>& strList )
{
	if( ( getSuffix( strPath ) ).toUpper() == "SHP" )
	{
		if( m_ShapeDriver.OpenFile(strPath) )
		{
			showFieldsInfo( m_ShapeDriver , *m_pFieldBandCombo );
			m_DType = Vector;
		}
	}
	else
	{
		if( m_GdalDriver.OpenFile(strPath) )
		{
			showBands( m_GdalDriver , *m_pFieldBandCombo );
			m_DType = Raster;
		}

	}

	if( m_DType != DNONE )
	{
		m_pLineEdit->setText( strPath );
		
		int i;
		int nCount = strList.count();
		
		m_pNewFieldCombo->addItem( "" );
		for( i = 0 ;  i != nCount ; ++i )
		{
			m_pNewFieldCombo->addItem( strList.at( i ) );
		}
	}
	return;
}
void EncodingPso::showFieldsInfo(const ShapeDriver &rShapeDriver, QComboBox& rComboBox)
{
	if( ! rShapeDriver.Opened() )
	{
		return;
	}

	rComboBox.clear();
	
	OGRLayer  *pLayer = rShapeDriver.GetLayer();

	OGRFeatureDefn *pFDefn = pLayer->GetLayerDefn();

	OGRFieldDefn *pFieldDefn;

	int nField;

	int nNumFields = pFDefn->GetFieldCount();

	QString strFieldName;

	QStringList strListName;
	
	strListName.append( QString("") );

	for( nField = 0; nField < nNumFields; nField++ )
	{
		pFieldDefn = pFDefn->GetFieldDefn( nField );

		strFieldName = pFieldDefn->GetNameRef();
        
		strListName.append( strFieldName );
	}

	rComboBox.addItems( strListName );
}

void EncodingPso::showBands( const GdalDriver& rGdalDriver , QComboBox& rComboBox )
{
	if( ! rGdalDriver.Opened() )
	{
		return;
	}

	rComboBox.clear();
	
	GDALDataset  *pDataset = rGdalDriver.GetDateSet();
	
	int nBand;

	int nNumBand = pDataset->GetRasterCount();
	
	QStringList strListName;
	
	strListName.append( QString("") );

	for( nBand = 0; nBand < nNumBand; nBand++ )
	{
        
		strListName.append( QString::number( nBand + 1 ) );
	}

	rComboBox.addItems( strListName );
}

void EncodingPso::activatedFieldBandCombo( int nIndex )
{
	delete m_pTableModel;
	m_pTableModel = NULL;

	delete m_pRangeListModel;
	m_pRangeListModel = NULL;

	if( nIndex == 0 )
	{
		m_pRButtonUValues->setEnabled( false );
		m_pRButtonRange->setEnabled( false );
	}
	else
	{
		if( m_DType == Vector )
		{
				OGRLayer  *pLayer = m_ShapeDriver.GetLayer();
				OGRFeatureDefn *pFDefn = pLayer->GetLayerDefn();
				OGRFieldDefn *pFieldDefn = pFDefn->GetFieldDefn( nIndex - 1 );
				OGRFieldType tempFieldType = pFieldDefn->GetType();
				if( tempFieldType == OFTString )
				{
					m_pRButtonUValues->setEnabled( true );
					m_pRButtonRange->setEnabled( false );
					m_pRButtonUValues->setChecked( false );
				}
				else if( tempFieldType == OFTInteger || tempFieldType == OFTReal )
				{
					m_pRButtonUValues->setEnabled( true );
					m_pRButtonRange->setEnabled( true );
					m_pRButtonUValues->setChecked( false );
					m_pRButtonRange->setChecked( false );
				}
				else
				{
					m_pRButtonUValues->setEnabled( false );
					m_pRButtonRange->setEnabled( false );
				}
		}
		else if( m_DType == Raster )
		{
			m_pRButtonUValues->setEnabled( true );
			m_pRButtonRange->setEnabled( true );
			m_pRButtonUValues->setChecked( false );
			m_pRButtonRange->setChecked( false );
		}
	}
}

QStandardItemModel* EncodingPso::setupUValuesTableModelForVector( const ShapeDriver &rShapeDriver, const QString& strFieldName )
{
	QStandardItemModel* pTableModel = NULL;
	
	if( !rShapeDriver.Opened()  )
	{
		return pTableModel;
	}

	OGRDataSource* pDataSource = rShapeDriver.GetDataSource();
	OGRLayer *pLayer = rShapeDriver.GetLayer();

	//QString strLayerName = pLayer->GetName();
	QString strLayerName = 	( QTextCodec::codecForName("UTF-8") )->toUnicode( pLayer->GetName() );

	QString strSelect = QString("SELECT  DISTINCT  \"%1\"  FROM  %2 ").arg( strFieldName )
		                                                              .arg( strLayerName );

	OGRLayer  *pSelectLayer = pDataSource->ExecuteSQL( unicodeToCstring( strSelect ).data()  , NULL , NULL );
	
	if( pSelectLayer == NULL )
	{
		return pTableModel;
	}

	if( pSelectLayer->GetFeatureCount() == 0 )
	{
		return pTableModel;
	}

	OGRFeatureDefn *pFDefn = pSelectLayer->GetLayerDefn();
	
	OGRFieldDefn *pFieldDefn = pFDefn->GetFieldDefn( 0 );

	OGRFeature *pFeature;
	
	QProgressBar ProgressBar(this);
	ProgressBar.setRange( 0 , pSelectLayer->GetFeatureCount() );
	ProgressBar.setGeometry( ( pStackedWidget->pos() ).x() ,( m_pButnReset->pos() ).y() +30 , ( pStackedWidget->geometry() ).width() ,20 );
	ProgressBar.show();

	int i = 0;
	
	QStandardItem *pItem;
	QString strNULL("");
	
	if( pFieldDefn->GetType() == OFTString )
	{
		pTableModel = new QStandardItemModel();
		
		pSelectLayer->ResetReading();
		while( (pFeature = pSelectLayer->GetNextFeature()) != NULL )
		{
			pItem = new QStandardItem( pFeature->GetFieldAsString( 0 ) );
			pTableModel->setItem( i , 0 , pItem );
			
			pItem = new QStandardItem( strNULL );
			pTableModel->setItem( i , 1 , pItem );

			OGRFeature::DestroyFeature( pFeature ); 
			i++;
			ProgressBar.setValue( i );
		}
	}
	else if( pFieldDefn->GetType() == OFTInteger )
	{
		pTableModel = new QStandardItemModel();
		
		pSelectLayer->ResetReading();
		while( (pFeature = pSelectLayer->GetNextFeature()) != NULL )
		{
			pItem = new QStandardItem( QString::number( pFeature->GetFieldAsInteger( 0 ) ) );
			pTableModel->setItem( i , 0 , pItem );
			
			pItem = new QStandardItem( strNULL );
			pTableModel->setItem( i , 1 , pItem );

			OGRFeature::DestroyFeature( pFeature ); 
			i++;
			ProgressBar.setValue( i );
		}
	}
	else if( pFieldDefn->GetType() == OFTReal )
	{
		pTableModel = new QStandardItemModel();
		
		pSelectLayer->ResetReading();
		while( (pFeature = pSelectLayer->GetNextFeature()) != NULL )
		{
			pItem = new QStandardItem( QString::number( pFeature->GetFieldAsDouble( 0 ) , 'g' , 15 ) );
			pTableModel->setItem( i , 0 , pItem );
			
			pItem = new QStandardItem( strNULL );
			pTableModel->setItem( i , 1 , pItem );

			OGRFeature::DestroyFeature( pFeature ); 

			i++;
			ProgressBar.setValue( i );
		}
		
	}
	else
	{
		pDataSource->ReleaseResultSet( pSelectLayer );
		return pTableModel;
	}
	pTableModel->setHeaderData( 0 , Qt::Horizontal ,tr(" 唯一值 ") );
	pTableModel->setHeaderData( 1 , Qt::Horizontal ,tr(" 编码值 ") );
	pDataSource->ReleaseResultSet( pSelectLayer );
	return pTableModel;
}

QStandardItemModel* EncodingPso::setupRangeListModelForVector( const ShapeDriver& rShapeDriver ,const QString& strFieldName )
{
	QStandardItemModel* pListModel = NULL;
	
	if( !rShapeDriver.Opened()  )
	{
		return pListModel;
	}

	OGRDataSource* pDataSource = rShapeDriver.GetDataSource();
	OGRLayer *pLayer = rShapeDriver.GetLayer();

	QString strLayerName = pLayer->GetName();
	
	QString strSelect = QString("SELECT  MIN( \"%1\" ) , MAX( \"%1\" )  FROM  %2 ").arg( strFieldName )
		                                                                           .arg( strLayerName );

	OGRLayer  *pSelectLayer = pDataSource->ExecuteSQL( unicodeToCstring( strSelect ).data()  , NULL , NULL );
	
	if( pSelectLayer == NULL )
	{
		return pListModel;
	}

	if( pSelectLayer->GetFeatureCount() == 0 )
	{
		return pListModel;
	}

	OGRFeatureDefn *pFDefn = pSelectLayer->GetLayerDefn();
	
	OGRFieldDefn *pFieldDefn = pFDefn->GetFieldDefn( 0 );

	OGRFeature *pFeature = pSelectLayer->GetFeature( 0 );
	
	QStandardItem *pItem;
	
	if( pFieldDefn->GetType() == OFTInteger )
	{
		pListModel = new QStandardItemModel();
		
		pItem = new QStandardItem( tr( "最小值    " ) + QString::number( pFeature->GetFieldAsInteger( 0 ) ) );
		pListModel->setItem( 0 , 0 , pItem );

		pItem = new QStandardItem( tr( "最大值    " ) +QString::number( pFeature->GetFieldAsInteger( 1 ) ) );
		pListModel->setItem( 1 , 0 , pItem );

		OGRFeature::DestroyFeature( pFeature ); 

	}
	else if( pFieldDefn->GetType() == OFTReal )
	{
		pListModel = new QStandardItemModel();
					
		pItem = new QStandardItem( tr( "最小值    " ) +QString::number( pFeature->GetFieldAsDouble( 0 ) , 'g' , 15  ) );
		pListModel->setItem( 0 , 0 , pItem );

		pItem = new QStandardItem( tr( "最大值    " ) +QString::number( pFeature->GetFieldAsDouble( 1 ) , 'g' , 15 ) );
		pListModel->setItem( 1 , 0 , pItem );

		OGRFeature::DestroyFeature( pFeature ); 
	}
	else
	{
		pDataSource->ReleaseResultSet( pSelectLayer );
		return pListModel;
	}
	
	pDataSource->ReleaseResultSet( pSelectLayer );
	return pListModel;
}

template<typename T>
void EncodingPso::setupUValuesTableForBand( QStandardItemModel* pTableModel ,GDALRasterBand  *pBand )
{
	if( pBand == NULL)
	{
		return;
	}

	GDALDataType DataType = pBand->GetRasterDataType();

	set<T> setValue;

	int   nXSize = pBand->GetXSize();
	int   nYSize = pBand->GetYSize();
	int i,j,k = 0;                 //k为tablemodel中item的行索引
	
	T *pScanline;
	pScanline = static_cast<T *> ( CPLMalloc( sizeof( T ) * nXSize ) );

	QStandardItem *pItem;
	QString strNULL("");
	
	QProgressBar ProgressBar(this);
	ProgressBar.setRange( 0 , nYSize * nYSize);
	ProgressBar.setGeometry( ( pStackedWidget->pos() ).x() ,( m_pButnReset->pos() ).y() +30 , ( pStackedWidget->geometry() ).width() ,20 );
	ProgressBar.show();

	for( j = 0 ; j < nYSize ; j++ )
	{
		pBand->RasterIO( GF_Read, 0, j, nXSize, 1 , pScanline, nXSize, 1 , DataType , 0, 0 );
		for( i = 0 ; i < nXSize ; i++ )
		{
			if( ( setValue.find( pScanline[i] ) ==  setValue.end () ) )
			{

				setValue.insert( pScanline[i] );
				pItem = new QStandardItem( QString::number( pScanline[i] ) );
				pTableModel->setItem( k , 0 , pItem );
				
				pItem = new QStandardItem( strNULL );
				pTableModel->setItem( k , 1 , pItem );
				k++;
			}
			ProgressBar.setValue( j*nXSize + i );
		}
	}
	
	CPLFree( pScanline ); 
	return;
}

QStandardItemModel* EncodingPso::setupUValuesTableModelForRaster( const GdalDriver& rGdalDriver ,int nIndex )
{
	QStandardItemModel* pTableModel = NULL;
	
	if( !rGdalDriver.Opened()  )
	{
		return pTableModel;
	}

	GDALDataset  *pDataset  = rGdalDriver.GetDateSet();

	if( ( nIndex > pDataset->GetRasterCount() ) || ( nIndex < 1 ) )
	{
		return pTableModel;
	}

	GDALRasterBand  *pBand = pDataset->GetRasterBand( nIndex );

	pTableModel = new QStandardItemModel();
	
	switch( pBand->GetRasterDataType() )
	{
	case GDT_Byte :
		setupUValuesTableModelFor8Bits( pTableModel , pBand );
		break;
	case GDT_UInt16 :
		setupUValuesTableModelFor16UInt( pTableModel , pBand );
		break;
	case GDT_Int16 :
		setupUValuesTableModelFor16Int( pTableModel , pBand );
		break;
	case GDT_Int32 :
		setupUValuesTableForBand<int>( pTableModel , pBand );
		break;
	case GDT_UInt32 : 
		setupUValuesTableForBand<unsigned int>( pTableModel , pBand );
		break;
	case GDT_Float32 :
		setupUValuesTableForBand<float>( pTableModel , pBand );
		break;
	case GDT_Float64 :
		setupUValuesTableForBand<double>( pTableModel , pBand );
		break;
	default:
		return NULL;
	}
	
	pTableModel->setHeaderData( 0 , Qt::Horizontal ,tr(" 唯一值 ") );
	pTableModel->setHeaderData( 1 , Qt::Horizontal ,tr(" 编码值 ") );
	return pTableModel;
}

void  EncodingPso::setupUValuesTableModelFor8Bits( QStandardItemModel* pTableModel  ,  GDALRasterBand  *pBand )
{
	if( pBand == NULL)
	{
		return;
	}
	
	int anHistogram[256];
	
	pBand->GetHistogram( -0.5, 255.5 ,256 , anHistogram, FALSE, FALSE, NULL, NULL );

	int i , k = 0;
	QStandardItem *pItem;
	QString strNULL("");

	for( i = 0 ; i < 256 ; i++ )
	{
		if( anHistogram[i] > 0 )
		{
			pItem = new QStandardItem(QString::number( i ) );
			pTableModel->setItem( k , 0 , pItem );
			
			pItem = new QStandardItem( strNULL );
			pTableModel->setItem( k , 1 , pItem );
			
			k++;
		}
	}
	return;
}

void  EncodingPso::setupUValuesTableModelFor16UInt( QStandardItemModel* pTableModel  ,  GDALRasterBand  *pBand )
{
	if( pBand == NULL)
	{
		return;
	}
	
	int* anHistogram;
	try
	{
		anHistogram = new int[65536];
	}

	catch( bad_alloc )
	{
		setupUValuesTableForBand<short>( pTableModel , pBand );
		return;
	}
	
	pBand->GetHistogram( -0.5, 65535.5 ,65536 , anHistogram, FALSE, FALSE, NULL, NULL );

	int i , k = 0;
	QStandardItem *pItem;
	QString strNULL("");

	for( i = 0 ; i < 65536 ; i++ )
	{
		if( anHistogram[i] > 0 )
		{
			pItem = new QStandardItem(QString::number( i ) );
			pTableModel->setItem( k , 0 , pItem );
			
			pItem = new QStandardItem( strNULL );
			pTableModel->setItem( k , 1 , pItem );
			
			k++;
		}
	}
	
	return;
}


void  EncodingPso::setupUValuesTableModelFor16Int( QStandardItemModel* pTableModel  ,  GDALRasterBand  *pBand )
{
	if( pBand == NULL)
	{
		return;
	}

	int* anHistogram;
	try
	{
		anHistogram = new int[65536];
	}

	catch( bad_alloc )
	{
		setupUValuesTableForBand<short>( pTableModel , pBand );
		return;
	}
	
	pBand->GetHistogram( -32768.5,32767.5 ,65536 , anHistogram, FALSE, FALSE, NULL, NULL );
	
	int i , k = 0;
	QStandardItem *pItem;
	QString strNULL("");
	
	for( i = 0 ; i < 65536 ; i++ )
	{
		if( anHistogram[i] > 0 )
		{
			pItem = new QStandardItem(QString::number( i - 32768 ) );
			pTableModel->setItem( k , 0 , pItem );
			
			pItem = new QStandardItem( strNULL );
			pTableModel->setItem( k , 1 , pItem );
			
			k++;
		}
	}
	
	return;
}

QStandardItemModel* EncodingPso::setupRangeListModelForRaster( const GdalDriver& rGdalDriver ,int nIndex )
{
	QStandardItemModel* pListModel = NULL;
	
	if( !rGdalDriver.Opened()  )
	{
		return pListModel;
	}

	GDALDataset  *pDataset  = rGdalDriver.GetDateSet();

	if( ( nIndex > pDataset->GetRasterCount() ) || ( nIndex < 1 ) )
	{
		return pListModel;
	}

	GDALRasterBand  *pBand = pDataset->GetRasterBand( nIndex );

	double fMin , fMax , fMean , fStdDev;
	
	pBand->GetStatistics( false , true , &fMin , &fMax , &fMean , &fStdDev);
	pListModel = new QStandardItemModel();

	QStandardItem *pItem;
	
	pItem = new QStandardItem( tr( "最小值    " ) + QString::number( fMin , 'g' , 15  ) );
	pListModel->setItem( 0 , 0 , pItem );
	
	pItem = new QStandardItem( tr( "最大值    " ) +QString::number( fMax , 'g' , 15 ) );
	pListModel->setItem( 1 , 0 , pItem );

	return pListModel;
/*	
	double fMin , fMax;
	fMin = pBand->GetMinimum( NULL );
	fMax = pBand->GetMaximum( NULL );

	pListModel = new QStandardItemModel();

	QStandardItem *pItem;
	
	pItem = new QStandardItem( tr( "最小值    " ) + QString::number( fMin , 'g' , 15  ) );
	pListModel->setItem( 0 , 0 , pItem );
	
	pItem = new QStandardItem( tr( "最大值    " ) +QString::number( fMax , 'g' , 15 ) );
	pListModel->setItem( 1 , 0 , pItem );

	return pListModel;
*/
}

void EncodingPso::activateUValuesRButton(bool checked)
{
	if( checked == true )
	{
		delete m_pRangeListModel;
		m_pRangeListModel = NULL;
		
		m_pRButtonRange->setChecked( false );                //可能引起下面一个事件处理程序，且下面的事件处理先执行(false 部分)

		m_EType = EUValues;                                   //一定要在setChecked( false )后面
		
		QString strItemText;
		
		strItemText = m_pFieldBandCombo->currentText();
		
		if( strItemText.isEmpty() || strItemText.isNull() )
		{
			return;
		}
		
		delete m_pTableModel;
		m_pTableModel = NULL;
		
		if( m_DType == Raster)
		{
			m_pTableModel = setupUValuesTableModelForRaster( m_GdalDriver ,strItemText.toInt() );
		}
		else if( m_DType == Vector )
		{
			m_pTableModel = setupUValuesTableModelForVector( m_ShapeDriver ,strItemText );
		}
		
		m_pUniqueValueLView->setModel( m_pTableModel );
	}
	else
	{
		delete m_pTableModel;
		m_pTableModel = NULL;
		m_EType = ENONE;
	}
}

void EncodingPso::activateRangeRButton(bool checked)
{
	if( checked == true )
	{
		delete m_pTableModel;
		m_pTableModel = NULL;
		
		m_pRButtonUValues->setChecked( false );            //可能引起上面一个事件处理程序，且上面的事件处理先执行(false 部分)

		m_EType = ERange;        //一定要在setChecked( false )后面
		
		QString strItemText;
		
		strItemText = m_pFieldBandCombo->currentText();
		
		if( strItemText.isEmpty() || strItemText.isNull() )
		{
			return;
		}
		
		delete m_pRangeListModel;
		m_pRangeListModel = NULL;
		
		if( m_DType == Raster)
		{
			m_pRangeListModel = setupRangeListModelForRaster( m_GdalDriver ,strItemText.toInt() );
		}
		else if( m_DType == Vector )
		{
			m_pRangeListModel = setupRangeListModelForVector( m_ShapeDriver ,strItemText );
		}
		m_pTableModel = new QStandardItemModel();

		m_pTableModel->setColumnCount( 3 );

		m_pTableModel->setHeaderData( 0 , Qt::Horizontal ,tr(" 编码值 ") );
		m_pTableModel->setHeaderData( 1 , Qt::Horizontal ,tr(" 闭区间下限 ") );
		m_pTableModel->setHeaderData( 2 , Qt::Horizontal ,tr(" 开区间上限 ") );
		
		connect(m_pTableModel, SIGNAL( itemChanged( QStandardItem* ) ), this, SLOT( verifyRangeBoundary( QStandardItem* ) ) );
		
		m_pRangeLView->setModel( m_pRangeListModel );
	}
	else
	{
		delete m_pRangeListModel;
		m_pRangeListModel = NULL;
		delete m_pTableModel;
		m_pTableModel = NULL;
		m_EType = ENONE;
	}
}

void EncodingPso::activateBinaryRButton( bool checked )
{
	if( checked )
	{
		QStandardItem *pItem;
		
		m_CType = CBinary;

		m_pButnAdd->setEnabled( false );
		m_pButnDel->setEnabled( false );
		
		delete m_pListModel;
		m_pListModel = new QStandardItemModel();
		
		pItem = new QStandardItem( QString("0") );
		m_pListModel->setItem( 0 , pItem );
		
		pItem = new QStandardItem( QString("1") );
		m_pListModel->setItem( 1 , pItem );
		m_pCodesLView->setModel( m_pListModel );

		QString strNULL( "" );
		
		m_pCodesLView->setEditTriggers( QAbstractItemView::NoEditTriggers );

		if( m_EType == EUValues)
		{
			setStringToModelColumn( m_pTableModel , 1 , strNULL );
		}
		else if( m_EType == ERange )
		{
			m_pTableModel->removeRows( 0 , m_pTableModel->rowCount() );
		}
	}
}

void EncodingPso::activateRealRButton( bool checked )
{
	if( checked )
	{
		m_CType = CReal;

		QString strNULL( "" );
		
		m_pButnAdd->setEnabled( true );
		m_pButnDel->setEnabled( true );
		
		delete m_pListModel;
		m_pListModel = new QStandardItemModel();
		
		connect(m_pListModel, SIGNAL( itemChanged( QStandardItem* ) ), this, SLOT( verifyRealCodes( QStandardItem * ) ) );
		
		m_pCodesLView->setModel( m_pListModel );
		m_pCodesLView->setEditTriggers( QAbstractItemView::DoubleClicked );

		if( m_EType == EUValues)
		{
			setStringToModelColumn( m_pTableModel , 1 , strNULL );
		}
		else if( m_EType == ERange )
		{
			m_pTableModel->removeRows( 0 , m_pTableModel->rowCount() );
		}
	}
}

void EncodingPso::addCodes()
{
	QStandardItem *pItem;
	
	pItem = new QStandardItem( QString("#") );
	m_pListModel->setItem( m_pListModel->rowCount() , pItem );

}

void EncodingPso::delCodes()                                                  //删除一个ITEM不产生ITEMCHANGE信号
{
	QModelIndex ModelIndex = m_pCodesLView->currentIndex();
	m_pListModel->removeRow( ModelIndex.row() );
	
	if( m_pTableModel != NULL )                                               //下一步后又回过来带改编码值
	{
		if( m_pTableModel->rowCount() > 0)
		{
			if( m_EType == EUValues)
			{
				setStringToModelColumn( m_pTableModel , 1 , QString( "" ) );
			}
			else if( m_EType == ERange )
			{
				setStringToModelColumn( m_pTableModel , 0 , QString( "" ) );
			}
		}
	}
}

void EncodingPso::addEncoding()
{
	QStandardItem *pItem;
	int nRowCount = m_pTableModel->rowCount();
	
	pItem = new QStandardItem( QString("") );
	m_pTableModel->setItem( nRowCount , 0 , pItem );

	pItem = new QStandardItem( QString("0") );
	m_pTableModel->setItem( nRowCount , 1 , pItem );

	pItem = new QStandardItem( QString("0") );
	m_pTableModel->setItem( nRowCount , 2 , pItem );
}

void EncodingPso::delEncoding()
{
	QModelIndexList listModelIndex = ( m_pEncodeTView->selectionModel() )->selectedRows();
	
	int nCount = listModelIndex.count();
	int i;
	
	for( i = 0 ; i < nCount ; i++ )
	{
		m_pTableModel->removeRow( ( listModelIndex.at( 0 ) ).row() );
	}
}

void EncodingPso::nextPage()
{
	if( m_CType == CReal )
	{
		int i , nRowCount = m_pListModel->rowCount();
		for( i = 0 ; i < nRowCount  ;  )
		{
			if( ( m_pListModel->item( i ) )->text() == "#" )
			{
				m_pListModel->removeRow( i );
				nRowCount--;
			}
			else
			{
				i++ ;
			}
			
		}
	}

	m_pEncodeTView->setModel( m_pTableModel );
	m_pEncodeTView->resizeColumnsToContents();

	QAbstractItemDelegate *pComboBoxDelegate;
	
	QString  strStatistics;
	
	if(	m_pTableModel != NULL )
	{
		if( m_pRangeListModel != NULL )
		{
			m_RangeGroup->setEnabled( true );
			m_UValuesGroup->setEnabled( false );
				
			pComboBoxDelegate = new ComboBoxDelegateRange( m_pListModel , m_pTableModel );
			delete m_pEncodeTView->itemDelegate();
			m_pEncodeTView->setItemDelegate( pComboBoxDelegate );

			m_pEncodeTView->setSelectionBehavior( QAbstractItemView::SelectRows );
			
			strStatistics.append( ( m_pRangeListModel->item( 0 ) )->text() + "\n" + ( m_pRangeListModel->item( 1 ) )->text() + "\n");
			strStatistics.append( "类型数目  " +  QString::number( m_pListModel->rowCount() ) );
			pStatisticsEdit->setText( strStatistics );
		}
		else
		{
			m_RangeGroup->setEnabled( false );
			m_UValuesGroup->setEnabled( true );

			pComboBoxDelegate = new ComboBoxDelegateUValues( m_pListModel , m_pTableModel );
			delete m_pEncodeTView->itemDelegate();
			m_pEncodeTView->setItemDelegate( pComboBoxDelegate );

			strStatistics.append( "唯一值数目  " +  QString::number( m_pTableModel->rowCount() ) + "\n");
			strStatistics.append( "类型数目    " +  QString::number( m_pListModel->rowCount() ) );
			pStatisticsEdit->setText( strStatistics );
		}
		
		m_pButnSave->setEnabled( true );
		m_pButnClear->setEnabled( true );
		m_pButnAddSchema->setEnabled( true );
		m_pButnVerify->setEnabled( true );
	}
	else
	{
		pStatisticsEdit->clear();
		m_pButnSave->setEnabled( false );
		m_pButnClear->setEnabled( false );
		m_UValuesGroup->setEnabled( false );
		m_pButnAddSchema->setEnabled( false );
		m_pButnVerify->setEnabled ( false );
	}
	
	pStackedWidget->setCurrentIndex( pStackedWidget->currentIndex() + 1 );
	m_pButnNext->setEnabled( false );
    m_pButnPre->setEnabled( true );

	m_pButnWriteNewField->setEnabled( false );

	m_strPathForNewField = "";

	m_encodeDom.clear();

	int i , nCount = m_pFieldBandCombo->count();
	for( i = 0 ; i != nCount ; ++i )
	{
		m_pFieldBandCombo2->addItem( m_pFieldBandCombo->itemText( i )  );
	}
}

void EncodingPso::previousPage()
{
	pStackedWidget->setCurrentIndex( pStackedWidget->currentIndex() - 1 );
	m_pButnPre->setEnabled( false );
	m_pButnNext->setEnabled( true );
}

void EncodingPso::verifyRealCodes(QStandardItem *pStandardItem)
{
	if( m_pTableModel != NULL )                                               //下一步后又回过来带改编码值
	{
		if( m_pTableModel->rowCount() > 0)
		{
			if( m_EType == EUValues)
			{
				setStringToModelColumn( m_pTableModel , 1 , QString( "" ) );
			}
			else if( m_EType == ERange )
			{
				setStringToModelColumn( m_pTableModel , 0 , QString( "" ) );
			}
		}
	}

	int nRowIndex = ( pStandardItem->index() ).row();
	QString str = pStandardItem->text();

	if( str == "#")
	{
		return ;
	}

	if( str.isEmpty() )
	{
		QMessageBox::information(0, tr("警告"), tr("不能输入空值!"), QMessageBox::Ok);
		m_pListModel->removeRow( nRowIndex  );
		return;
	}

	bool OK;
	str.toDouble( &OK );
	
	if( !OK )
	{
		QMessageBox::information(0, tr("警告"), tr("请输入实值!"), QMessageBox::Ok);
		m_pListModel->removeRow( nRowIndex  );
		return;
	}

	int nCount = m_pListModel->rowCount();
	int i;
	
	for( i = 0 ; i < nCount ; i++ )
	{
		if( i == nRowIndex )
		{
			continue;
		}

		if( ( m_pListModel->item( i ) )->text() == str )
		{
			QMessageBox::information(0, tr("警告"), tr("编码值重复!"), QMessageBox::Ok);
			m_pListModel->removeRow( nRowIndex  );
			nCount--;                                     //上面删除了一个，不减1就内存访问出错
		} 
	}
	
	return;
}

void EncodingPso::verifyRangeBoundary( QStandardItem* pStandardItem)
{
	if( m_EType == ERange )
	{
		int nColumnIndex =   pStandardItem->column();
		if( nColumnIndex == 1 || nColumnIndex == 2 )
		{
			QString str = pStandardItem->text();
			if( str.isEmpty() )
			{
				QMessageBox::information(0, tr("警告"), tr("不能输入空值!"), QMessageBox::Ok);
				pStandardItem->setText("0");
				return;
			}
			
			bool OK;
			str.toDouble( &OK );
			if( !OK )
			{
				QMessageBox::information(0, tr("警告"), tr("请输入实值!"), QMessageBox::Ok);
				pStandardItem->setText("0");
				return;
			}
		}
	}
}

void EncodingPso::addStringToList( const QString& str1 , QList<double>& list1 , const QString& str2 , QList<double>& list2 )
{
	double dbTemp1 = str1.toDouble();
	double dbTemp2 = str2.toDouble();

	int nCount = list1.count();
	if( nCount == 0 )
	{
		list1.append( dbTemp1 );
		list2.append( dbTemp2 );
		return;
	}

	int i;
	for( i = 0 ; i <  nCount ; i++ )
	{
		if( dbTemp1  <  list1.at( i )     )
		{
			list1.insert( i , dbTemp1);
			list2.insert( i , dbTemp2 );
			return;
		}
	}
	list1.append( dbTemp1 );
	list2.append( dbTemp2 );
	return;
}

void EncodingPso::verifyEncodingForClick()
{
	verifyEncoding( true );
}

bool EncodingPso::verifyEncoding( bool infoOK)
{
	if( m_pTableModel == NULL )
	{
		return false;
	}
	
	int nRowCount = m_pTableModel->rowCount();

	if( nRowCount == 0 )
	{
		return false;
	}

	int i;

	QList<double> dblistLower , dblistUpper;

	QString strMin;
	QString strMax;

	if( m_EType == ERange )
	{
		for( i = 0 ; i < nRowCount ; i++ )
		{
			if( ( m_pTableModel->item( i , 0 ) )->text() == "" )
			{
				QMessageBox::information( 0, tr("警告"), QString( tr("第%1个编码值不能为空") ).arg( i + 1 ), QMessageBox::Ok);
				return false;
			}

			if( ( ( m_pTableModel->item( i , 1 ) )->text() ).toDouble() >= ( ( m_pTableModel->item( i , 2 ) )->text() ).toDouble() )
			{
				QMessageBox::information(0, tr("警告"), QString( tr("第%1个区间不正确") ).arg( i + 1 ), QMessageBox::Ok);
				return false;
			}
		}
		
		for( i = 0 ; i < nRowCount ; i++ )
		{
			addStringToList( ( m_pTableModel->item( i , 1 ) )->text() , dblistLower ,  ( m_pTableModel->item( i , 2 ) )->text() , dblistUpper );
		}

		strMin = ( ( m_pRangeListModel->item( 0 ) )->text() ).section( "    " , -1 , -1 );
		strMax = ( ( m_pRangeListModel->item( 1 ) )->text() ).section( "    " , -1 , -1 );
		
		if( strMin.toDouble() < dblistLower.at( 0 ) )
		{
			QMessageBox::information(0, tr("警告"), QString( tr("没有包含最小值!") ), QMessageBox::Ok);
			return false;
		}

		if( strMax.toDouble()  >= dblistUpper.at( nRowCount - 1 ) )
		{
			QMessageBox::information(0, tr("警告"), QString( tr("没有包含最大值或区间重叠!") ), QMessageBox::Ok);
			return false;
		}

		for( i = 0 ; i < nRowCount - 1 ; i++ )
		{
			if( dblistUpper.at( i ) != dblistLower.at( i + 1 ) )
			{
				QMessageBox::information(0, tr("警告"), QString( tr("区间不连续或区间重叠!") ), QMessageBox::Ok);
				return false;
			}
		}
		
		if( infoOK )
		{
			QMessageBox::information(0, tr("成功"), QString( tr("编码正确") ), QMessageBox::Ok);
		}

		return true;
	}
	else if( m_EType == EUValues )
	{
		for( i = 0 ; i < nRowCount ; i++ )
		{
			if( ( m_pTableModel->item( i , 1 ) )->text() == "" )
			{
				QMessageBox::information(0, tr("警告"), QString( tr("第%1个唯一值的编码不能为空") ).arg( i + 1 ), QMessageBox::Ok);
				return false;
			}
		}
		
		if( infoOK )
		{
			QMessageBox::information(0, tr("成功"), QString( tr("编码正确") ), QMessageBox::Ok);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void EncodingPso::setRange()
{
	RangeDialog *pDialogRange = new RangeDialog( m_pListModel , m_pTableModel );
	
	if( ( pDialogRange->exec() ) == QDialog::Accepted)
	{
		if( pDialogRange->verifyRange() )
		{
			patchEncoding( *m_pTableModel ,pDialogRange->RangeLower , pDialogRange->RangeUpper , ( pDialogRange->pComboBox )->currentText() );
		}

		if(  ( pDialogRange->pCheckBox ) ->isChecked() )
		{
			int i;
			int nRowCount = m_pTableModel->rowCount();
			QString str = ( pDialogRange->pComboBox2 )->currentText();
			QStandardItem *pItem;

			for( i = 0 ; i < nRowCount ; i++ )
			{
				pItem = m_pTableModel->item( i , 1 );
				if( pItem->text() == "" )
				{
					pItem->setText( str );
				}
			}
		}
	}
	delete pDialogRange;
}

void EncodingPso::patchEncoding(QStandardItemModel &rTableModel, const double RangeLower, const double RangeUpper, const QString &str )
{
	int nCountItem = rTableModel.rowCount();
	int i;
	QString strTemp;
	double dbTemp;
	bool OK;
	for( i = 0 ; i < nCountItem ; i++ )
	{
		strTemp = ( rTableModel.item( i , 0 ) )->text();
		dbTemp = strTemp.toDouble( & OK );
		if( OK && ( RangeLower <= dbTemp ) && ( dbTemp <= RangeUpper ) )
		{
			( rTableModel.item( i , 1 ) )->setText( str ) ;
		}
	}
}

void EncodingPso::ReSet()
{
	m_pFieldBandCombo->clear();

	m_pRButtonUValues->setChecked( false );
	m_pRButtonRange->setChecked( false );
	m_pRButtonUValues->setEnabled( false );
	m_pRButtonRange->setEnabled( false );
	m_EType = ENONE;
	
	m_pRButtonBinary->setChecked( true );
	activateBinaryRButton( true );
	m_CType = CBinary;

	m_RangeGroup->setEnabled( false );
	m_UValuesGroup->setEnabled( false );
	m_pButnSave->setEnabled( false );
	m_pButnClear->setEnabled( false );
	m_pButnAddSchema->setEnabled( false );
	m_pButnVerify->setEnabled( false );

	closeFile();
	m_DType = DNONE;

	delete m_pTableModel;
	m_pTableModel = NULL;

	delete m_pRangeListModel;
	m_pRangeListModel = NULL;

	pStatisticsEdit->clear();
}


void EncodingPso::saveEncodeSchema()
{	
	if( !verifyEncoding( false) )
	{
		return;
	}

	QString strFileName = QFileDialog::getSaveFileName(this, tr("Save As File") , "E:/data" , tr("Xml (*.xml)"));

	if( strFileName == "" )
	{
		return;
	}

	QFile file( strFileName );
	
	if( !file.open( QIODevice::WriteOnly ) )
	{
		QMessageBox::information(0, tr("失败"), tr("保存编码方案失败!"), QMessageBox::Ok);
		return;
	}

	QDomDocument DomDocument;
	
	generateDomFromTableModel( *m_pTableModel , DomDocument ,  m_EType , m_CType );

	QTextStream tStream(&file);

	DomDocument.save( tStream , 8 );
	
	file.flush();
	file.close();
			
	QMessageBox::information(0, tr("成功"), tr("保存编码方案成功!"), QMessageBox::Ok);
}

QString EncodingPso::NoSuffix(const QString& strPath)
{
	if( strPath == "" )
	{
		return QString("");
	}

	const QChar *pChar = strPath.unicode();              //有/0

	int i;
	int nIndex = -1;
	int nNum = strPath.size();
	for( i = 0 ; i < nNum ; i++ )
	{
		if( pChar[i] == '.' )
		{
			nIndex = i;
		}
	}
	
	if( nIndex == -1 )
	{
		QMessageBox::information(0, tr("失败"), tr("路径不合乎规范"), QMessageBox::Ok);
		return QString("");
	}

	const int nNoSuffixNum = nIndex;                        //字符数

	QChar *pNewChar = new QChar[nNoSuffixNum+1];

	for( i = 0 ; i < nNoSuffixNum ; i++ )
	{
		pNewChar[i] = pChar[i];
	}

	pNewChar[nNoSuffixNum] = '\0';


	QString strResult( pNewChar );

	delete []pNewChar;

	return strResult;
}

QString EncodingPso::getSuffix( const QString& strPath )
{
	if( strPath == "" )
	{
		return QString("");
	}

	const QChar *pChar = strPath.unicode();              //有/0

	int i;
	int nIndex = -1;
	int nNum = strPath.size();
	for( i = 0 ; i < nNum ; i++ )
	{
		if( pChar[i] == '.' )
		{
			nIndex = i;
		}
	}
	
	if( nIndex == -1 )
	{
		QMessageBox::information(0, tr("失败"), tr("路径不合乎规范"), QMessageBox::Ok);
		return QString("");
	}
                       
	const int nSuffixNum = nNum - nIndex - 1;//字符数

	QChar *pSuffix = new QChar[nSuffixNum+1];

	for( i = 0 ; i < nSuffixNum ; i++ )
	{
		pSuffix[i] = pChar[i + nIndex + 1 ];
	}

	pSuffix[nSuffixNum] = '\0';


	QString strResult( pSuffix );

	delete []pSuffix;

	return strResult;
}

void EncodingPso::generateDomFromTableModel(const QStandardItemModel &rTableModel, QDomDocument &rDomDocument  , const EncodeType eType , const CodeType cType)
{
	rDomDocument.clear();
	QDomProcessingInstruction DomProcessingInstruction = rDomDocument.createProcessingInstruction( "xml" , "version=\"1.0\" " );
	rDomDocument.appendChild( DomProcessingInstruction );
	
	QDomElement root = rDomDocument.createElement( "Encode" );
	rDomDocument.appendChild( root );

	int nCountItem = rTableModel.rowCount();
	int i ,j;
		
	QDomElement eleTemp;
	QDomElement Item;
	QDomElement Code;
	
	if( eType == EUValues && cType == CBinary )
	{
		eleTemp = rDomDocument.createElement("EType");
		eleTemp.setAttribute( "Type" , "UValues" );
		root.appendChild( eleTemp );
		
		eleTemp = rDomDocument.createElement("CType");
		eleTemp.setAttribute( "Type" , "Binary" );
		root.appendChild( eleTemp );

		for( i = 0 ; i < nCountItem ; i++ )
		{
			Item = rDomDocument.createElement("Item");
			Item.setAttribute( "Code" , ( rTableModel.item( i , 1 ) )->text() );
			Item.setAttribute( "UValue" , ( rTableModel.item( i , 0 ) )->text() );
			root.appendChild( Item );
		}
	}
	else if(  eType == ERange && cType == CBinary )
	{
		eleTemp = rDomDocument.createElement("EType");
		eleTemp.setAttribute( "Type" , "Range" );
		root.appendChild( eleTemp );
		
		eleTemp = rDomDocument.createElement("CType");
		eleTemp.setAttribute( "Type" , "Binary" );
		root.appendChild( eleTemp );

		for( j = 0 ; j < 2 ; j++ )
		{
			Item = rDomDocument.createElement("Item");
			Item.setAttribute( "Code" , j );


			for( i = 0 ; i < nCountItem ; i++ )
			{

				if( ( ( m_pTableModel->item( i , 0 ) )->text() ).toInt() == j )
				{
					eleTemp = rDomDocument.createElement("Range");
					eleTemp.setAttribute( "Lower" , ( m_pTableModel->item( i , 1 ) )->text() );
					eleTemp.setAttribute( "Upper" , ( m_pTableModel->item( i , 2 ) )->text() );
					Item.appendChild( eleTemp );
				}
			}
			root.appendChild( Item );
		}
	}
	else if( eType == EUValues && cType == CReal )
	{
		eleTemp = rDomDocument.createElement("EType");
		eleTemp.setAttribute( "Type" , "UValues" );
		root.appendChild( eleTemp );
		
		eleTemp = rDomDocument.createElement("CType");
		eleTemp.setAttribute( "Type" , "Real" );
		
		int nCodesCount = m_pListModel->rowCount();
		for( i = 0 ; i< nCodesCount ; i++ )
		{
			Code = rDomDocument.createElement("Code");
			Code.setAttribute( "code" ,( m_pListModel->item( i ) )->text() );
			eleTemp.appendChild( Code );
		}

		root.appendChild( eleTemp );

		for( i = 0 ; i < nCountItem ; i++ )
		{
			Item = rDomDocument.createElement("Item");
			Item.setAttribute( "UValue" , ( rTableModel.item( i , 0 ) )->text() );
			Item.setAttribute( "Code" , ( rTableModel.item( i , 1 ) )->text() );
			root.appendChild( Item );
		}
	}
	else if( eType == ERange && cType == CReal )
	{
		eleTemp = rDomDocument.createElement("EType");
		eleTemp.setAttribute( "Type" , "Range" );
		root.appendChild( eleTemp );
		
		eleTemp = rDomDocument.createElement("CType");
		eleTemp.setAttribute( "Type" , "Real" );
		
		int nCodesCount = m_pListModel->rowCount();
		for( i = 0 ; i< nCodesCount ; i++ )
		{
			Code = rDomDocument.createElement("Code");
			Code.setAttribute( "code" ,( m_pListModel->item( i ) )->text() );
			eleTemp.appendChild( Code );
		}

		root.appendChild( eleTemp );

		QString strCode;
		for( j = 0 ; j < nCodesCount ; j++ )
		{
			strCode = ( m_pListModel->item( j ) )->text();
			Item = rDomDocument.createElement("Item");
			Item.setAttribute( "Code" ,strCode );
			
			for( i = 0 ; i < nCountItem ; i++ )
			{

				if(  ( m_pTableModel->item( i , 0 ) )->text()  == strCode )
				{
					eleTemp = rDomDocument.createElement("Range");
					eleTemp.setAttribute( "Lower" , ( m_pTableModel->item( i , 1 ) )->text() );
					eleTemp.setAttribute( "Upper" , ( m_pTableModel->item( i , 2 ) )->text() );
					Item.appendChild( eleTemp );
				}
			}
			root.appendChild( Item );
		}
	}
}

void EncodingPso::loadDomToTableModel( const QDomDocument& rDomDocument , QStandardItemModel& rTableModel ,  const EncodeType eType  )
{
	QDomNodeList nodeList;
	QDomElement nodeElement;

	if( eType == EUValues  )
	{
		int i,j;
		int nItemCount =  rTableModel.rowCount();
		
		nodeList = rDomDocument.elementsByTagName( "Item" );
		int nUValueCount = nodeList.count();

		QString attrUValue;
		
		QProgressBar ProgressBar(this);
		ProgressBar.setGeometry( ( pStackedWidget->pos() ).x() ,( m_pButnReset->pos() ).y() +30 , ( pStackedWidget->geometry() ).width() ,20 );
		ProgressBar.setRange( 0 ,  nItemCount );
		ProgressBar.show();
		
		for( i = 0 ; i < nItemCount ; i++ )
		{
			for( j = 0 ; j < nUValueCount ; j++ )
			{
				nodeElement =  ( nodeList.at(j) ).toElement() ;
				attrUValue = nodeElement.attribute( "UValue" );
				if( attrUValue == (rTableModel.item( i , 0 ) )->text() )
				{
					( rTableModel.item( i , 1 ) )->setText( nodeElement.attribute( "Code" ) );
					break;
				}
			}
			ProgressBar.setValue( i );
		}
	}
	else if(  eType == ERange )
	{
		QDomElement rangeElement;

		QStandardItem *pItem ;

		rTableModel.removeRows( 0 , rTableModel.rowCount() );
		
		nodeList = rDomDocument.elementsByTagName( "Item" );
		int nItemCount = nodeList.count();
		int i,j = 0;
		QString strCode;

		for( i = 0 ; i < nItemCount ; i++ )
		{
			nodeElement =    ( nodeList.at(i) ).toElement() ;
			strCode = nodeElement.attribute( "Code" );
			
			rangeElement = nodeElement.firstChildElement();
			while( !rangeElement.isNull() )
			{
				pItem = new QStandardItem( strCode );
				rTableModel.setItem( j , 0 , pItem );
				
				pItem = new QStandardItem( rangeElement.attribute( "Lower" ) );
				rTableModel.setItem( j , 1 , pItem );

				pItem = new QStandardItem( rangeElement.attribute( "Upper" ) );
				rTableModel.setItem( j , 2 , pItem );
				
				j++;
				rangeElement  = rangeElement.nextSiblingElement();
				
			}
		}

	}
}

void EncodingPso::loadEncodeSchema()
{
	QString strPath = QFileDialog::getOpenFileName(this,tr("Open Schema XML"), "E:/data", tr("Xml (*.xml)"));
	
	if( strPath == NULL )
	{
		return;
	}
	
	QFile xmlFile( strPath );

	if( !xmlFile.open( QIODevice::ReadOnly ))
	{
		QMessageBox::information(0, tr("失败"), tr("打开编码方案失败!"), QMessageBox::Ok);
		return;
	}
	
	QDomDocument DomDocument;
	
	if( !DomDocument.setContent( &xmlFile , false ) )
	{
		xmlFile.close();
		QMessageBox::information(0, tr("失败"), tr("加载编码方案失败!"), QMessageBox::Ok);
		return;
	}

	xmlFile.close();

	QDomElement nodeElement;

	nodeElement = DomDocument.documentElement();

	if( nodeElement.tagName() != "Encode" )
	{
		QMessageBox::information(0, tr("失败"), tr("加载的不是编码方案!"), QMessageBox::Ok);
		return;
	}

	nodeElement = nodeElement.firstChildElement() ;
	if( nodeElement.attribute( "Type" ) != returnEncodeType() )
	{
		QMessageBox::information(0, tr("失败"), tr("加载方案的编码类型不正确或者当前没有编码类型!"), QMessageBox::Ok);
		return;
	}

	nodeElement = nodeElement.nextSiblingElement();
	QString strCType =  nodeElement.attribute( "Type" );
	if( strCType != returnCodeType() )
	{
		QMessageBox::information(0, tr("失败"), tr("加载方案的编码值类型不正确!"), QMessageBox::Ok);
		return;
	}
	
	if( strCType == "Real" )
	{
		int nCodesCount = m_pListModel->rowCount();
		if( nCodesCount == 0 )
		{
			QMessageBox::information(0, tr("失败"), tr("加载方案的编码值与现有方案不匹配!"), QMessageBox::Ok);
			return;
		}
		
		int i;
		
		nodeElement = nodeElement.firstChildElement();
		QString strCode;
		while( !nodeElement.isNull() )
		{
			strCode = nodeElement.attribute( "code" );
			for( i = 0 ; i < nCodesCount ; i++ )
			{
				if( strCode == ( m_pListModel->item( i ) )->text() )
				{
					break;
				}

			}

			if( i == nCodesCount )
			{
				QMessageBox::information(0, tr("失败"), tr("加载方案的编码值与现有方案不匹配!"), QMessageBox::Ok);
				return;
			}
			nodeElement = nodeElement.nextSiblingElement();
		}
	}

	loadDomToTableModel( DomDocument , *m_pTableModel , m_EType );
}

void EncodingPso::clearEncodeSchema()
{
	if( QMessageBox::information(0, tr("清除"), tr("清除已有编码方案?"), QMessageBox::Yes , QMessageBox::No ) == QMessageBox::No  )
	{
		return ;
	}

	if( m_EType== EUValues )
	{
		setStringToModelColumn( m_pTableModel , 1 ,  QString( "" ) );
	}
	else if( m_EType == ERange )
	{
		setStringToModelColumn( m_pTableModel , 0 ,  QString( "" ) );
		setStringToModelColumn( m_pTableModel , 1 ,  QString( "0" ) );
		setStringToModelColumn( m_pTableModel , 2 ,  QString( "0" ) );
	}
}

void EncodingPso::setStringToModelColumn( QStandardItemModel *pTableModel , int nColumnIndex , const QString & str  )
{
	if( pTableModel == NULL )
	{
		return;
	}

	if( pTableModel->columnCount() < nColumnIndex + 1 )
	{
		return ;
	}
	int nCountItem = pTableModel->rowCount();
	if( nCountItem == 0 )
	{
		return;
	}

	int i;

	for( i = 0 ;i < nCountItem ; i++ )
	{
		( pTableModel->item( i , nColumnIndex ) )->setText( str ) ;
	}
}

const QString EncodingPso::returnCodeType()
{
	QString strCodetype;
	if( m_pRButtonBinary->isChecked() )
	{
		strCodetype = "Binary" ;
	}
	else
	{
		strCodetype = "Real" ;
	}
	return strCodetype;
}

const QString EncodingPso::returnEncodeType()
{
	QString strEncodetype;
	if( m_pRButtonUValues->isChecked() )
	{
		strEncodetype = "UValues" ;
	}
	else if( m_pRButtonRange->isChecked() ) 
	{
		strEncodetype = "Range" ;
	}
	else
	{
		strEncodetype = "NONE" ;
	}
	return strEncodetype;
}

void EncodingPso::closeFile()
{
	if( m_DType == Vector )
	{
		m_ShapeDriver.CloseFile();
	}else if( m_DType == Raster )
	{
		m_GdalDriver.CloseFile();
	}
}

void EncodingPso::loadXMLForNewField()
{
	if( ( m_pFieldBandCombo2->currentText() ).isEmpty() )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("未选需编码字段！") ), QMessageBox::Ok);
		return ;
	}

	if( ( m_pNewFieldCombo->currentText() ).isEmpty() )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("未选新字段的名称！") ), QMessageBox::Ok);
		return ;
	}

	m_strPathForNewField = QFileDialog::getOpenFileName(this,
		tr("Open File"), "E:/data", tr("XML files (*.xml)"));
	
	m_encodeDom.loadFromFile( m_strPathForNewField );
	
	if( m_encodeDom.isEmpty() )
	{
		m_pButnWriteNewField->setEnabled( false );
		return;
	}

	if( m_encodeDom.encodeType() != EncodeDom::EUValues )
	{
		m_encodeDom.clear();
		m_pButnWriteNewField->setEnabled( false );
		return;
	}

	m_pButnWriteNewField->setEnabled( true );
}

void EncodingPso::writeNewField()
{
	if( m_encodeDom.isEmpty() )
	{
		return;
	}

	QMap<QString , QString> map = m_encodeDom.returnUValuesAndCodes();

	if( map.count() == 0 )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("编码方案有误，写入不成功！") ), QMessageBox::Ok);
		return;
	}

	if( !m_ShapeDriver.Opened() )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("未获得图层，写入不成功！") ), QMessageBox::Ok);
	}


	OGRLayer* pLayer = m_ShapeDriver.GetLayer() ;
	
	// 查找字段是否已存在
	OGRFeatureDefn* pFeatureDefn = pLayer->GetLayerDefn();

	int nFieldIndex = pFeatureDefn->GetFieldIndex( unicodeToCstring( m_pNewFieldCombo->currentText() ).data() );
	
	if ( nFieldIndex  != -1 )
	{
		QMessageBox::information(0, tr("警告"), QString( tr("已有此字段，写入不成功！") ), QMessageBox::Ok);
		return;
	}

	// 创建字段
	OGRFieldDefn FieldDefn( unicodeToCstring( m_pNewFieldCombo->currentText() ).data() , OFTString);
	
	FieldDefn.SetWidth(32);
	
	if( pLayer->CreateField( &FieldDefn ) != OGRERR_NONE)
	{
		QMessageBox::information(0, tr("警告"), QString( tr("创建字段不成功！") ), QMessageBox::Ok);
		return;
	}
	
	// 写入值
	
	int nOldFieldIndex = pFeatureDefn->GetFieldIndex( unicodeToCstring( m_pFieldBandCombo2->currentText() ).data() );

	int nNewFieldIndex = ( pLayer->GetLayerDefn() )->GetFieldCount() - 1;
	
	int nCount = pLayer->GetFeatureCount();

	QString strKey;

	QProgressBar ProgressBar(this);
	ProgressBar.setRange( 0 , nCount );
	ProgressBar.setGeometry( ( pStackedWidget->pos() ).x() ,( m_pButnReset->pos() ).y() +30 , ( pStackedWidget->geometry() ).width() ,20 );
	ProgressBar.show();

	for (int i=0; i != nCount; ++i)
	{
		OGRFeature* pFeature = pLayer->GetFeature( i );

		strKey = pFeature->GetFieldAsString( nOldFieldIndex );
		
		pFeature->SetField( nNewFieldIndex , unicodeToCstring ( map.value( strKey  ) ).data()  );
		pLayer->SetFeature( pFeature ); 
		OGRFeature::DestroyFeature(pFeature);

		ProgressBar.setValue( i );
	}
}

QByteArray EncodingPso::unicodeToCstring( const QString& str )
{
	QTextCodec* pTextCode = QTextCodec::codecForLocale();

	QByteArray aryTemp( 256, '0');
	
	aryTemp = pTextCode->fromUnicode( str );

	return aryTemp;
}

void EncodingPso::activateFieldBandCombo2( int nIndex )
{
	if( m_pFieldBandCombo2->itemText( nIndex ).isEmpty() )
	{
		m_pButnWriteNewField->setEnabled( false );
	}
}

void EncodingPso::activateNewFieldCombo( int nIndex )
{
	if( m_pNewFieldCombo->itemText( nIndex ).isEmpty() )
	{
		m_pButnWriteNewField->setEnabled( false );
	}
}