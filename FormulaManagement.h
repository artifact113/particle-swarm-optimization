#ifndef _FORMULAMANAGEMENT_H_
#define _FORMULAMANAGEMENT_H_

#include "ui_FormulaManagement.h"
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QFile>
#include <QDomElement>


class FormulaManagement : public QDialog, public Ui::FormulaManagement
{
	Q_OBJECT

public:
	explicit FormulaManagement(QWidget *parent = 0);
	~FormulaManagement();


	/// 载入配置文件
	bool loadConfig();

public slots:
	/// 更新公式名称
	void updateFormulaName(QTreeWidgetItem* item, int column);

	/// 显示右键菜单
	void showRightMenu(const QPoint &pos);

	/// 添加公式箱
	void addFormulabox();

	/// 添加公式集
	void addFormulaset();

	/// 添加公式
	void addFormula();

	/// 改名
	void renameFormula();

	/// 删除公式
	void deleteFormula();

	/// 弹出公式属性窗口
	void showProperty();

protected:
	/// 窗口大小改变事件
	void resizeEvent(QResizeEvent* event);

private:
	/// 解析配置文件
	void parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement);

	/// QDomElement转QTreeWidgetItem
	QTreeWidgetItem elementToItem(QDomElement &element);

	/// 公式名称唯一性检测
	bool isNameUnique(const QString &newName);
};

#endif _FORMULAMANAGEMENT_H_