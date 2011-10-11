#include "MainForm.h"
#include "ui_MainForm.h"
#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include "ogrsf_frmts.h"

using namespace std;


MainForm::MainForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainForm)
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));

    ui->setupUi(this);

	setFixedSize(550,310);

	minEverEco = 0;
	maxEverEco = 300000;

	///	折叠或展开参数设置面板
	connect(ui->BtnParam,SIGNAL(clicked()),this,SLOT(FoldOrExpand()));

	///	计算权重值
	connect(ui->DialEconomic,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
	connect(ui->DialCost,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
	connect(ui->DialSuitability,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));
	connect(ui->DialCompact,SIGNAL(valueChanged(int)),this,SLOT(ChangeWeight()));

	///	打开文件
	connect(ui->BtnOpen,SIGNAL(clicked()),this,SLOT(OpenFile()));

	/// 更新文件相关信息
	connect(ui->TxtPath,SIGNAL(textChanged(const QString &)),this,SLOT(UpdateInfo(const QString &)));

	/// 开始PSO计算
	connect(ui->BtnExecute,SIGNAL(clicked()),this,SLOT(StartPSO()));
}


MainForm::~MainForm()
{
    delete ui;
}


///	折叠或展开参数设置面板
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


///	计算权重值
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


///	打开文件
void MainForm::OpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,"Open",QDir::currentPath(),"shp files (*.shp)");
	
	if (!fileName.isNull())	// 用户选择了文件
	{ 
		ui->TxtPath->setText(fileName);
	}
}


/// 更新文件相关信息
void MainForm::UpdateInfo(const QString & path)
{

	OGRRegisterAll();		// OGRRegisterAll()注册OGR支持格式的驱动器，这样可以访问OGR支持格式的文件
    OGRDataSource* poDS;	// OGRDataSource类代表数据源，一个数据源可以是一个或多个文件，也可以数据库

	QByteArray byte = path.toUtf8();
	const char* strPath = byte.data();
	poDS = OGRSFDriverRegistrar::Open(strPath);	// 以只读方式打开shapefile文件
	if(poDS == NULL)
	{
		QMessageBox::warning(this,"SpatialPSO",tr("打开文件失败!"));
		return;
	}

	OGRLayer  *poLayer;				// OGRLayer类表示数据源里的图层
    poLayer = poDS->GetLayer(0);	// 获取shapefile里的图层
	ui->SpinTotalPolygon->setValue(poLayer->GetFeatureCount());


	string layerName(poLayer->GetName()); 
	string strSQL = "SELECT SUM(SHAPE_Area) FROM " + layerName;
	OGRLayer  *resultLayer = poDS->ExecuteSQL(strSQL.c_str(),NULL,NULL);

	ui->SpinTotalArea->setValue(resultLayer->GetFeature(0)->GetFieldAsDouble(0)/666.67);

	
}


void MainForm::StartPSO()
{




}