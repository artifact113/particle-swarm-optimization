#ifndef ENCODINGPSO_H
#define ENCODINGPSO_H

#include <QVector>
#include <QString>
#include <QDialog>
#include <QList>

#include "ui_Component.h"
#include "dataprovider.h"
#include "dataprovider.h"
#include "encodedom.h"

class QComboBox;
class QStandardItemModel;
class QStandardItem;
class QDomDocument;
class QByteArray;

class  EncodingPso :public QDialog , public Ui::Component
{
	Q_OBJECT
public:
	enum CodeType { CBinary , CReal };

	enum EncodeType { ENONE , EUValues , ERange };
	
	enum DataType { DNONE , Vector , Raster };  //Vector打开矢量文件  Raster打开栅格文件 NONE为未打开文件,文件未打开时不一定是NONE
	
	EncodingPso(  const QString& strPath , const QList<QString>& strList ,QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~EncodingPso();

public slots:
	
	void activatedFieldBandCombo( int nIndex );
	
	void activateBinaryRButton( bool checked );
	
	void activateRealRButton( bool checked );

	void activateUValuesRButton( bool checked );
	
	void activateRangeRButton( bool checked );

	void activateFieldBandCombo2( int nIndex );
	
	void activateNewFieldCombo( int nIndex );
	
	void addCodes();
	
	void delCodes();

	void addEncoding();

	void delEncoding();
	
	void nextPage();
	
	void previousPage();
	
	void verifyRealCodes( QStandardItem* pStandardItem);

	bool verifyEncoding( bool infoOK );    //传递BOOL值

	void verifyEncodingForClick();        //传递BOOL值

	void verifyRangeBoundary( QStandardItem* pStandardItem);
	
	void setRange();
	
	void ReSet();
	
	void saveEncodeSchema();
	
	void loadEncodeSchema();

	void clearEncodeSchema();

	void loadXMLForNewField();

	void writeNewField();

private:
	void initialDialog( const QString& strPath , const QList<QString>& strList );

	void showFieldsInfo( const ShapeDriver& rShapeDriver , QComboBox& rComboBox );                               //不打开文件
	
	void showBands( const GdalDriver& rGdalDriver , QComboBox& rComboBox ); 

    QStandardItemModel* setupUValuesTableModelForVector( const ShapeDriver& rShapeDriver ,const QString& strFieldName );                    //不打开文件,创建QStandardItemModel,因为用进度条时涉及成员变量，所以非独立函数

	QStandardItemModel* setupUValuesTableModelForRaster( const GdalDriver& rGdalDriver ,int nIndex );

	void setupUValuesTableModelFor8Bits( QStandardItemModel* pTableModel  , GDALRasterBand  *pBand );

	void setupUValuesTableModelFor16UInt( QStandardItemModel* pTableModel  , GDALRasterBand  *pBand );

	void setupUValuesTableModelFor16Int( QStandardItemModel* pTableModel  , GDALRasterBand  *pBand );

	QStandardItemModel* setupRangeListModelForVector( const ShapeDriver& rShapeDriver ,const QString& strFieldName ); 

	QStandardItemModel* setupRangeListModelForRaster( const GdalDriver& rGdalDriver ,int nIndex ); 

	void patchEncoding( QStandardItemModel& rTableModel , const double RangeLower , const double RangeUpper , const QString& str );
	
	QString NoSuffix(const QString& strPath );
	
	QString getSuffix( const QString& strPath );
	
	void generateDomFromTableModel(const QStandardItemModel& rTableModel , QDomDocument& rDomDocument , const EncodeType eType , const CodeType cType );
	
	void setStringToModelColumn(  QStandardItemModel* pTableModel , int nColumnIndex , const QString & str );
	
	const QString returnCodeType();   //不是独立函数

	const QString returnEncodeType();  
	
	void closeFile();    //不是独立函数

	void addStringToList( const QString& str1 , QList<double>& list1 , const QString& str2 , QList<double>& list2 );

	void loadDomToTableModel( const QDomDocument& rDomDocument , QStandardItemModel& rTableModel ,  const EncodeType eType  );//只加载不验证

	template<typename T>
	void setupUValuesTableForBand( QStandardItemModel* pTableModel ,GDALRasterBand  *pBand );    //T为分配的类型

	QByteArray unicodeToCstring( const QString& str );
	
	CodeType m_CType;
	EncodeType m_EType;
	DataType m_DType;

	QStandardItemModel *m_pTableModel;
	QStandardItemModel *m_pListModel;
	QStandardItemModel *m_pRangeListModel;
	ShapeDriver m_ShapeDriver;
	GdalDriver m_GdalDriver;

	QString m_strPathForNewField;
	EncodeDom m_encodeDom;
};

#endif // EncodingPso_H
