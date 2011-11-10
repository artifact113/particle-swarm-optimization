#include "HPGCToolbox.h"
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
#include "XmlOperator.h"


/***********************************************public*********************************************/
/// 构造函数
HPGCToolbox::HPGCToolbox(QgisInterface *iface, const QString &title, QWidget *parent)
: QDockWidget(title, parent), _iface(iface)
{
	setupUi(this);

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
	connect(treeToolbox, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showRightMenu(const QPoint &)));
}


/// 析构函数
HPGCToolbox::~HPGCToolbox()
{


}


/// 载入配置文件
bool HPGCToolbox::loadConfig()
{
	// 打开配置文件
	QString filename("./HPGCToolbox/config.xml");
	QDomDocument document = XmlOperator::XmlRead(filename);
	if (document.isNull())
	{
		return false;
	}

	QDomElement rootElement = document.documentElement();
	if (rootElement.isNull())
	{
		return false;
	}

	// 验证配置文件
    if (rootElement.tagName() != "toolboxfolder")
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect config file!"));
		return false;
    }

	// 解析配置文件
	QTreeWidgetItem* rootItem = new QTreeWidgetItem(elementToItem(rootElement));	
	treeToolbox->addTopLevelItem(rootItem);
	rootItem->setExpanded(true);
    parseConfig(rootItem, rootElement);

	return true; 
}


/***********************************************public slots***************************************/
/// 更新工具名称
void HPGCToolbox::updateToolName(QTreeWidgetItem* item, int column)
{
	QString name = item->text(0);
	QString id = item->text(1);
	QString toolType = item->text(2);
	if(name.isEmpty() || id.isEmpty())
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("New name cannot be empty!"));
		return;
	}

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

	QDomElement* changedElement = elementByID(rootElement, id, toolType);
	if (!changedElement)
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed to find the match record!"));
		return;
	}

	changedElement->setAttribute("name", name);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed update to config file!"));
	}	
}


/// 显示右键菜单
void HPGCToolbox::showRightMenu(const QPoint &pos)
{
	QTreeWidgetItem* item = treeToolbox->itemAt(pos);
	if (!item)
	{
		return;
	}

	QMenu* popMenu = new QMenu(treeToolbox);
	QAction* addToolbox = new QAction(QIcon(":/toolbox"), tr("AddToolbox"), 0);
	QAction* addToolset = new QAction(QIcon(":/toolset"), tr("AddToolset"), 0);
	QAction* addTool = new QAction(QIcon(":/tool"), tr("AddTool"), 0);
	QAction* renTool = new QAction(QIcon(":/rename"), tr("Rename"), 0);
	QAction* delTool = new QAction(QIcon(":/delete"), tr("Delete"), 0);
	QAction* properties = new QAction(QIcon(":/property"), tr("property"), 0);

	connect(addToolbox, SIGNAL(triggered()), this, SLOT(addToolbox()));
	connect(addToolset, SIGNAL(triggered()), this, SLOT(addToolset()));
	connect(addTool, SIGNAL(triggered()), this, SLOT(addTool()));
	connect(renTool, SIGNAL(triggered()), this, SLOT(renameTool()));
	connect(delTool, SIGNAL(triggered()), this, SLOT(deleteTool()));
	connect(properties, SIGNAL(triggered()), this, SLOT(showProperty()));

	QString toolType = item->text(2);
	if (toolType == "toolboxfolder")
	{
		popMenu->addAction(addToolbox);
		popMenu->addSeparator();
		popMenu->addAction(renTool);
	}
	else if (toolType == "toolbox")
	{
		popMenu->addAction(addToolbox);
		popMenu->addAction(addToolset);
		popMenu->addSeparator();
		popMenu->addAction(renTool);
		popMenu->addSeparator();
		popMenu->addAction(delTool);

	}
	else if (toolType == "toolset")
	{		
		popMenu->addAction(addTool);
		popMenu->addSeparator();
		popMenu->addAction(renTool);
		popMenu->addSeparator();
		popMenu->addAction(delTool);
		popMenu->addSeparator();
		popMenu->addAction(properties);

	}
	else if (toolType == "tool")
	{
		popMenu->addAction(renTool);		
		popMenu->addSeparator();
		popMenu->addAction(delTool);
		
	}
	
	popMenu->exec(QCursor::pos());
}


/// 添加工具箱
void HPGCToolbox::addToolbox()
{
	QTreeWidgetItem* item = treeToolbox->currentItem();
	QString id = item->text(1);
	QString toolType = item->text(2);
	
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

	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Toolbox");
	
	QDomElement newElement = document.createElement("toolbox");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);	
	currentElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
	{
		connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
		return;
	}

	QTreeWidgetItem* newItem = new QTreeWidgetItem(item);	
	newItem->setIcon(0, QIcon(":/toolbox"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "toolbox");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	item->addChild(newItem);
	treeToolbox->setCurrentItem(newItem, 0);
	treeToolbox->editItem(newItem);
	

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
}



