#include "IndicatorManagement.h"
#include <QObject>
#include <QWidget>
#include <QSize>
#include <QEvent>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QTreeWidget>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QPoint>
#include <QSet>
#include <QList>
#include "XmlOperator.h"
#include "FileOperator.h"


/***********************************************public*********************************************/
/// 构造函数
IndicatorManagement::IndicatorManagement(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);

	connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
	connect(treeIndicator, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showRightMenu(const QPoint &)));
	connect(treeIndicator, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showProperty()));
}


/// 析构函数
IndicatorManagement::~IndicatorManagement()
{

}


/// 载入配置文件
bool IndicatorManagement::loadConfig()
{
	// 配置文件
	QString filename("./HPGCToolbox/indicator.xml");

	// 验证配置文件
	if (!XmlOperator::XmlVerify(filename, ""))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect config file!"));
		return false;
	}

	// 打开配置文件
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to read config file!"));
		return false;
	}

	// 解析配置文件
	QDomElement rootElement = document.documentElement();
	QDomNodeList childNodes(rootElement.childNodes());
	for (int i=0; i != childNodes.count(); ++i)
	{
		QDomElement childElement(childNodes.at(i).toElement());
		QTreeWidgetItem* childItem = new QTreeWidgetItem(elementToItem(childElement));	
		treeIndicator->addTopLevelItem(childItem);
		childItem->setExpanded(true);
		parseConfig(childItem, childElement);
	}

	return true; 
}


/***********************************************public slots***************************************/
/// 更新指标名称
void IndicatorManagement::updateIndicatorName(QTreeWidgetItem* item)
{
	QString name = item->text(0);
	QString id = item->text(1);
	QString indicatorType = item->text(2);

	if(name.isEmpty())
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("New name cannot be empty!"));
		item->setText(0, QObject::tr("New Name"));
		return;
	}

	// 指标名称唯一性检测
	if (indicatorType == "indicator" && !isNameUnique(name))
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Already exists same named indicator!\nSame named indicators may cause problems,we sugguest that every indicator should has a unique name."));
	}
	

	// 配置文件
	QString filename("./HPGCToolbox/indicator.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, indicatorType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 重写配置文件
	changedElement->setAttribute("name", name);
	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed update to config file!"));
	}
}


/// 显示右键菜单
void IndicatorManagement::showRightMenu(const QPoint &pos)
{
	QTreeWidgetItem* item = treeIndicator->itemAt(pos);

	QMenu* popMenu = new QMenu(treeIndicator);
	QAction* addIndicatorbox = new QAction(QIcon(":/indicatorset"), tr("Add Indicatorset"), 0);
	QAction* addIndicatorset = new QAction(QIcon(":/indicatorset"), tr("Add Indicatorset"), 0);
	QAction* addIndicator = new QAction(QIcon(":/indicator"), tr("Add Indicator"), 0);
	QAction* renIndicator = new QAction(QIcon(":/rename"), tr("Rename"), 0);
	QAction* delIndicator = new QAction(QIcon(":/delete"), tr("Delete"), 0);
	QAction* properties = new QAction(QIcon(":/property"), tr("Property"), 0);

	connect(addIndicatorbox, SIGNAL(triggered()), this, SLOT(addIndicatorbox()));
	connect(addIndicatorset, SIGNAL(triggered()), this, SLOT(addIndicatorset()));
	connect(addIndicator, SIGNAL(triggered()), this, SLOT(addIndicator()));
	connect(renIndicator, SIGNAL(triggered()), this, SLOT(renameIndicator()));
	connect(delIndicator, SIGNAL(triggered()), this, SLOT(deleteIndicator()));
	connect(properties, SIGNAL(triggered()), this, SLOT(showProperty()));

	if (!item)
	{
		popMenu->addAction(addIndicatorbox);
		popMenu->exec(QCursor::pos());
		return;
	}

	QString indicatorType = item->text(2);
	if (indicatorType == "indicatorset")
	{
		popMenu->addAction(addIndicatorset);
		popMenu->addAction(addIndicator);
		popMenu->addSeparator();
		popMenu->addAction(renIndicator);
		popMenu->addAction(delIndicator);
	}
	else if (indicatorType == "indicator")
	{
		popMenu->addAction(properties);
		popMenu->addSeparator();
		popMenu->addAction(renIndicator);
		popMenu->addAction(delIndicator);
	}

	popMenu->exec(QCursor::pos());
}


