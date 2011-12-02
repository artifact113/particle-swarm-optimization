#ifndef _FORMOBJECT_H_
#define _FORMOBJECT_H_

#include "ui_FormObject.h"
#include <QObject>
#include <QDialog>


class FormObject : public QDialog, public Ui::FormObject
{
	Q_OBJECT

public:
	explicit FormObject(QWidget *parent = 0);
	~FormObject();

	
public slots:
	/// 打开数据源
	void openDataSource();

	/// 打开算法参数设置界面
	void setAlgorithmParam();

protected:


private:
	QString mDataSource;
	QString mFiledName;
	QMap<QString, QString> mEncode;

};


#endif // _FORMOBJECT_H_