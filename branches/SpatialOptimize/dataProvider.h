#ifndef SHAPEFILEPROVIDER_H
#define SHAPEFILEPROVIDER_H

#include "ogrsf_frmts.h"

#include <QString>
#include <QtCore/qglobal.h>

class  ShapefileProvider
{
public:
	ShapefileProvider();
	~ShapefileProvider();
	
	bool openFile( const QString& strPath );
	void closeFile();
	
	OGRDataSource* getDataSource() const;
	OGRLayer* getLayer() const;
	QString getPath() const ;

	bool opened() const ;

private:
	ShapefileProvider( const ShapefileProvider& );
	ShapefileProvider& 	operator = ( const ShapefileProvider& );

private:
	OGRDataSource* m_pDataSource;
	OGRLayer* m_pLayer;
	QString m_Path;
	bool m_hasOpened;                     //是否成功打开文件

};

#endif