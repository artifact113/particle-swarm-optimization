#ifndef _INDICATORMANAGEMENT_H_
#define _INDICATORMANAGEMENT_H_

#include "ui_IndicatorManagement.h"
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QFile>
#include <QDomElement>


class IndicatorManagement : public QDialog, public Ui::IndicatorManagement
{
	Q_OBJECT

public:
	explicit IndicatorManagement(QWidget *parent = 0);
	~IndicatorManagement();


	/// 载入配置文件
	bool loadConfig();

public slots:
	/// 更新指标名称
	void updateIndicatorName(QTreeWidgetItem* item, int column);

	/// 显示右键菜单
	void showRightMenu(const QPoint &pos);

	/// 添加指标箱
	void addIndicatorbox();

	/// 添加指标集
	void addIndicatorset();

	/// 添加指标
	void addIndicator();

	/// 改名
	void renameIndicator();

	/// 删除指标
	void deleteIndicator();

	/// 弹出指标属性窗口
	void showProperty();

protected:
	/// 窗口大小改变事件
	void resizeEvent(QResizeEvent* event);

private:
	/// 解析配置文件
	void parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement);

	/// QDomElement转QTreeWidgetItem
	QTreeWidgetItem elementToItem(QDomElement &element);

	/// 指标名称唯一性检测
	bool isNameUnique(const QString &newName);
};

#endif _INDICATORMANAGEMENT_H_