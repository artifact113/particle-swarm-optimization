#ifndef _FORMOBJECT_H_
#define _FORMOBJECT_H_

#include "ui_FormObject.h"
#include <QObject>
#include <QDialog>
#include <QString>
#include <QSet>


class FormObject : public QDialog, public Ui::FormObject
{
	Q_OBJECT

public:
	explicit FormObject(const QString &config, const QString &toolName, QWidget* parent = 0);
	~FormObject();


	/// 初始化配置
	void initData();

public slots:
	/// 打开数据源
	void openDataSource();

	/// 显示编码界面
	void showEncode();

	/// 填充cmbFieldName
	void fillFieldName(const QString &filename);

	/// 打开算法参数设置界面
	void setAlgorithmParam();

protected:


private:
	QString mConfig;
	QDomDocument mConfigDocument;
	QString mDataSource;
	QString mFiledName;
	QSet<QString> msetUniqueValue;
};


#endif // _FORMOBJECT_H_