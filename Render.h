#ifndef Render_H
#define Render_H

#include "qgisinterface.h"
#include "qgsrenderer.h"
#include "qgsvectorlayer.h"

#include <QString>
#include <QList>

class QColor;

class Render
{
public:
	Render ( QgisInterface* iface );
	~Render();
	
	//加载图层并渲染
	static bool uniquevalueRender( QgisInterface* iface , const QString& strPath , const QString& strFieldName , const QList<QString>* pUserUValuesList = NULL );             

	//渲染已加载图层
	static bool uniquevalueRender2( QgisInterface* iface , QgsVectorLayer* pVectorLayer  , const QString& strFieldName , const QList<QString>* pUserUValuesList = NULL );

private:
	QgisInterface* m_iface;

	static QList<QString> returnUValues( const QString& strPath , const QString& strFieldName );

	static QString Render::BaseName(const QString& strPath);

	static QColor randomColor();

	static bool check( const QString& str , const QList<QString>& rStrList );

};

#endif 