#include "FormProperty.h"
#include <QWidget>
#include <QDialog>


/***********************************************public*********************************************/
/// 构造函数
FormProperty::FormProperty(const QString &title, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
}


/// 析构函数
FormProperty::~FormProperty()
{


}