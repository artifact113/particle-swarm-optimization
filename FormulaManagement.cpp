#include "FormulaManagement.h"
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
#include "FormulaManagement.h"
#include "FormProperty.h"


/***********************************************public*********************************************/
/// 构造函数
FormulaManagement::FormulaManagement(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);

	connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
	connect(treeFormula, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showRightMenu(const QPoint &)));
	connect(treeFormula, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showProperty()));
}


/// 析构函数
FormulaManagement::~FormulaManagement()
{

}


/// 载入配置文件
bool FormulaManagement::loadConfig()
{
	// 配置文件
	QString filename("./HPGCToolbox/formula.xml");

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
		treeFormula->addTopLevelItem(childItem);
		childItem->setExpanded(true);
		parseConfig(childItem, childElement);
	}

	return true; 
}


/***********************************************public slots***************************************/
/// 更新公式名称
void FormulaManagement::updateFormulaName(QTreeWidgetItem* item, int column)
{
	if (column != 0)
	{
		return;
	}	

	QString name = item->text(0);
	QString id = item->text(1);
	QString formulaType = item->text(2);

	if(name.isEmpty())
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("New name cannot be empty!"));
		item->setText(0, QObject::tr("New Name"));
		return;
	}

	// 公式名称唯一性检测
	/*if (formulaType == "formulaset" && !isNameUnique(name))
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Already exists same named formulaset!\nSame name may cause problems,we sugguest that every formula should has a unique name."));
	}*/
	

	// 配置文件
	QString filename("./HPGCToolbox/formula.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, formulaType);
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
void FormulaManagement::showRightMenu(const QPoint &pos)
{
	QTreeWidgetItem* item = treeFormula->itemAt(pos);

	QMenu* popMenu = new QMenu(treeFormula);
	QAction* addFormulabox = new QAction(QIcon(":/folder"), tr("Add Formulabox"), 0);
	QAction* addFormulaset = new QAction(QIcon(":/formulaset"), tr("Add Formulaset"), 0);
	QAction* addFormula = new QAction(QIcon(":/formula"), tr("Add Formula"), 0);
	QAction* renFormula = new QAction(QIcon(":/rename"), tr("Rename"), 0);
	QAction* delFormula = new QAction(QIcon(":/delete"), tr("Delete"), 0);
	QAction* properties = new QAction(QIcon(":/property"), tr("Property"), 0);

	connect(addFormulabox, SIGNAL(triggered()), this, SLOT(addFormulabox()));
	connect(addFormulaset, SIGNAL(triggered()), this, SLOT(addFormulaset()));
	connect(addFormula, SIGNAL(triggered()), this, SLOT(addFormula()));
	connect(renFormula, SIGNAL(triggered()), this, SLOT(renameFormula()));
	connect(delFormula, SIGNAL(triggered()), this, SLOT(deleteFormula()));
	connect(properties, SIGNAL(triggered()), this, SLOT(showProperty()));

	if (!item)
	{
		popMenu->addAction(addFormulabox);
		popMenu->exec(QCursor::pos());
		return;
	}

	QString formulaType = item->text(2);
	if (formulaType == "formulabox")
	{
		popMenu->addAction(addFormulaset);
		popMenu->addAction(addFormula);
		popMenu->addSeparator();
		popMenu->addAction(renFormula);
		popMenu->addAction(delFormula);
	}
	else if (formulaType == "formulaset")
	{
		popMenu->addAction(addFormulaset);
		popMenu->addAction(addFormula);
		popMenu->addSeparator();
		popMenu->addAction(renFormula);
		popMenu->addAction(delFormula);
	}
	else if (formulaType == "formula")
	{
		popMenu->addAction(properties);
		popMenu->addSeparator();
		popMenu->addAction(renFormula);
		popMenu->addAction(delFormula);
	}

	popMenu->exec(QCursor::pos());
}


/// 添加公式箱
void FormulaManagement::addFormulabox()
{
	QString id = "0";
	QString formulaType = "formulaboxfolder";

	// 配置文件
	QString filename("./HPGCToolbox/formula.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, formulaType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 新节点信息
	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Formulabox");

	// 写到配置文件
	QDomElement newElement = document.createElement("formulabox");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);	
	changedElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int))))
	{
		connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
		return;
	}

	// 添加到treeFormula中显示
	QTreeWidgetItem* newItem = new QTreeWidgetItem();
	newItem->setIcon(0, QIcon(":/folder"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "formulabox");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	treeFormula->addTopLevelItem(newItem);
	treeFormula->setCurrentItem(newItem, 0);
	treeFormula->editItem(newItem);
	

	connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));

}


/// 添加公式集
void FormulaManagement::addFormulaset()
{
	QTreeWidgetItem* currentItem = treeFormula->currentItem();
	QString id = currentItem->text(1);
	QString formulaType = currentItem->text(2);

	// 配置文件
	QString filename("./HPGCToolbox/formula.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, formulaType);
	if (!changedElement)
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	// 新节点信息
	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Formulaset");

	// 写到配置文件
	QDomElement newElement = document.createElement("formulaset");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);
	changedElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int))))
	{
		connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
		return;
	}

	// 添加到treeFormula中显示
	QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
	newItem->setIcon(0, QIcon(":/formulaset"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "formulaset");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	currentItem->addChild(newItem);
	treeFormula->setCurrentItem(newItem, 0);
	treeFormula->editItem(newItem);
	

	connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
}



