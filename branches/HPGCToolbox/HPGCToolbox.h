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
	/// 更新工具名称
	void updateToolName(QTreeWidgetItem* item, int column);

	/// 显示右键菜单
	void showRightMenu(const QPoint &pos);

	/// 改名
	void renameTool();

	/// 添加工具箱
	void addToolbox();


protected:
	/// 窗口大小改变事件
	void resizeEvent(QResizeEvent* event);

private:
	QgisInterface* _iface;



	/// 解析配置文件
	void parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement);
	
	/// QDomElement转QTreeWidgetItem
	QTreeWidgetItem elementToItem(QDomElement &element);

	/// 返回当前id的节点指针
	QDomElement* elementByID(QDomElement &element, const QString &id, const QString &toolType);

};


#endif // _HPGCTOOLBOX_H_