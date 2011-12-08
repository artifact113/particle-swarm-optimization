#include "HPGCToolbox.h"
#include <QObject>
#include <QWidget>
#include <QSize>
#include <QEvent>
#include <QDomElement>
#include <QDomDocument>
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
#include <QLibrary>
#include "FormProperty.h"
#include "IndicatorManagement.h"
#include "FormulaManagement.h"
#include "AlgorithmPlugin.h"
#include "XmlOperator.h"
#include "FileOperator.h"


/***********************************************public*********************************************/
/// 构造函数
HPGCToolbox::HPGCToolbox(QgisInterface *iface, const QString &title, QWidget *parent)
: QDockWidget(title, parent), _iface(iface)
{
	setupUi(this);
 
	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
	connect(treeToolbox, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showRightMenu(const QPoint &)));
	connect(treeToolbox, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openTool()));
}


/// 析构函数
HPGCToolbox::~HPGCToolbox()
{
 

}


/// 载入配置文件
bool HPGCToolbox::loadConfig()
{
	// 配置文件
	QString filename("./HPGCToolbox/config.xml");

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
	QTreeWidgetItem* rootItem = new QTreeWidgetItem(elementToItem(rootElement));	
	treeToolbox->addTopLevelItem(rootItem);
	rootItem->setExpanded(true);
    parseConfig(rootItem, rootElement);

	return true; 
}


/***********************************************public slots***************************************/
/// 更新工具名称
void HPGCToolbox::updateToolName(QTreeWidgetItem* item, int colum)
{
	if (colum != 0)
	{
		return;
	}	

	QString name = item->text(0);
	QString id = item->text(1);
	QString toolType = item->text(2);

	if(name.isEmpty())
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("New name cannot be empty!"));
		item->setText(0, QObject::tr("New Name"));
		return;
	}

	// 配置文件
	QString filename("./HPGCToolbox/config.xml");

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

	// 重写配置文件
	changedElement->setAttribute("name", name);
	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed update to config file!"));
	}
}


/// 显示右键菜单
void HPGCToolbox::showRightMenu(const QPoint &pos)
{
	QTreeWidgetItem* item = treeToolbox->itemAt(pos);

	QMenu* popMenu = new QMenu(treeToolbox);
	QAction* addToolbox = new QAction(QIcon(":/toolbox"), tr("AddToolbox"), 0);
	QAction* addToolset = new QAction(QIcon(":/toolset"), tr("AddToolset"), 0);
	QAction* addTool = new QAction(QIcon(":/tool"), tr("AddTool"), 0);
	QAction* renTool = new QAction(QIcon(":/rename"), tr("Rename"), 0);
	QAction* delTool = new QAction(QIcon(":/delete"), tr("Delete"), 0);
	QAction* properties = new QAction(QIcon(":/property"), tr("Property"), 0);
	QAction* openTool = new QAction(QIcon(":/opentool"),tr("Open"), 0);
	QAction* indicatorManagement = new QAction(QIcon(":/indicator"), tr("Indicator Management"), 0);
	QAction* formulaManagement = new QAction(QIcon(":/formula"), tr("Formula Management"), 0);


	connect(addToolbox, SIGNAL(triggered()), this, SLOT(addToolbox()));
	connect(addToolset, SIGNAL(triggered()), this, SLOT(addToolset()));
	connect(addTool, SIGNAL(triggered()), this, SLOT(addTool()));
	connect(renTool, SIGNAL(triggered()), this, SLOT(renameTool()));
	connect(delTool, SIGNAL(triggered()), this, SLOT(deleteTool()));
	connect(properties, SIGNAL(triggered()), this, SLOT(showProperty()));
	connect(openTool, SIGNAL(triggered()), this, SLOT(openTool()));
	connect(indicatorManagement, SIGNAL(triggered()), this, SLOT(showIndicators()));
	connect(formulaManagement, SIGNAL(triggered()), this, SLOT(showFormulas()));


	if (!item)
	{
		popMenu->addAction(indicatorManagement);
		popMenu->addAction(formulaManagement);
		popMenu->exec(QCursor::pos());
		return;
	}

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
		popMenu->addAction(delTool);

	}
	else if (toolType == "toolset")
	{		
		popMenu->addAction(addTool);
		popMenu->addSeparator();
		popMenu->addAction(renTool);
		popMenu->addAction(delTool);
		popMenu->addSeparator();
		popMenu->addAction(properties);

	}
	else if (toolType == "tool")
	{
		popMenu->addAction(openTool);
		popMenu->addSeparator();
		popMenu->addAction(renTool);		
		popMenu->addAction(delTool);
		
	}
	
	popMenu->exec(QCursor::pos());
}