/// 添加公式
void FormulaManagement::addFormula()
{
	// 指定公式算法包
	QString myfilename = QFileDialog::getOpenFileName(this, QObject::tr("Specify formulaset package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));

	if (!myfilename.isEmpty())
	{
		// 验证公式算法包
		if (!FileOperator::VerifyFormulaFile(myfilename))
		{			
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect formula package!\nClick YES to respecify one."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
			if (button == QMessageBox::Yes)
			{
				addFormula();
			}
			return;
		}

		// 复制公式算法包
		QFileInfo myFileInfo(myfilename);
		QString tofilename = "./HPGCToolbox/Formula/" + myFileInfo.fileName();

		if (!FileOperator::CopyFile(myfilename, tofilename))
		{
			QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed copy file to the program folder!"));
			return;
		}

		QTreeWidgetItem* currentItem = treeFormula->currentItem();
		QString id = currentItem->text(1);
		QString formulaType = currentItem->text(2);

		// 配置文件
		QString filename("./HPGCToolbox/formula.xml");

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
		QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, formulaType);
		if (!changedElement)
		{
			QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
			return;
		}


		// 新节点信息
		int count = rootElement.attribute("count", "-1").toInt();
		QString newId = QString::number(count + 1);
		QString newName = tr("New Formula");
		QString newFilename = tofilename;

		// 写入配置文件
		QDomElement newElement = document.createElement("formula");
		newElement.setAttribute("name", newName);
		newElement.setAttribute("id", newId);
		newElement.setAttribute("filename", newFilename);
		changedElement->appendChild(newElement);
		rootElement.setAttribute("count", newId);

		if (!XmlOperator::XmlWrite(document, filename))
		{
			QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
			return;
		}

		// 使用Qt你不得不掌握些奇淫技巧
		if(!disconnect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int))))
		{
			connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
			return;
		}

		QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
		newItem->setIcon(0, QIcon(":/formula"));
		newItem->setText(0, newName);
		newItem->setText(1, newId);
		newItem->setText(2, "formula");
		newItem->setText(3, newFilename);
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		newItem->setExpanded(true);
		currentItem->addChild(newItem);
		treeFormula->setCurrentItem(newItem, 0);
		treeFormula->editItem(newItem);

		

		connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
	}
}


/// 改名
void FormulaManagement::renameFormula()
{
	QTreeWidgetItem* currentItem = treeFormula->currentItem();
	if (currentItem)
	{
		treeFormula->editItem(currentItem, 0);
	}	
}


/// 删除公式
void FormulaManagement::deleteFormula()
{
	QTreeWidgetItem* currentItem = treeFormula->currentItem();
	QString id = currentItem->text(1);
	QString formulaType = currentItem->text(2);

	// 确认删除
	QMessageBox::StandardButton button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("This will delete ALL sub-formulas!\nClick YES to confirm."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
	if (button == QMessageBox::Cancel)
	{
		return;
	}	

	// 配置文件
	QString filename("./HPGCToolbox/formula.xml");

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
	QDomElement* changedElement = XmlOperator::elementByID(rootElement, id, formulaType);
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
		QMessageBox::warning(NULL, tr("FormulaManagement"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int))))
	{
		connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));
		return;
	}

	delete currentItem;

	connect(treeFormula, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateFormulaName(QTreeWidgetItem*, int)));

}


/// 弹出公式属性窗口
void FormulaManagement::showProperty()
{
	QTreeWidgetItem* currentItem = treeFormula->currentItem();

	if (currentItem)
	{
		QString formulaType = currentItem->text(2);
		if (formulaType == "formula")
		{
			FormProperty myForm(currentItem, FORMULA);
			myForm.exec();
		}
	}
}


/***********************************************protected******************************************/
/// 窗口大小变更事件
void FormulaManagement::resizeEvent(QResizeEvent* event)
{	

	treeFormula->resize(this->size());
	QWidget::resizeEvent(event);
}


/***********************************************private********************************************/
/// 解析配置文件
void FormulaManagement::parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement)
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
QTreeWidgetItem FormulaManagement::elementToItem(QDomElement &element)
{
	QString formulaType(element.tagName());
	QTreeWidgetItem item;
	item.setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	item.setText(0, element.attribute("name"));
	item.setText(1, element.attribute("id"));
	item.setText(2, element.tagName());

	if (formulaType == "formulabox")
	{
		item.setIcon(0, QIcon(":/folder"));
	}
	else if (formulaType == "formulaset")
	{
		item.setIcon(0, QIcon(":/formulaset"));
	}
	else if (formulaType == "formula")
	{
		item.setIcon(0, QIcon(":/formula"));
		item.setText(3, element.attribute("filename", "NULL"));
	}

	return item;
}


/// 公式名称唯一性检测
bool FormulaManagement::isNameUnique(const QString &newName)
{
	treeFormula->setColumnCount(3);
	QList<QTreeWidgetItem*> formulas(treeFormula->findItems("formula", Qt::MatchFixedString | Qt::MatchCaseSensitive | Qt::MatchRecursive, 2));
	treeFormula->setColumnCount(1);

	QList<QString> names;
	QList<QTreeWidgetItem*>::iterator iterFormula;
	for (iterFormula = formulas.begin(); iterFormula != formulas.end(); ++iterFormula)
	{
		names<<((*iterFormula)->text(0));
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
