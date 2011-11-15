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

	QString id = _currentItem->text(1);
	QString toolType = _currentItem->text(2);

	// 打开配置文件
	QString filename("./HPGCToolbox/config.xml");
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		return;
	}

	QDomElement rootElement = document.documentElement();
	if (rootElement.isNull())
	{
		return;
	}

	QDomElement* currentElement = elementByID(rootElement, id, toolType);
	if (!currentElement)
	{
		QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed to find the match record!"));
		return;
	}

	QString myfilename = currentElement->attribute("filename","");
	txtFile->setText(QFileInfo(myfilename).absoluteFilePath());

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
	QString myName = txtName->text();
	QString myFilename = txtFile->text();

	if (!myName.isEmpty())
	{
		_currentItem->setText(0, myName);
	}
	
	if (!myFilename.isEmpty())
	{
		QFile myfile(myFilename);

		// 验证并复制DLL
		if (verifyDLL(myfile))
		{
			if (copyDLL(myfile))
			{
				QString id = _currentItem->text(1);
				QString toolType = _currentItem->text(2);

				// 打开配置文件
				QString filename("./HPGCToolbox/config.xml");
				QDomDocument document = XmlOperator::XmlRead(filename);
				if (document.isNull())
				{
					return;
				}

				QDomElement rootElement = document.documentElement();
				if (rootElement.isNull())
				{
					return;
				}

				QDomElement* currentElement = elementByID(rootElement, id, toolType);
				if (!currentElement)
				{
					QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed to find the match record!"));
					return;
				}

				QString newFilename = "./HPGCToolbox/ToolsetDLL/" + QFileInfo(myfile).fileName();

				currentElement->setAttribute("filename", newFilename);

				if (!XmlOperator::XmlWrite(document, filename))
				{
					QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
					return;
				}
			}			
		}
	}
	
	this->close();
}


/// 变更算法包
void FormProperty::changeFile()
{
	QString newFilename = QFileDialog::getOpenFileName(this, QObject::tr("Respecify algorithm package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));
	if (!newFilename.isEmpty())
	{
		QFile newFile(newFilename);
		if (verifyDLL(newFile))
		{
			QFileInfo newFileInfo(newFilename);
			txtFile->setText(newFileInfo.absoluteFilePath());
			showDetail(newFile);
		}
		else
		{
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect dll file!\nClick YES to respecify one."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
			if (button == QMessageBox::Yes)
			{
				changeFile();
			}		
			
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


/// 复制DLL至./HPGCToolbox/ToolsetDLL目录下
bool FormProperty::copyDLL(QFile &file)
{
	QFileInfo fileInfo(file);

	// 1.检查文件是否存在
	if (!fileInfo.exists())
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("The file doesn't exist!\n") + fileInfo.absoluteFilePath());
		return false;			
	}

	// 2.检查./HPGCToolbox/ToolsetDLL/目录下是否存在同名dll文件
	QFileInfo targetfileInfo("./HPGCToolbox/ToolsetDLL/" + fileInfo.fileName());
	if (targetfileInfo.exists())
	{
		if (targetfileInfo != fileInfo)
		{
			QMessageBox::StandardButton button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Found a same named file in the program folder!\n") + targetfileInfo.absolutePath() + QObject::tr("\nYou can choose YES to overwrite it(NOT Recommendant)."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
			if (button == QMessageBox::Yes)
			{
				QFile targetfile(targetfileInfo.absoluteFilePath());
				if (!(targetfile.remove() && file.copy(targetfileInfo.absoluteFilePath())))
				{
					QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed rewrite file to the program folder!\n") + targetfileInfo.absolutePath());
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		if(!file.copy(targetfileInfo.absoluteFilePath()))
		{
			QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed copy file to the program folder!\n") + targetfileInfo.absolutePath());
			return false;
		}
	}

	return true;
}


/// 返回当前id的节点指针
QDomElement* FormProperty::elementByID(QDomElement &element, const QString &id, const QString &toolType)
{
	if (element.isNull())
	{
		return NULL;
	}

	if (element.hasAttribute("id"))
	{
		if (element.attribute("id") == id)
		{
			return &element;
		}
	}


	QDomNodeList allchildNodes(element.elementsByTagName(toolType));
	for (int i=0; i != allchildNodes.count(); ++i)
	{
		QDomElement* childElement = new QDomElement(allchildNodes.at(i).toElement());
		if (childElement->hasAttribute("id"))
		{
			if (childElement->attribute("id") == id)
			{
				return childElement;
			}
		}
	}

	return NULL;
}


/// 显示DLL详情
void FormProperty::showDetail(QFile &file)
{

}