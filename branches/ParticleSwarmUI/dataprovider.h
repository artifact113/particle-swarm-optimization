#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include "ogrsf_frmts.h"

#include <QString>
#include <QtCore/qglobal.h>

#include "gdal_priv.h"
#include "cpl_conv.h" 

class  ShapeDriver
{
public:
	ShapeDriver();
	~ShapeDriver();
	

	
	bool OpenFile(QString strPath);
	void CloseFile();
	OGRDataSource* GetDataSource() const;
	OGRLayer* GetLayer() const;
	QString GetPath() const ;
	void SetPath(QString strPath);

	bool Opened() const ;

private:
	OGRDataSource* m_pDataSource;
	OGRLayer* m_pLayer;
	QString m_Path;

	bool HasOpened;                     //是否成功打开文件

};

class  GdalDriver
{
public:
	GdalDriver();
	~GdalDriver();
	
	bool OpenFile(QString strPath);
	void CloseFile();
	GDALDataset  *GetDateSet() const;
	QString GetPath() const ;
	void SetPath(QString strPath);

	bool Opened() const ;

private:
	GDALDataset  *m_pDataset;
	QString m_Path;

	bool HasOpened;                     //是否成功打开文件

};

#endif // SHAPEPROVIDER_H
