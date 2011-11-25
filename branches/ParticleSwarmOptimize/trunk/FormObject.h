#ifndef _FORMOBJECT_H_
#define _FORMOBJECT_H_

#include "ui_FormObject.h"
#include <QObject>
#include <QDialog>

class FormObject : public QDialog, public Ui::FormObject
{
	Q_OBJECT

public:
	explicit FormObject(QWidget *parent = 0);
	~FormObject();


public slots:


protected:


private:


};


#endif // _FORMOBJECT_H_