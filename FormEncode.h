#ifndef _FORMENCODE_H_
#define _FORMENCODE_H_

#include "ui_FormEncode.h"
#include "dataProvider.h"
#include <QDialog>
#include <QString>

class FormEncode : public QDialog  
{
	Q_OBJECT

public:
	explicit FormEncode( QWidget *parent = 0 );
	FormEncode( const QString& strFilePath , QWidget *parent = 0 );
	~FormEncode();

public slots:
	void ClickOnButnOpen();

	void ActivateCBoxField( int nIndex );
	void ActivateCBoxEncodeType( int nIndex );

private:
	void getFields( const QString& strPath );
	void initialiseDlg();
	void getUValues( );                               //根据当前界面状态获取唯一值，本身不打开文件
	bool isReadyToGetUValues();
	void setTableWidgetPropertyAfterGetUValues();

private:
	Ui::FormEncode ui;
	ShapefileProvider m_shapefile;

};

#endif // _FORMENCODE_H_