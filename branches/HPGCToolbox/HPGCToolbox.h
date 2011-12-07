#ifndef _HPGCTOOLBOX_H_
#define _HPGCTOOLBOX_H_

#include "ui_HPGCToolbox.h"
#include <QDockWidget>
#include <QSize>
#include <QEvent>
#include <QDomElement>
#include <QTreeWidget>
#include <QFile>
#include "qgisinterface.h"


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
	void updateToolName(QTreeWidgetItem* item, int colum);

	/// 显示右键菜单
	void showRightMenu(const QPoint &pos);

	/// 添加工具箱
	void addToolbox();

	/// 添加工具集
	void addToolset();

	/// 添加工具
	void addTool();

	/// 改名
	void renameTool();

	/// 删除工具箱
	void deleteTool();

	/// 弹出工具集属性窗口
	void showProperty();

	/// 打开工具
	void openTool();

	/// 弹出指标管理窗口
	void showIndicators();

	/// 弹出公式管理窗口
	void showFormulas();

protected:
	/// 窗口大小改变事件
	void resizeEvent(QResizeEvent* event);

private:
	QgisInterface* _iface;		// QGIS操作接口


	/// 解析配置文件
	void parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement);
	
	/// QDomElement转QTreeWidgetItem
	QTreeWidgetItem elementToItem(QDomElement &element);

	/// 新建工具配置文件
	bool createToolConfig(const QString &filename, const QString &name, const QString &id);

	/// 显示参数配置界面


};


#endif // _HPGCTOOLBOX_H_