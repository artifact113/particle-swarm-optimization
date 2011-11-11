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


/***********************************************public*********************************************/
/// 构造函数
FormProperty::FormProperty(QTreeWidgetItem* currentItem, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	_currentItem = currentItem;
	QString title = QObject::tr("Properties - ") + _currentItem->text(0);
	this->setWindowTitle(title);
	txtName->setText(_currentItem->text(0));

	connect(txtName, SIGNAL(textChanged(const QString &)), this, SLOT(activateBtnApply()));
	connect(txtFile, SIGNAL(textChanged(const QString &)), this, SLOT(activateBtnApply()));
	connect(btnApply, SIGNAL(clicked(bool)), this, SLOT(saveConfig()));
	connect(btnOpenFile, SIGNAL(clicked(bool)), this, SLOT(changeFile()));

	btnApply->setEnabled(false);
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
	QString newName = txtName->text();
	QString newFilename = txtFile->text();

	if (!newName.isEmpty())
	{
		_currentItem->setText(0, newName);
	}
	
	if (!newFilename.isEmpty())
	{

	}
	
	this->close();
}


/// 变更算法包
void FormProperty::changeFile()
{
	QString newFilename = QFileDialog::getOpenFileName(this, QObject::tr("Specify algorithm package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));
	if (!newFilename.isEmpty())
	{
		QFile newFile(newFilename);
		if (verifyDLL(newFile))
		{
			QFileInfo newFileInfo(newFilename);
			txtFile->setText(newFileInfo.absoluteFilePath());
		}
		
	}
	

}
/***********************************************protected******************************************/




/***********************************************private********************************************/
/// 验证DLL
bool FormProperty::verifyDLL(QFile &file)
{
	QFileInfo fileInfo(file);

	// 1.检查文件是否存在
	if (!fileInfo.exists())
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("The file doesn't exist!\n") + fileInfo.absoluteFilePath());
		return false;			
	}

	// 2.验证DLL
	return true;	
}