#include "FromProperty.h"
#include <QWidget>
#include <QDialog>


/***********************************************public*********************************************/
/// 构造函数
FromProperty::FromProperty(const QString &title, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
}


/// 析构函数
FromProperty::~FromProperty()
{


}