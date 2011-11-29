#include "FormObject.h"
#include <QDialog>
#include <QFileDialog>
#include <QString>


/***********************************************public*********************************************/
/// 构造函数
FormObject::FormObject(QWidget *parent)
:QDialog(parent)
{
	setupUi(this);
	
	connect(btnOpenDataSource, SIGNAL(clicked()), this, SLOT(openDataSource()));

}


/// 析构函数
FormObject::~FormObject()
{

}


/// 打开数据源
void FormObject::openDataSource()
{
	QString myfilename = QFileDialog::getOpenFileName(this, QObject::tr("Specify datasource"), "/", QObject::tr("Shape file(*.shp)"));

	if (!myfilename.isEmpty())
	{
		this->txtDataSource->setText(myfilename);
	}
}