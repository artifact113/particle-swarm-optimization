#include "MainForm.h"
#include "ui_MainForm.h"

MainForm::MainForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
	setFixedSize(550,310);
	connect(ui->BtnParam,SIGNAL(clicked()),this,SLOT(FoldOrExpand()));
	connect(ui->DialEconomic,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
}


MainForm::~MainForm()
{
    delete ui;
}


void MainForm::FoldOrExpand()
{
	int height = this->geometry().height();
	if (height == 310)
	{
		setFixedSize(550,600);
	}
	else
	{
		setFixedSize(550,310);
	}
}


void MainForm::ChangeWeight()
{
	ui->TxtEcoTemp->setValue(ui->DialEconomic->value()/100.0);
}