#ifndef _FORMENCODE_H_
#define _FORMENCODE_H_

#include "ui_FormEncode.h"
#include <QDialog>

class FormEncode : public QDialog, public Ui::FormEncode
{
	Q_OBJECT

public:
	explicit FormEncode(QWidget *parent = 0);
	~FormEncode();

};

#endif // _FORMENCODE_H_