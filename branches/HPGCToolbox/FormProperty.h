#ifndef _FORMPROPERTY_H_
#define _FORMPROPERTY_H_

#include "ui_FormProperty.h"
#include <QString>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QFile>


class FormProperty : public QDialog, public Ui::FormProperty
{
	Q_OBJECT
     
public:
	explicit FormProperty(QTreeWidgetItem* currentItem,QWidget *parent = 0);
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
	QTreeWidgetItem* _currentItem;

	/// 验证DLL
	bool verifyDLL(QFile &file);

};

#endif // _FORMPROPERTY_H_