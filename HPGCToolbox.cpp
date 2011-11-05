#include "HPGCToolbox.h"
#include <QWidget>
#include <QSize>
#include <QResizeEvent>
#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTreeWidget>
#include <QIcon>

HPGCToolbox::HPGCToolbox(QgisInterface *iface, const QString &title, QWidget *parent)
: QDockWidget(title, parent), _iface(iface)
{
	setupUi(this);	
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
	QFile file("./HPGCToolbox/config.xml");
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Open config file failed!"));
        return false;
	}

	// 读取配置文件
    QString errorStr;
    int errorLine;
    int errorColumn; 
    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) 
	{
		file.close();
		QMessageBox::warning(this,  tr("Warning"),  tr("Read config file failed!"));
		return false;
    }
	file.close();
 
	// 验证配置文件
    QDomElement rootElement = domDocument.documentElement();
    if (rootElement.tagName() != "toolboxfolder")
	{
		file.close();
		QMessageBox::warning(this,  tr("Warning"), tr("Incorrect config file!"));
		return false;
    }

	// 解析配置文件
	QTreeWidgetItem* rootItem = new QTreeWidgetItem(elementToItem(rootElement));
	treeToolbox->addTopLevelItem(rootItem);
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
	return item;
}

