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
	connect(ui->DialCost,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
	connect(ui->DialSuitability,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
	connect(ui->DialCompact,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
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
	ui->TxtCostTemp->setValue(ui->DialCost->value()/100.0);
	ui->TxtSuitTemp->setValue(ui->DialSuitability->value()/100.0);
	ui->TxtCompTemp->setValue(ui->DialCompact->value()/100.0);

	double sum = ui->TxtEcoTemp->value() + ui->TxtCostTemp->value() + ui->TxtSuitTemp->value() + ui->TxtCompTemp->value();

	ui->TxtEcoWeight->setValue(ui->TxtEcoTemp->value()/sum);
	ui->TxtCostWeight->setValue(ui->TxtCostTemp->value()/sum);
	ui->TxtSuitWeight->setValue(ui->TxtSuitTemp->value()/sum);
	ui->TxtCompWeight->setValue(ui->TxtCompTemp->value()/sum);

}