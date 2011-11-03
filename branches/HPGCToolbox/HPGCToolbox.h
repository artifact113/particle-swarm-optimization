#ifndef _HPGCTOOLBOX_H_
#define _HPGCTOOLBOX_H_

#include "ui_HPGCToolbox.h"
#include <QDockWidget>
#include "qgisplugin.h"
#include <QSize>
#include <QResizeEvent>


class HPGCToolbox : public QDockWidget, public Ui::HPGCToolbox
{
    Q_OBJECT

public:
	explicit HPGCToolbox(QgisInterface* iface, const QString & title ,QWidget *parent = 0);
    ~HPGCToolbox();
	

public slots:
	


protected:
	void resizeEvent(QResizeEvent* event);


private:
	QgisInterface* _iface;


	/// 载入配置文件
	bool loadConfig();

};


#endif // _HPGCTOOLBOX_H_