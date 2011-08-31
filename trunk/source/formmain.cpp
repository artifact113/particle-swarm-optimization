#include "formmain.h"
#include "ui_formmain.h"

FormMain::FormMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormMain)
{
    ui->setupUi(this);
}

FormMain::~FormMain()
{
    delete ui;
}
