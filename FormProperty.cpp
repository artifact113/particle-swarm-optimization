#include "FormProperty.h"
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QString>
#include <QTreeWidgetItem>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomElement>
#include "XmlOperator.h"
#include "FileOperator.h"


/***********************************************public*********************************************/
/// 构造函数
FormProperty::FormProperty(QTreeWidgetItem* currentItem, const PLUGINTYPE &type, QWidget *parent)
:mType(type), QDialog(parent)
{
	setupUi(this);

	mCurrentItem = currentItem;

	// 设置窗口标题
	QString title = QObject::tr("Properties - ") + mCurrentItem->text(0);
	this->setWindowTitle(title);
	if (mType == ALGORITHM)
	{
		this->setWindowIcon(QIcon(":/toolset"));
	}
	else if(mType == INDICATOR)
	{
		this->setWindowIcon(QIcon(":/indicatorset"));
	}
	

	// 设定名称
	txtName->setText(mCurrentItem->text(0));

	connect(txtName, SIGNAL(textChanged(const QString &)), this, SLOT(activateBtnApply()));
	connect(txtFile, SIGNAL(textChanged(const QString &)), this, SLOT(activateBtnApply()));
	connect(btnApply, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
	connect(btnOpenFile, SIGNAL(clicked(bool)), this, SLOT(changeFile()));
	

	QString id = mCurrentItem->text(1);
	QString toolType = mCurrentItem->text(2);

	// 配置文件
	QString filename;
	if (mType == ALGORITHM)
	{
		filename = "./HPGCToolbox/config.xml";
	}
	else if (mType == INDICATOR)
	{
		filename = "./HPGCToolbox/indicator.xml";
	}

	// 验证配置文件
	if (!XmlOperator::XmlVerify(filename, ""))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect config file!"));
		return;
	}

	// 打开配置文件
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to read config file!"));
		return;
	}

	// 查询变更节点
	QDomElement rootElement = document.documentElement();
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, toolType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	QString myfilename = changedElement->attribute("filename","");
	txtFile->setText(QFileInfo(myfilename).absoluteFilePath());
	btnApply->setEnabled(false);

	// 验证算法包
	if (!FileOperator::VerifyFile(myfilename, mType))
	{			
		QMessageBox::StandardButton button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect algorithm package!\nClick YES to respecify one."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
		if (button == QMessageBox::Yes)
		{
			changeFile();
		}		
	}

	showDetail(txtFile->text());
}


/// 析构函数
FormProperty::~FormProperty()
{
	

}


/***********************************************public slots***************************************/
/// 激活btnApply按钮
void FormProperty::activateBtnApply()
{
	btnApply->setEnabled(true);
}


/// 保存配置
void FormProperty::saveConfig()
{
	QString myname = txtName->text();
	QString myfilename = txtFile->text();
	QString id = mCurrentItem->text(1);
	QString toolType = mCurrentItem->text(2);


	// 名称不能为空
	if (myname.isEmpty())
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Name cannot be empty!"));
		return;		
	}

	// 算法包也不能为空
	if (myfilename.isEmpty())
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("You must specify a algorithm package!"));
		return;
	}

	// 验证算法包
	if (!FileOperator::VerifyFile(myfilename, mType))
	{			
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect algorithm package!"));
		return;
	}

	// 复制算法包
	QFileInfo myFileInfo(myfilename);
	QString tofilename = "./HPGCToolbox/Algorithm/" + myFileInfo.fileName();

	if (!FileOperator::CopyFile(myfilename, tofilename))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed copy file to the program folder!"));
		return;
	}

	// 配置文件
	QString filename;
	if (mType == ALGORITHM)
	{
		filename = "./HPGCToolbox/config.xml";
	}
	else if (mType == INDICATOR)
	{
		filename = "./HPGCToolbox/indicator.xml";
	}
	
	

	// 验证配置文件
	if (!XmlOperator::XmlVerify(filename, ""))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect config file!"));
		return;
	}

	// 打开配置文件
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to read config file!"));
		return;
	}

	// 查询变更节点
	QDomElement rootElement = document.documentElement();
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, toolType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 写配置文件
	changedElement->setAttribute("filename", tofilename);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	mCurrentItem->setText(0, myname); // 会自动更新配置文件的name属性	
	mCurrentItem->setText(3, tofilename);
	this->close();
}


/// 变更算法包
void FormProperty::changeFile()
{
	QString newFilename = QFileDialog::getOpenFileName(this, QObject::tr("Respecify algorithm package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));
	if (!newFilename.isEmpty())
	{
		if (FileOperator::VerifyAlgorithmFile(newFilename))
		{
			QFileInfo newFileInfo(newFilename);
			txtFile->setText(newFileInfo.absoluteFilePath());
			showDetail(newFilename);
		}
		else
		{
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect algorithm file!\nClick YES to respecify one."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
			if (button == QMessageBox::Yes)
			{
				changeFile();
			}
		}
	}
	

}
/***********************************************protected******************************************/




/***********************************************private********************************************/
/// 显示DLL详情
void FormProperty::showDetail(const QString &file)
{

}