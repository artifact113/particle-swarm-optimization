#include "FormObject.h"
#include <QDialog>


/***********************************************public*********************************************/
/// 构造函数
FormObject::FormObject(QWidget *parent)
:QDialog(parent)
{
	setupUi(this);

}

/// 析构函数
FormObject::~FormObject()
{

}