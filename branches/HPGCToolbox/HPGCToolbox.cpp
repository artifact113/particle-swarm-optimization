#include "HPGCToolbox.h"
#include <QWidget>
#include <QSize>
#include <QResizeEvent>

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
	//treeToolbox->resize(event->size());
　　//HPGCToolbox::resizeEvent(event); 
}
