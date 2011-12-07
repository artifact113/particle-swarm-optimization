#include "FormObject.h"
#include <QString>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include "FormEncode.h"
#include "ShapefileOperator.h"
#include "XmlOperator.h"


/***********************************************public*********************************************/
/// 构造函数
FormObject::FormObject(const QString &config, const QString &toolName, QWidget* parent)
:QDialog(parent), mConfig(config)
{
	setupUi(this);
	this->setWindowTitle(toolName);
	initData();

	connect(btnOpenDataSource, SIGNAL(clicked()), this, SLOT(openDataSource()));

	connect(btnEncode, SIGNAL(clicked()), this, SLOT(showEncode()));

	connect(txtDataSource, SIGNAL(textChanged(const QString &)), this, SLOT(fillFieldName(const QString &)));

	connect(btnAlgorithmParamSet, SIGNAL(clicked()), this, SLOT(setAlgorithmParam()));

}


/// 析构函数
FormObject::~FormObject()
{

}


/// 初始化配置
void FormObject::initData()
{
	if (!XmlOperator::XmlVerify(mConfig, ""))
	{
		QDomProcessingInstruction instruction = mConfigDocument.createProcessingInstruction("xml", "version='1.0' encoding='utf-8'");
		mConfigDocument.appendChild(instruction);
		
		
		return;
	} 

		



	mConfigDocument = XmlOperator::XmlRead(mConfig);
	
}


/// 打开数据源
void FormObject::openDataSource()
{
	QString myfilename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("指定数据源"), "/", QString::fromLocal8Bit("Shape文件(*.shp)"));
	
	if (!myfilename.isEmpty())
	{
		mDataSource = myfilename;
		this->txtDataSource->setText(myfilename);		
	}
}


/// 显示编码界面
void FormObject::showEncode()
{
	if (mDataSource.isEmpty())
	{
		FormEncode myFormEncode;
		myFormEncode.exec();
	} 
	else
	{
		FormEncode myFormEncode(mDataSource);
		myFormEncode.exec();
	}
}


/// 填充cmbFieldName
void FormObject::fillFieldName(const QString &filename)
{
	this->cmbFieldName->clear();

	vector<QString> fieldNames(ShapefileReader::GetFieldNames(filename));
	vector<QString>::iterator iter;
	for (iter = fieldNames.begin(); iter != fieldNames.end(); ++iter)
	{
		QString fieldName(*iter);
		this->cmbFieldName->addItem(fieldName);
	}
}



/// 打开算法参数设置界面
void FormObject::setAlgorithmParam()
{

}