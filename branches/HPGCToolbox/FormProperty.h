#ifndef _FORMPROPERTY_H_
#define _FORMPROPERTY_H_

#include "ui_FormProperty.h"
#include <QString>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QFile>
#include <QDomElement>
#include "HPGCToolboxGlobal.h"


class FormProperty : public QDialog, public Ui::FormProperty
{
	Q_OBJECT
     
public:

	explicit FormProperty(QTreeWidgetItem* currentItem, const PLUGINTYPE &type, QWidget *parent = 0);
	~FormProperty();

public slots:
	/// 激活btnApply按钮
	void activateBtnApply();

	/// 保存配置
	void saveConfig();

	/// 改变算法包
	void changeFile();

protected:


private:
	QTreeWidgetItem* mCurrentItem;

	PLUGINTYPE mType;

	/// 显示DLL详情
	void showDetail(const QString &file, const PLUGINTYPE &type);

};

#endif // _FORMPROPERTY_H_