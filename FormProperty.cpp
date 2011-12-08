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
#include <QLibrary>
#include <QString>
#include "XmlOperator.h"
#include "FileOperator.h"
#include "AlgorithmPlugin.h"
#include "IndicatorPlugin.h"
#include "FormulaPlugin.h"



/***********************************************public*********************************************/
/// 构造函数
FormProperty::FormProperty(QTreeWidgetItem* currentItem, const PLUGINTYPE &type, QWidget *parent)
:mCurrentItem(currentItem), mType(type), QDialog(parent)
{
	setupUi(this);


	// 设置窗口图标和标题
	QString title = QString::fromLocal8Bit("属性 - ") + mCurrentItem->text(0);
	this->setWindowTitle(title);
	if (mType == ALGORITHM)
	{
		this->setWindowIcon(QIcon(":/toolset"));
	}
	else if(mType == INDICATOR)
	{
		this->setWindowIcon(QIcon(":/indicator"));
	}
	else if(mType == FORMULA)
	{
		this->setWindowIcon(QIcon(":/formula"));
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
	else if (mType ==  FORMULA)
	{
		filename = "./HPGCToolbox/formula.xml";
	}
	

	// 验证配置文件
	if (!XmlOperator::XmlVerify(filename, ""))
	{
		QMessageBox::critical(this,  QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("配置文件格式不正确！"));
		return;
	}

	// 打开配置文件
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("读取配置文件失败!"));
		return;
	}

	// 查询变更节点
	QDomElement rootElement = document.documentElement();
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, toolType);
	if (!changedElement)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("配置文件中未找到相应的记录!"));
		return;
	}

	QString myfilename = changedElement->attribute("filename","");
	txtFile->setText(QFileInfo(myfilename).absoluteFilePath());
	btnApply->setEnabled(false);

	// 验证算法包
	if (!FileOperator::VerifyFile(myfilename, mType))
	{			
		QMessageBox::StandardButton button = QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("不是合法的算法包!\n点击“是”重新指定."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
		if (button == QMessageBox::Yes)
		{
			changeFile();
		}		
	}

	showDetail(txtFile->text(),mType);
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
		QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("名称不能为空!"));
		return;		
	}

	// 算法包也不能为空
	if (myfilename.isEmpty())
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("必须指定一个算法包!"));
		return;
	}

	// 验证算法包
	if (!FileOperator::VerifyFile(myfilename, mType))
	{			
		QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("无效算法包!"));
		return;
	}

	// 复制算法包
	QFileInfo myFileInfo(myfilename);
	QString tofilename;

	if (mType == ALGORITHM)
	{
		tofilename = "./HPGCToolbox/Algorithm/" + myFileInfo.fileName();
	}
	else if (mType == INDICATOR)
	{
		tofilename = "./HPGCToolbox/Indicator/" + myFileInfo.fileName();
	}
	else if (mType == FORMULA)
	{
		tofilename = "./HPGCToolbox/Formula/" + myFileInfo.fileName();
	}
	

	if (!FileOperator::CopyFile(myfilename, tofilename))
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未能将该指标计算包复制到程序文件夹!"));
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
	else if (mType == FORMULA)
	{
		filename = "./HPGCToolbox/formula.xml";
	}
	
	

	// 验证配置文件
	if (!XmlOperator::XmlVerify(filename, ""))
	{
		QMessageBox::critical(this,  QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("配置文件格式不正确！"));
		return;
	}

	// 打开配置文件
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("读取配置文件失败!"));
		return;
	}

	// 查询变更节点
	QDomElement rootElement = document.documentElement();
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, toolType);
	if (!changedElement)
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("配置文件中未找到相应的记录!"));
		return;
	}

	// 写配置文件
	changedElement->setAttribute("filename", tofilename);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未能保存至配置文件!"));
		return;
	}

	mCurrentItem->setText(0, myname); // 会自动更新配置文件的name属性	
	mCurrentItem->setText(3, tofilename);
	this->close();
}


/// 变更算法包
void FormProperty::changeFile()
{
	QString newFilename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("指定算法包"), "/", QString::fromLocal8Bit("动态链接库(*.dll)"));
	if (!newFilename.isEmpty())
	{
		if (FileOperator::VerifyAlgorithmFile(newFilename))
		{
			QFileInfo newFileInfo(newFilename);
			txtFile->setText(newFileInfo.absoluteFilePath());
			showDetail(newFilename, mType);
		}
		else
		{
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("不是合法的算法包!\n点击“是”重新指定."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
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
void FormProperty::showDetail(const QString &file, const PLUGINTYPE &type)
{
	QLibrary myLibrary(file);

	if (mType == ALGORITHM)
	{
		typedef AlgorithmPlugin* (*TypeClassFactory)();
		typedef void* (*TypeUnload)(AlgorithmPlugin*);
		TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
		TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");		
		if (pClassFactory && pUnload)
		{
			AlgorithmPlugin* myPlugin = (*pClassFactory)();
			QString myLabel(myPlugin->name());
			QString myDescription(myPlugin->description());
			QString myHelp(myPlugin->help());

			txtLabel->setText(myLabel);
			textDescription->setText(myDescription);
			textHelp->setText(myHelp);

			(*pUnload)(myPlugin);
		}
	} 
	else if(mType == INDICATOR)
	{
		typedef IndicatorPlugin* (*TypeClassFactory)();
		typedef void* (*TypeUnload)(IndicatorPlugin*);
		TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
		TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");		
		if (pClassFactory && pUnload)
		{
			IndicatorPlugin* myPlugin = (*pClassFactory)();
			QString myLabel(myPlugin->name());
			QString myDescription(myPlugin->description());
			QString myHelp(myPlugin->help());

			txtLabel->setText(myLabel);
			textDescription->setText(myDescription);
			textHelp->setText(myHelp);

			(*pUnload)(myPlugin);
		}

	}
	else if (mType == FORMULA)
	{
		typedef FormulaPlugin* (*TypeClassFactory)();
		typedef void* (*TypeUnload)(FormulaPlugin*);
		TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
		TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");		
		if (pClassFactory && pUnload)
		{
			FormulaPlugin* myPlugin = (*pClassFactory)();
			QString myLabel(QString::fromLocal8Bit(myPlugin->name().c_str()));
			QString myDescription(QString::fromLocal8Bit(myPlugin->description().c_str()));
			QString myHelp(QString::fromLocal8Bit(myPlugin->help().c_str()));

			txtLabel->setText(myLabel);
			textDescription->setText(myDescription);
			textHelp->setText(myHelp);

			(*pUnload)(myPlugin);
		}

	}
	

}