#ifndef _FORMENCODE_H_
#define _FORMENCODE_H_

#include "ui_FormEncode.h"
#include "dataProvider.h"
#include <QDialog>
#include <QString>
#include <set>

using namespace std;

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

	void ActivateCBoxField( int nIndex );
	void ActivateCBoxEncodeType( int nIndex );

private:
	void getFields( const QString& strPath );
	void initialiseDlg();
	void getUValues( );                               //根据当前界面状态获取唯一值，本身不打开文件
	bool isReadyToGetUValues();
	void setTableWidgetPropertyAfterGetUValues();
	void initialiseSet();
	bool isReadyToGetRanges();
	void showRangesFromSet(); 
	void setTableWidgetPropertyAfterGetGetRanges();
	bool checkOutputFieldName();

private:
	Ui::FormEncode ui;
	ShapefileProvider m_shapefile;
	set<double> m_rangeSet;

};

#endif // _FORMENCODE_H_