/// 添加指标箱
void IndicatorManagement::addIndicatorbox()
{
	QString id = "0";
	QString indicatorType = "indicatorbox";

	// 配置文件
	QString filename("./HPGCToolbox/indicator.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, indicatorType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 新节点信息
	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Indicatorset");

	// 写到配置文件
	QDomElement newElement = document.createElement("indicatorset");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);	
	changedElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, tr("IndicatorManagement"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*))))
	{
		connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
		return;
	}

	// 添加到treeIndicator中显示
	QTreeWidgetItem* newItem = new QTreeWidgetItem();
	newItem->setIcon(0, QIcon(":/indicatorset"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "indicatorset");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	treeIndicator->addTopLevelItem(newItem);
	treeIndicator->setCurrentItem(newItem, 0);
	treeIndicator->editItem(newItem);

	connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));


}


/// 添加指标集
void IndicatorManagement::addIndicatorset()
{
	QTreeWidgetItem* currentItem = treeIndicator->currentItem();
	QString id = currentItem->text(1);
	QString indicatorType = currentItem->text(2);

	// 配置文件
	QString filename("./HPGCToolbox/indicator.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, indicatorType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 新节点信息
	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Indicatorset");

	// 写到配置文件
	QDomElement newElement = document.createElement("indicatorset");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);	
	changedElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, tr("IndicatorManagement"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*))))
	{
		connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
		return;
	}

	// 添加到treeIndicator中显示
	QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
	newItem->setIcon(0, QIcon(":/indicatorset"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "indicatorset");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	currentItem->addChild(newItem);
	treeIndicator->setCurrentItem(newItem, 0);
	treeIndicator->editItem(newItem);

	connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
}



/// 添加指标
void IndicatorManagement::addIndicator()
{
	// 指定指标算法包
	QString myfilename = QFileDialog::getOpenFileName(this, QObject::tr("Specify indicator package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));

	if (!myfilename.isEmpty())
	{
		// 验证指标算法包
		if (!FileOperator::VerifyIndicatorFile(myfilename))
		{			
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect indicator package!\nClick YES to respecify one."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
			if (button == QMessageBox::Yes)
			{
				addIndicator();
			}
			return;
		}

		// 复制指标算法包
		QFileInfo myFileInfo(myfilename);
		QString tofilename = "./HPGCToolbox/Indicator/" + myFileInfo.fileName();

		if (!FileOperator::CopyFile(myfilename, tofilename))
		{
			QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed copy file to the program folder!"));
			return;
		}

		QTreeWidgetItem* currentItem = treeIndicator->currentItem();
		QString id = currentItem->text(1);
		QString indicatorType = currentItem->text(2);

		// 配置文件
		QString filename("./HPGCToolbox/indicator.xml");

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
		QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, indicatorType);
		if (!changedElement)
		{
			QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
			return;
		}



		// 新节点信息
		int count = rootElement.attribute("count", "-1").toInt();
		QString newId = QString::number(count + 1);
		QString newName = tr("New Indicator");
		QString newFilename = tofilename;
		QString newIndicID = "0";

		// 写入配置文件
		QDomElement newElement = document.createElement("indicator");
		newElement.setAttribute("name", newName);
		newElement.setAttribute("id", newId);
		newElement.setAttribute("filename", newFilename);
		newElement.setAttribute("indicID", newIndicID);
		changedElement->appendChild(newElement);
		rootElement.setAttribute("count", newId);

		if (!XmlOperator::XmlWrite(document, filename))
		{
			QMessageBox::critical(NULL, tr("IndicatorManagement"), tr("Failed update to config file!"));
			return;
		}

		// 使用Qt你不得不掌握些奇淫技巧
		if(!disconnect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*))))
		{
			connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
			return;
		}

		QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
		newItem->setIcon(0, QIcon(":/indicator"));
		newItem->setText(0, newName);
		newItem->setText(1, newId);
		newItem->setText(2, "indicator");
		newItem->setText(3, newFilename);
		newItem->setText(4, newIndicID);
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		newItem->setExpanded(true);
		currentItem->addChild(newItem);
		treeIndicator->setCurrentItem(newItem, 0);
		treeIndicator->editItem(newItem);

		connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
	}
}


