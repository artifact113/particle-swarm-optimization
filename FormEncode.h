#ifndef _FORMENCODE_H_
#define _FORMENCODE_H_

#include "ui_FormEncode.h"
#include "dataProvider.h"
#include <QDialog>
#include <QString>
#include <QDomDocument>
#include <QList>


class FormEncode : public QDialog  
{
	Q_OBJECT

public:
	explicit FormEncode( QWidget *parent = 0 );
	FormEncode( const QString& strFilePath , QWidget *parent = 0 );
	~FormEncode();

public slots:
	void ClickOnButnOpen();
	void ClickOnButnAdd();
	void ClickOnButnDelete();
	void ClickOnButnConfirm();
	void ClickOnButnSave();
	void ClickOnButnLoad();

	void ActivateCBoxField( int nIndex );
	void ActivateCBoxEncodeType( int nIndex );

private:
	void getFields( const QString& strPath );
	void initialiseDlg();
	void getUValues( );                                  //根据当前界面状态获取唯一值，本身不打开文件
	bool isReadyToGetUValues();
	void setTableWidgetPropertyAfterGetUValues();
	void initialiseList();
	bool isReadyToGetRanges();
	void setTableWidgetPropertyAfterGetRanges();
	bool checkOutputFieldName();
	void writeUValueEncode();                            //只在ClickOnButnConfirm()中调用
	void writeRangeEncode();                             //只在ClickOnButnConfirm()中调用
	void saveUValueEncode();                             
	void saveRangeEncode();
	void generateUValueEncodeDom( QDomDocument& dom );                     //只在saveUValueEncode()中调用
	void generateRangeEncodeDom( QDomDocument& dom );                      //只在saveRangeEncode()中调用
	void loadUValueEncode();             
	void loadRangeEncode();  
	void loadUValueEncodeFromDom( const QDomDocument& dom );             //只在loadRangeEncode()中调用
	void loadRangeEncodeFromDom( const QDomDocument& dom );              //只在loadRangeEncode()中调用
	QString generateRange( const double min , const double max );


private:
	Ui::FormEncode ui;
	ShapefileProvider m_shapefile;
	QList<double> m_pointList;

};

#endif // _FORMENCODE_H_