#include "HPGCToolbox.h"
#include <QWidget>
#include <QSize>
#include <QResizeEvent>
#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTreeWidget>


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
    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "toolboxfolder")
	{
		file.close();
		QMessageBox::warning(this,  tr("Warning"), tr("Incorrect config file!"));
		return false;
    }

	// 解析配置文件
    parseConfig(root);

	return true; 
}


/// 解析配置文件
bool HPGCToolbox::parseConfig(QDomElement &root)
{

	treeToolbox->setHeaderItem(&itemRoot);
	return true;
}


QTreeWidgetItem HPGCToolbox::traverse(QDomElement element)
{
	if (!element.hasChildNodes())
	{
		QTreeWidgetItem item(elementToItem(element));
		QDomElement sibling = element.nextSiblingElement();
		while (!sibling.isNull())
		{
			QTreeWidgetItem siblingItem = traverse(sibling);
			sibling = sibling.nextSiblingElement();
		}
		return item;
	}


	

}


QTreeWidgetItem HPGCToolbox::elementToItem(QDomElement &element)
{
	QStringList strings()<<element.attribute("name")<<element.attribute("id");
	QTreeWidgetItem item(strings);
	item.setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
	
	switch(elementtagName())
	{
	case "tool":
		item.setIcon(0, Icon(":/tool"));
		break;
	case "toolset":
		item.setIcon(0, Icon(":/toolset"));
		break;
	case "toolbox":
		item.setIcon(0, Icon(":/toolbox"));
		break;
	case "toolboxfolder":
		item.setIcon(0, Icon(":/toolboxfolder"));
		break;
	default:
		return NULL;
	}

	return item;
}