/// 改名
void IndicatorManagement::renameIndicator()
{
	QTreeWidgetItem* currentItem = treeIndicator->currentItem();
	if (currentItem)
	{
		treeIndicator->editItem(currentItem, 0);
	}	
}


/// 删除指标
void IndicatorManagement::deleteIndicator()
{
	QTreeWidgetItem* currentItem = treeIndicator->currentItem();
	QString id = currentItem->text(1);
	QString indicatorType = currentItem->text(2);

	// 确认删除
	QMessageBox::StandardButton button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("This will delete ALL sub-indicators!\nClick YES to confirm."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
	if (button == QMessageBox::Cancel)
	{
		return;
	}	

	// 配置文件
	QString filename("./HPGCToolbox/indicator.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, indicatorType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 写入配置文件
	QDomNode parentNode = changedElement->parentNode();
	parentNode.removeChild(*changedElement);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::warning(NULL, tr("IndicatorManagement"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*))))
	{
		connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));
		return;
	}

	delete currentItem;

	connect(treeIndicator, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateIndicatorName(QTreeWidgetItem*)));

}


/// 弹出指标属性窗口
void IndicatorManagement::showProperty()
{
	QTreeWidgetItem* currentItem = treeIndicator->currentItem();

	if (currentItem)
	{
		QString indicatorType = currentItem->text(2);
		if (indicatorType == "indicator")
		{
			//FormProperty myForm(currentItem);
			//myForm.exec();
		}
	}
}


/***********************************************protected******************************************/
/// 窗口大小变更事件
void IndicatorManagement::resizeEvent(QResizeEvent* event)
{	

	treeIndicator->resize(this->size());
	QWidget::resizeEvent(event);
}


/***********************************************private********************************************/
/// 解析配置文件
void IndicatorManagement::parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement)
{
	QDomNodeList nodes(parentElement.childNodes());
	for (int i=0; i != nodes.count(); ++i)
	{
		QDomElement element(nodes.at(i).toElement());
		QTreeWidgetItem* item = new QTreeWidgetItem(elementToItem(element));			
		parentItem->addChild(item);
		//item->setExpanded(true);
		if (element.hasChildNodes())
		{
			parseConfig(item, element);
		}
	}
}


/// QDomElement转QTreeWidgetItem
QTreeWidgetItem IndicatorManagement::elementToItem(QDomElement &element)
{
	QString indicatorType(element.tagName());
	QTreeWidgetItem item;
	item.setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	item.setText(0, element.attribute("name"));
	item.setText(1, element.attribute("id"));
	item.setText(2, element.tagName());

	if (indicatorType == "indicatorset")
	{
		item.setIcon(0, QIcon(":/indicatorset"));
	}
	else if (indicatorType == "indicator")
	{
		item.setIcon(0, QIcon(":/indicator"));
		item.setText(3, element.attribute("filename", "NULL"));
		item.setText(4, element.attribute("indicID","-1"));

	}

	return item;
}


/// 指标名称唯一性检测
bool IndicatorManagement::isNameUnique(const QString &newName)
{
	QList<QTreeWidgetItem*> indicators(treeIndicator->findItems("indicator", Qt::MatchFixedString | Qt::MatchRecursive, 2));

	QList<QString> names;
	QList<QTreeWidgetItem*>::iterator iterIndicator;
	for (iterIndicator = indicators.begin(); iterIndicator != indicators.end(); ++iterIndicator)
	{
		names<<((*iterIndicator)->text(0));
	}

	int count = 0;
	QList<QString>::iterator iterName;
	for (iterName = names.begin(); iterName != names.end(); ++iterName)
	{
		if ((*iterName) == newName)
		{
			++count;
		}		
	}

	if (count == 1)
	{
		return true;
	}
	else
	{
		return false;
	}	

}

	