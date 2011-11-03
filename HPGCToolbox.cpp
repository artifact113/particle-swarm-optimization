#include "HPGCToolbox.h"
#include <QWidget>
#include <QSize>
#include <QResizeEvent>
#include <QDomDocument>
#include <QFile>

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
	return true;
}