/// 添加工具箱
void HPGCToolbox::addToolbox()
{
	QTreeWidgetItem* currentItem = treeToolbox->currentItem();
	QString id = currentItem->text(1);
	QString toolType = currentItem->text(2);
	
	// 配置文件
	QString filename("./HPGCToolbox/config.xml");

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

	// 新节点信息
	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Toolbox");
	
	// 写到配置文件
	QDomElement newElement = document.createElement("toolbox");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);	
	changedElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
	{
		connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
		return;
	}

	// 添加到treeToolbox中显示
	QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
	newItem->setIcon(0, QIcon(":/toolbox"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "toolbox");
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	currentItem->addChild(newItem);
	treeToolbox->setCurrentItem(newItem, 0);
	treeToolbox->editItem(newItem);

	treeToolbox->topLevelItem(0)->setText(3, newId);

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
}



/// 添加工具集
void HPGCToolbox::addToolset()
{
	// 指定算法包
	QString myfilename = QFileDialog::getOpenFileName(this, QObject::tr("Specify algorithm package"), "/", QObject::tr("Dynamic Link Library(*.dll)"));

	if (!myfilename.isEmpty())
	{
		// 验证算法包
		if (!FileOperator::VerifyAlgorithmFile(myfilename))
		{			
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("Incorrect algorithm package!\nClick YES to respecify one."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
			if (button == QMessageBox::Yes)
			{
				addToolset();
			}
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

		QTreeWidgetItem* currentItem = treeToolbox->currentItem();
		QString id = currentItem->text(1);
		QString toolType = currentItem->text(2);

		// 配置文件
		QString filename("./HPGCToolbox/config.xml");

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

		// 新节点信息
		int count = rootElement.attribute("count", "-1").toInt();
		QString newId = QString::number(count + 1);
		QString newName = tr("New Toolset");
		QString newFilename = tofilename;

		// 写入配置文件
		QDomElement newElement = document.createElement("toolset");
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
		if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
		{
			connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
			return;
		}

		QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
		newItem->setIcon(0, QIcon(":/toolset"));
		newItem->setText(0, newName);
		newItem->setText(1, newId);
		newItem->setText(2, "toolset");
		newItem->setText(3, newFilename);
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		newItem->setExpanded(true);
		currentItem->addChild(newItem);
		treeToolbox->setCurrentItem(newItem, 0);
		treeToolbox->editItem(newItem);

		treeToolbox->topLevelItem(0)->setText(3, newId);

		connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
	}
}


/// 添加工具
void HPGCToolbox::addTool()
{
	QTreeWidgetItem* currentItem = treeToolbox->currentItem();
	QString id = currentItem->text(1);
	QString toolType = currentItem->text(2);

	// 配置文件
	QString filename("./HPGCToolbox/config.xml");

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

	// 新节点信息
	int count = rootElement.attribute("count", "-1").toInt();
	QString newId = QString::number(count + 1);
	QString newName = tr("New Tool");
	QString newConfig = "./HPGCToolbox/ToolConfig/tool_" + newId + ".xml";
	
	// 新建工具配置文件
	createToolConfig(newConfig, newName, currentItem->text(3));

	// 写入配置文件
	QDomElement newElement = document.createElement("tool");
	newElement.setAttribute("name", newName);
	newElement.setAttribute("id", newId);
	newElement.setAttribute("config", newConfig);
	changedElement->appendChild(newElement);
	rootElement.setAttribute("count", newId);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
	{
		connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
		return;
	}

	QTreeWidgetItem* newItem = new QTreeWidgetItem(currentItem);	
	newItem->setIcon(0, QIcon(":/tool"));
	newItem->setText(0, newName);
	newItem->setText(1, newId);
	newItem->setText(2, "tool");
	newItem->setText(3, newConfig);
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	newItem->setExpanded(true);
	currentItem->addChild(newItem);
	treeToolbox->setCurrentItem(newItem, 0);
	treeToolbox->editItem(newItem);

	treeToolbox->topLevelItem(0)->setText(3, newId);

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));

}


/// 改名
void HPGCToolbox::renameTool()
{
	QTreeWidgetItem* currentItem = treeToolbox->currentItem();
	if (currentItem)
	{
		treeToolbox->editItem(currentItem, 0);
	}	
}


