#ifndef _FORMPROPERTY_H_
#define _FORMPROPERTY_H_


#include "ui_FormProperty.h"
#include "qgisplugin.h"
#include <QWidget>
#include <QDialog>


class FormProperty : public QDialog, public Ui::FormProperty
{
	Q_OBJECT
     
public:
	explicit FormProperty(const QString & title ,QWidget *parent = 0);
	~FormProperty();
};

#endif // _FORMPROPERTY_H_