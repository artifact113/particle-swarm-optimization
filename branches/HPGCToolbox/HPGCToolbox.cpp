#include "HPGCToolbox.h"
#include <QWidget>
#include <QSize>
#include <QEvent>
#include <QDomElement>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTreeWidget>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include "XmlOperator.h"

HPGCToolbox::HPGCToolbox(QgisInterface *iface, const QString &title, QWidget *parent)
: QDockWidget(title, parent), _iface(iface)
{
	setupUi(this);

	connect(treeToolbox, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateToolName(QTreeWidgetItem*, int)));
}


HPGCToolbox::~HPGCToolbox()
{


}


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


bool HPGCToolbox::loadConfig()
{
	// 打开配置文件
	QString filename("./HPGCToolbox/config.xml");
	QDomElement rootElement = XmlOperator::XmlRead(filename);

	if (rootElement.isNull())
	{
		return false;
	}

	// 验证配置文件
    if (rootElement.tagName() != "toolboxfolder")
	{
		QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Incorrect config file!"));
		return false;
    }

	// 解析配置文件
	QTreeWidgetItem* rootItem = new QTreeWidgetItem(elementToItem(rootElement));	
	treeToolbox->addTopLevelItem(rootItem);
	rootItem->setExpanded(true);
    parseConfig(rootItem, rootElement);

	return true; 
}


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


/// 右键菜单
void HPGCToolbox::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* popMenu = new QMenu(this);
    popMenu->addAction(new QAction(tr("Add"), this));
    popMenu->addAction(new QAction(tr("Delete"), this));
	QTreeWidgetItem* item = treeToolbox->itemAt(mapFromGlobal(QCursor::pos()));
    if(item)
    {
        popMenu->addAction(new QAction(tr("Edit"), this));
    }    
    popMenu->exec(QCursor::pos());
}


/// 更新工具名称
void HPGCToolbox::updateToolName(QTreeWidgetItem* item, int column)
{
	QString name = item->text(0);
	QString id = item->text(1);

	// 打开配置文件
	QString filename("./HPGCToolbox/config.xml");
	QDomElement rootElement = XmlOperator::XmlRead(filename);

	if (rootElement.isNull())
	{
		return;
	}

	QDomElement* changedElement = elementByID(rootElement, id);
	changedElement->setAttribute("name", name);

	if (!XmlOperator::XmlWrite(rootElement, filename))
	{
		QMessageBox::warning(NULL, tr("HPGCToolbox"), tr("Failed update to config file!"));
	}
	
}


/// 返回当前id的节点指针
QDomElement* HPGCToolbox::elementByID(QDomElement &element, const QString &id)
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


	for (int i=0; i != element.childNodes().count(); ++i)
	{
		QDomElement childElement = element.childNodes().at(i).toElement();
		if (childElement.hasAttribute("id"))
		{
			if (childElement.attribute("id") == id)
			{
				return &childElement;
			}

		}
	}

	return NULL;
}