/// 添加工具集
void HPGCToolbox::addToolset()
{
	QString myfilename = QFileDialog::getOpenFileName(this, QObject::tr("Specify algorithm package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));
	if (!myfilename.isNull())
	{
		QFile myfile(myfilename);

		// 验证并复制DLL
		if (verifyDLL(myfile))
		{
			if (copyDLL(myfile))
			{
				QTreeWidgetItem* item = treeToolbox->currentItem();
				QString id = item->text(1);
				QString toolType = item->text(2);

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

				int count = rootElement.attribute("count", "-1").toInt();
				QString newId = QString::number(count + 1);
				QString newName = tr("New Toolset");
				QString newFilename = "./HPGCToolbox/ToolsetDLL/" + QFileInfo(myfile).fileName();

				QDomElement newElement = document.createElement("toolset");
				newElement.setAttribute("name", newName);
				newElement.setAttribute("id", newId);
				newElement.setAttribute("filename", newFilename);
				currentElement->appendChild(newElement);
				rootElement.setAttribute("count", newId);

				if (!XmlOperator::XmlWrite(document, filename))
				{
					QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
					return;
				}

				// 使用Qt你不得不掌握些奇淫技巧
				if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
				{
					connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
					return;
				}

				QTreeWidgetItem* newItem = new QTreeWidgetItem(item);	
				newItem->setIcon(0, QIcon(":/toolset"));
				newItem->setText(0, newName);
				newItem->setText(1, newId);
				newItem->setText(2, "toolset");
				newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
				newItem->setExpanded(true);
				item->addChild(newItem);
				treeToolbox->setCurrentItem(newItem, 0);
				treeToolbox->editItem(newItem);

				connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
			}			
		}
	}
}


/// 添加工具
void HPGCToolbox::addTool()
{
	
}


/// 改名
void HPGCToolbox::renameTool()
{
	QTreeWidgetItem* item = treeToolbox->currentItem();
	treeToolbox->editItem(item, 0);
}


/// 删除工具箱
void HPGCToolbox::deleteTool()
{
	QTreeWidgetItem* item = treeToolbox->currentItem();
	QString id = item->text(1);
	QString toolType = item->text(2);
	
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

	QDomNode parentNode = currentElement->parentNode();
	parentNode.removeChild(*currentElement);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
	{
		connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
		return;
	}

	delete item;

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));

}


/// 弹出工具集属性窗口
void HPGCToolbox::showProperty()
{
	QTreeWidgetItem* item = treeToolbox->currentItem();
	QString id = item->text(0);
	
	//FromProperty myFrom("");
	//myFrom.show();


}
/***********************************************protected******************************************/
/// 窗口大小变更事件
void HPGCToolbox::resizeEvent(QResizeEvent* event)
{	
	if (this->isFloating())
	{
		treeToolbox->move(0, 0);
		treeToolbox->resize(this->size());		
	}
	else
	{
		treeToolbox->move(0, 20);
		treeToolbox->resize(this->width(), this->height() - 20);
	}
	
	QWidget::resizeEvent(event);
}


/***********************************************private********************************************/
/// 解析配置文件
void HPGCToolbox::parseConfig(QTreeWidgetItem* parentItem, QDomElement &parentElement)
{
	QDomNodeList nodes(parentElement.childNodes());
	QStringList toolTypes;
	toolTypes<<"toolboxfolder"<<"toolbox"<<"toolset"<<"tool";

	for (int i=0; i != nodes.count(); ++i)
	{
		QDomElement element(nodes.at(i).toElement());
		if (toolTypes.contains(element.tagName()))
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(elementToItem(element));			
			parentItem->addChild(item);
			item->setExpanded(true);
			if (element.hasChildNodes())
			{
				parseConfig(item, element);
			}
		}
	}
}


/// QDomElement转QTreeWidgetItem
QTreeWidgetItem HPGCToolbox::elementToItem(QDomElement &element)
{
	QString toolType(element.tagName());
	QTreeWidgetItem item;

	if (toolType == "toolboxfolder")
	{
		item.setIcon(0, QIcon(":/toolboxfolder"));		
	}
	else if (toolType == "toolbox")
	{
		item.setIcon(0, QIcon(":/toolbox"));		
	}
	else if (toolType == "toolset")
	{
		item.setIcon(0, QIcon(":/toolset"));
	}
	else if (toolType == "tool")
	{
		item.setIcon(0, QIcon(":/tool"));
	}

	item.setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	item.setText(0, element.attribute("name"));
	item.setText(1, element.attribute("id"));
	item.setText(2, element.tagName());
	return item;
}



/// 返回当前id的节点指针
QDomElement* HPGCToolbox::elementByID(QDomElement &element, const QString &id, const QString &toolType)
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


/// 验证DLL
bool HPGCToolbox::verifyDLL(QFile &file)
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
bool HPGCToolbox::copyDLL(QFile &file)
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