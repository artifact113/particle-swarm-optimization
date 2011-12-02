#include "FormObject.h"
#include <string>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "FormEncode.h"
#include "ShapefileOperator.h"


/***********************************************public*********************************************/
/// 构造函数
FormObject::FormObject(QWidget *parent)
:QDialog(parent)
{
	setupUi(this);
	
	connect(btnOpenDataSource, SIGNAL(clicked()), this, SLOT(openDataSource()));

	connect(btnEncode, SIGNAL(clicked()), this, SLOT(showEncode()));

	connect(txtDataSource, SIGNAL(textChanged(const QString &)), this, SLOT(fillFieldName(const QString &)));

	connect(btnAlgorithmParamSet, SIGNAL(clicked()), this, SLOT(setAlgorithmParam()));

}


/// 析构函数
FormObject::~FormObject()
{

}


/// 打开数据源
void FormObject::openDataSource()
{
	QString myfilename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("指定数据源"), "/", QString::fromLocal8Bit("Shape文件(*.shp)"));
	
	if (!myfilename.isEmpty())
	{
		this->txtDataSource->setText(myfilename);		
	}
}


/// 显示编码界面
void FormObject::showEncode()
{
	FormEncode myFormEncode;

	myFormEncode.exec();

}


/// 填充cmbFieldName
void FormObject::fillFieldName(const QString &filename)
{
	this->cmbFieldName->clear();

	vector<string> fieldNames(ShapefileReader::GetFieldNames((const char *)filename.toUtf8()));
	vector<string>::iterator iter;
	for (iter = fieldNames.begin(); iter != fieldNames.end(); ++iter)
	{
		QString fieldName(QString::fromLocal8Bit((*iter).c_str()));
		this->cmbFieldName->addItem(fieldName);
	}
}



/// 打开算法参数设置界面
void FormObject::setAlgorithmParam()
{

}