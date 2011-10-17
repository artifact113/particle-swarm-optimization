#include "MainForm.h"
#include "ui_MainForm.h"
#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "ogrsf_frmts.h"
#include "LandUseLayer.h"
#include "DatatypeOperator.h"
#include "ShapefileOperator.h"
#include "SpatialPSO.h"

using namespace std;



MainForm::MainForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainForm)
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    ui->setupUi(this);

	setFixedSize(550,310);


	///	折叠或展开参数设置面板
	connect(ui->BtnParam,SIGNAL(clicked()),this,SLOT(FoldOrExpand()));

	///	打开文件
	connect(ui->BtnOpen,SIGNAL(clicked()),this,SLOT(OpenFile()));

	/// 更新文件相关信息
	connect(ui->TxtPath,SIGNAL(textChanged(const QString &)),this,SLOT(UpdateInfo(const QString &)));

	/// 权重显示
	connect(ui->DialEconomic, SIGNAL(valueChanged(int)), this, SLOT(setBenefitWeight(int)));
	connect(ui->DialCost, SIGNAL(valueChanged(int)), this,SLOT(setChangeCostWeight(int)));
	connect(ui->DialSuitability, SIGNAL(valueChanged(int)), this, SLOT(setSuitabilityWeight(int)));
	connect(ui->DialCompact, SIGNAL(valueChanged(int)), this, SLOT(setCompactnessWeight(int)));

	connect(ui->BtnOpenSuit,SIGNAL(clicked()),this,SLOT(OpenSuitFile()));

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
	ui->stackedWidget->setCurrentIndex(0);

	string filePath(path.toStdString());

	// 1、建立LandUseLayer来描述shape文件
	int featureCount = ShapefileReader::GetFeatureCount(filePath);
	vector<string> ids = ShapefileReader::GetFieldValues(filePath,"FID");
	vector<string> areas = ShapefileReader::GetFieldValues(filePath,"SHAPE_Area");
	vector<string> landUseCodes = ShapefileReader::GetFieldValues(filePath,"OldUseCode");

	vector<LandUsePolygon*> polygons;
	for (int i=0; i != featureCount; ++i)
	{
		int id = DatatypeOperator::stringToint(ids.at(i));
		double area = DatatypeOperator::stringTodouble(areas.at(i)) / 10000;
		int landUseCode = DatatypeOperator::stringToint(landUseCodes.at(i));
		LandUsePolygon* polygon = new LandUsePolygon(id, area, landUseCode);
		polygons.push_back(polygon);
	}

	DatatypeOperator::DestroyVector(ids);
	DatatypeOperator::DestroyVector(areas);
	DatatypeOperator::DestroyVector(landUseCodes);

	LandUseLayer layer(_useCodeNum, polygons);
	DatatypeOperator::DestroyVector(polygons);

	vector<double> UseAreas = layer.UseAreas();
	double totalArea = layer.TotalArea();

	ui->SpinTotalPolygon->setValue(featureCount);
	ui->SpinTotalArea->setValue(totalArea);

	ui->TxtGD_2->setValue(UseAreas.at(0));
	ui->TxtYD_2->setValue(UseAreas.at(1));
	ui->TxtLD_2->setValue(UseAreas.at(2));
	ui->TxtMCD_2->setValue(UseAreas.at(3));
	ui->TxtQTNYD_2->setValue(UseAreas.at(4));
	ui->TxtCXJSYD_2->setValue(UseAreas.at(5));
	ui->TxtJTSLYD_2->setValue(UseAreas.at(6));
	ui->TxtQTJSYD_2->setValue(UseAreas.at(7));
	ui->TxtSY_2->setValue(UseAreas.at(8));
	ui->TxtTTZZ_2->setValue(UseAreas.at(9));
	ui->TxtQTWLYTD_2->setValue(UseAreas.at(10));	
}


void MainForm::setBenefitWeight(int value)
{
	ui->TxtEcoTemp->setValue(value / 100.0);
}


void MainForm::setChangeCostWeight(int value)
{
	ui->TxtCostTemp->setValue(value / 100.0);
}


void MainForm::setSuitabilityWeight(int value)
{
	ui->TxtSuitTemp->setValue(value / 100.0);
}


