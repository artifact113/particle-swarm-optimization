#ifndef _HPGCTOOLBOX_H_
#define _HPGCTOOLBOX_H_

#include "ui_HPGCToolbox.h"
#include <QDockWidget>
#include "qgisplugin.h"
#include <QSize>
#include <QResizeEvent>
#include <QDomElement>


class HPGCToolbox : public QDockWidget, public Ui::HPGCToolbox
{
    Q_OBJECT

public:
	explicit HPGCToolbox(QgisInterface* iface, const QString & title ,QWidget *parent = 0);
    ~HPGCToolbox();


	/// 载入配置文件
	bool loadConfig();
	

public slots:
	


protected:
	void resizeEvent(QResizeEvent* event);


private:
	QgisInterface* _iface;


	/// 解析配置文件
	bool parseConfig(QDomElement &root);
	
};


#endif // _HPGCTOOLBOX_H_