/// 删除工具箱
void HPGCToolbox::deleteTool()
{
	QTreeWidgetItem* currentItem = treeToolbox->currentItem();
	QString id = currentItem->text(1);
	QString toolType = currentItem->text(2);

	// 确认删除
	QMessageBox::StandardButton button = QMessageBox::warning(NULL, QObject::tr("HPGCToolbox"), QObject::tr("This will delete ALL sub-tools!\nClick YES to confirm."), QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
	if (button == QMessageBox::Cancel)
	{
		return;
	}	
	
	// 配置文件
	QString filename("./HPGCToolbox/config.xml");

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

	// 写入配置文件
	QDomNode parentNode = changedElement->parentNode();
	parentNode.removeChild(*changedElement);

	if (!XmlOperator::XmlWrite(document, filename))
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return;
	}

	// 使用Qt你不得不掌握些奇淫技巧
	if(!disconnect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int))))
	{
		connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
		return;
	}

	delete currentItem;

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));

}


/// 弹出工具集属性窗口
void HPGCToolbox::showProperty()
{
	QTreeWidgetItem* currentItem = treeToolbox->currentItem();

	if (currentItem)
	{
		FormProperty myForm(currentItem, ALGORITHM);
		myForm.exec();
	}
}


/// 打开工具
void HPGCToolbox::openTool()
{
	QTreeWidgetItem* currentItem = treeToolbox->currentItem();
	QString toolname(currentItem->text(0));
	QString tooltype = currentItem->text(2);
	if (tooltype == "tool")
	{
		QString config(currentItem->text(3));
		if (!XmlOperator::XmlVerify(config, ""))
		{
			QMessageBox::critical(NULL, QString::fromLocal8Bit("HPGCToolbox"), QString::fromLocal8Bit("配置文件内容格式不正确！"));
			return;
		}

		QString filename = 	currentItem->parent()->text(3);
		QLibrary myLibrary(filename);
		typedef AlgorithmPlugin* (*TypeClassFactory)();
		typedef void* (*TypeUnload)(AlgorithmPlugin*);
		TypeClassFactory pClassFactory = (TypeClassFactory)myLibrary.resolve("classFactory");
		TypeUnload pUnload = (TypeUnload)myLibrary.resolve("unload");		
		if (pClassFactory && pUnload)
		{
			AlgorithmPlugin* myPlugin = (*pClassFactory)();
			myPlugin->showUI(config, toolname, NULL);
			(*pUnload)(myPlugin);
		}
	}
	
}


/// 弹出指标管理窗口
void HPGCToolbox::showIndicators()
{
	IndicatorManagement myForm;
	if (!myForm.loadConfig())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("载入配置文件失败!"));
		return;
	}

	myForm.exec();
}


/// 弹出公式管理窗口
void HPGCToolbox::showFormulas()
{
	FormulaManagement myForm;
	if (!myForm.loadConfig())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("载入配置文件失败!"));
		return;
	}

	myForm.exec();
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
QTreeWidgetItem HPGCToolbox::elementToItem(QDomElement &element)
{
	QString toolType(element.tagName());
	QTreeWidgetItem item;
	item.setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	item.setText(0, element.attribute("name"));
	item.setText(1, element.attribute("id"));
	item.setText(2, element.tagName());

	if (toolType == "toolboxfolder")
	{
		item.setIcon(0, QIcon(":/toolboxfolder"));
		item.setText(3, element.attribute("count"));
	}
	else if (toolType == "toolbox")
	{
		item.setIcon(0, QIcon(":/toolbox"));
	}
	else if (toolType == "toolset")
	{
		item.setIcon(0, QIcon(":/toolset"));
		item.setText(3, element.attribute("filename"));
	}
	else if (toolType == "tool")
	{
		item.setIcon(0, QIcon(":/tool"));
		item.setText(3, element.attribute("config"));
	}
	
	return item;
}


/// 新建工具配置文件
bool HPGCToolbox::createToolConfig(const QString &savefilename, const QString &name, const QString &filename)
{
	QDomDocument document;
	QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version='1.0' encoding='utf-8'");
	document.appendChild(instruction);

	QDomElement rootElement = document.createElement("tool");	
	rootElement.setAttribute("name", name);
	rootElement.setAttribute("filename", filename);
	document.appendChild(rootElement);


	if (!XmlOperator::XmlWrite(document, savefilename))
	{
		QMessageBox::critical(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
		return false;
	}

	return true;
}