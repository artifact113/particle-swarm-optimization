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
	
	enum DataType { DNONE , Vector , Raster };  //Vector��ʸ���ļ�  Raster��դ���ļ� NONEΪδ���ļ�,�ļ�δ��ʱ��һ����NONE
	
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

	bool verifyEncoding( bool infoOK );    //����BOOLֵ

	void verifyEncodingForClick();        //����BOOLֵ

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

	void showFieldsInfo( const ShapeDriver& rShapeDriver , QComboBox& rComboBox );                               //�����ļ�
	
	void showBands( const GdalDriver& rGdalDriver , QComboBox& rComboBox ); 

    QStandardItemModel* setupUValuesTableModelForVector( const ShapeDriver& rShapeDriver ,const QString& strFieldName );                    //�����ļ�,����QStandardItemModel,��Ϊ�ý�����ʱ�漰��Ա���������ԷǶ�������

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
	
	const QString returnCodeType();   //���Ƕ�������

	const QString returnEncodeType();  
	
	void closeFile();    //���Ƕ�������

	void addStringToList( const QString& str1 , QList<double>& list1 , const QString& str2 , QList<double>& list2 );

	void loadDomToTableModel( const QDomDocument& rDomDocument , QStandardItemModel& rTableModel ,  const EncodeType eType  );//ֻ���ز���֤

	template<typename T>
	void setupUValuesTableForBand( QStandardItemModel* pTableModel ,GDALRasterBand  *pBand );    //TΪ���������

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