void MainForm::setCompactnessWeight(int value)
{
	ui->TxtCompTemp->setValue(value / 100.0);
}


void MainForm::OpenSuitFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,"Open",QDir::currentPath(),"shp files (*.shp)");
	
	if (!fileName.isNull())	// 用户选择了文件
	{ 
		ui->TxtSuitFile->setText(fileName);
	}
}


void MainForm::StartPSO()
{

	ui->stackedWidget->setCurrentIndex(1);

	// 1、建立LandUseLayer来描述shape文件
	string filePath(ui->TxtPath->text().toStdString());
	int featureCount = ShapefileReader::GetFeatureCount(filePath);
	vector<string> ids = ShapefileReader::GetFieldValues(filePath,"FID");
	vector<string> areas = ShapefileReader::GetFieldValues(filePath,"SHAPE_Area");
	vector<string> landUseCodes = ShapefileReader::GetFieldValues(filePath,"OldUseCode");

	vector<LandUsePolygon*> polygons;
	for (int i=0; i != featureCount; ++i)
	{
		int id = DatatypeOperator::stringToint(ids.at(i));
		double area = DatatypeOperator::stringTodouble(areas.at(i)) / 10000;
		int landUseCode = DatatypeOperator::stringToint(landUseCodes.at(i));
		LandUsePolygon* polygon = new LandUsePolygon(id, area, landUseCode);
		polygons.push_back(polygon);
	}

	DatatypeOperator::DestroyVector(ids);
	DatatypeOperator::DestroyVector(areas);
	DatatypeOperator::DestroyVector(landUseCodes);

	LandUseLayer* layer = new LandUseLayer(_useCodeNum, polygons);
	DatatypeOperator::DestroyVector(polygons);

	// 经济效益
	vector<double> avgBenefits(11, 0);
	avgBenefits.at(0) = ui->TxtGD->value();
	avgBenefits.at(1) = ui->TxtYD->value();
	avgBenefits.at(2) = ui->TxtLD->value();
	avgBenefits.at(3) = ui->TxtMCD->value();
	avgBenefits.at(4) = ui->TxtQTNYD->value();
	avgBenefits.at(5) = ui->TxtCXJSYD->value();
	avgBenefits.at(6) = ui->TxtJTSLYD->value();
	avgBenefits.at(7) = ui->TxtQTJSYD->value();
	avgBenefits.at(8) = ui->TxtSY->value();
	avgBenefits.at(9) = ui->TxtTTZZ->value();
	avgBenefits.at(10) = ui->TxtQTWLYTD->value();

	layer->AvgBenefits = avgBenefits;
	DatatypeOperator::DestroyVector(avgBenefits);


	// 变更费用
	vector<double> avgChangeCosts(11 * 11, 0);
	for (int i=0; i != 11; ++i)
	{
		for (int j=0; j != 11; ++j)
		{
			QString value = ui->tableWidget->itemAt(i,j)->text();
			double changeCost = value.toDouble();
			avgChangeCosts.at(i * 11 + j) = changeCost;
		}
	}

	layer->AvgChangeCosts = avgChangeCosts;
	DatatypeOperator::DestroyVector(avgChangeCosts);


	// 适宜性评价
	vector<double> avgSuitabilities(featureCount * 11, 0);
	string suitFilePath(ui->TxtSuitFile->text().toStdString());
	if (!suitFilePath.empty())
	{
	if (ui->ChkGD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "GDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "YDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 1) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkLD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "LDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 2) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkMCD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "MCDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 3) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkQTNYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "QTNYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 4) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkCXJSYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "CXJSYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 5) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkJTSLYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "JTSLYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 6) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkQTJSYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "QTJSYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 7) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkSY->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "SYSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 8) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkTTZZ->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "TTZZSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 9) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	if (ui->ChkQTWLYTD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "QTWLYTDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 10) = DatatypeOperator::stringTodouble(values.at(i));
		}
	}
	}

	layer->AvgSuitabilities = avgSuitabilities;
	DatatypeOperator::DestroyVector(avgSuitabilities);


	// 空间紧凑度
	vector<vector<int>> avgCompactnesses(featureCount,vector<int>()); //= ShapefileReader::GetAdjacency(filePath);

	layer->AvgCompactnesses = avgCompactnesses;
	DatatypeOperator::DestroyVector(avgCompactnesses);


	LayerAssessor* layerAssessor = new LayerAssessor(layer);
	double benefitWeight = ui->TxtEcoTemp->value();
	double changeCostWeight = ui->TxtCostTemp->value();
	double suitabilityWeight = ui->TxtSuitTemp->value();
	double compactnessWeight = ui->TxtCompTemp->value();
	double sum = benefitWeight + changeCostWeight + suitabilityWeight + compactnessWeight;
	if (sum != 0)
	{
		layerAssessor->BenefitWeight = benefitWeight / sum;
		layerAssessor->ChangeCostWeight = changeCostWeight / sum;
		layerAssessor->SuitabilityWeight = suitabilityWeight / sum;
		layerAssessor->CompactnessWeight = compactnessWeight / sum;
	}

	layerAssessor->MaxBenefit = layer->MaxBenefit();
	layerAssessor->MinBenefit = layer->MinBenefit();
	layerAssessor->MaxChangeCost = layer->MaxChangeCost();
	layerAssessor->MinChangeCost = layer->MinChangeCost();
	layerAssessor->MaxSuitability = layer->MaxSuitability();
	layerAssessor->MinSuitability = layer->MinSuitability();
	layerAssessor->MaxCompactness = layer->MaxCompactness();
	layerAssessor->MinCompactness = layer->MinCompactness();


	SpatialFunctionBase* function = new SpatialFunctionBase(layerAssessor);
	int swarmSize = ui->spinIteration->value();
	int dimension = featureCount;
	int range = 11;
	SpatialParticleSwarm* swarm = new SpatialParticleSwarm(function, swarmSize, dimension, range);
	swarm->PercentMaximumVelocityOfSearchSpace = ui->spinSearch->value();
	swarm->UseGlobalOptimum = ui->chkUseGlobal->isChecked();
	swarm->TendencyToOwnBest = ui->dspinC1->value();
	swarm->TendencyToGlobalBest = ui->dspinC2->value();

	int maxIteration = ui->spinIteration->value();

	ui->prgBenefit->setMaximum((int)layerAssessor->MaxBenefit);
	ui->prgBenefit->setMinimum((int)layerAssessor->MinBenefit);
	ui->prgChangeCost->setMaximum((int)layerAssessor->MaxChangeCost);
	ui->prgSuitability->setMaximum((int)layerAssessor->MaxSuitability);
	ui->prgSuitability->setMinimum((int)layerAssessor->MinSuitability);	
	ui->prgCompactness->setMaximum((int)layerAssessor->MaxCompactness);

	vector<double> results;
	double oldBestCost = 1.0e+100;
	double sameCount = 0;
	for (int i = 0; i != maxIteration; ++i)
	{
		swarm->Iteration();
		results.assign(swarm->BestPosition.begin(), swarm->BestPosition.end());
		double newBestCost = swarm->BestCost;		

		ui->prgBenefit->setValue((int)(layerAssessor->TotalBenefit()));
		ui->prgChangeCost->setValue((int)(layerAssessor->TotalChangeCost()));
		ui->prgSuitability->setValue((int)(layerAssessor->TotalSuitability()));
		ui->prgCompactness->setValue((int)(layerAssessor->TotalCompactness()));
		ui->prgTotalScore->setValue((int)(layerAssessor->TotalScore()));
		ui->lcdIterationNum->display(i+1);

		if (fabs(newBestCost - oldBestCost) < 0.01 )
		{
			sameCount++;
		}
		else
		{
			sameCount = 0;
		}
		oldBestCost = newBestCost;
		if (sameCount == 20)
		{			
			break;
		}

	}
	
	vector<string> values;	
	vector<double>::iterator iter;
	for (iter=results.begin(); iter != results.end(); ++iter)
	{
		values.push_back(DatatypeOperator::ConvertTostring((int)(*iter)));
	}

	//bool isSuccess = ShapefileWriter::WriteToFile(filePath, "NewUseCode", values);
	
	delete layer;
	delete layerAssessor;
	delete swarm;
	delete function;

	QMessageBox::information(this, "SpatialPSO","The Calculation is complete!");
	
}