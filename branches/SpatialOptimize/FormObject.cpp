#include "FormObject.h"
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "FormEncode.h"


/***********************************************public*********************************************/
/// 构造函数
FormObject::FormObject(QWidget *parent)
:QDialog(parent)
{
	setupUi(this);
	
	connect(btnEncode, SIGNAL(clicked()), this, SLOT(openDataSource()));

	connect(btnAlgorithmParamSet, SIGNAL(clicked()), this, SLOT(setAlgorithmParam()));

}


/// 析构函数
FormObject::~FormObject()
{

}


/// 打开数据源
void FormObject::openDataSource()
{
	FormEncode myFormEncode;

	myFormEncode.exec();
}


/// 打开算法参数设置界面
void FormObject::setAlgorithmParam()
{

}