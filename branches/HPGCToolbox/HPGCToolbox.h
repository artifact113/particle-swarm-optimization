#ifndef _HPGCTOOLBOX_H_
#define _HPGCTOOLBOX_H_

#include "ui_HPGCToolbox.h"
#include "qgisplugin.h"
#include <QDockWidget>
#include <QSize>
#include <QEvent>
#include <QDomElement>
#include <QTreeWidget>


class HPGCToolbox : public QDockWidget, public Ui::HPGCToolbox
{
    Q_OBJECT

public:
	explicit HPGCToolbox(QgisInterface* iface, const QString & title ,QWidget *parent = 0);
    ~HPGCToolbox();


	/// 载入配置文件
	bool loadConfig();
	

public slots:
	void updateToolName(QTreeWidgetItem* item, int column);


protected:
	void resizeEvent(QResizeEvent* event);

	void contextMenuEvent(QContextMenuEvent * event);


private:
	QgisInterface* _iface;


	/// 解析配置文件
	void parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement);
	
	/// QDomElement转QTreeWidgetItem
	QTreeWidgetItem elementToItem(QDomElement &element);

};


#endif // _HPGCTOOLBOX_H_