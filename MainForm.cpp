#include "MainForm.h"
#include "ui_MainForm.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "ogrsf_frmts.h"
#include "LandUseLayer.h"
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
	string filePath(path.toStdString());

	// 1、建立LandUseLayer来描述shape文件
	int featureCount = ShapefileReader::GetFeatureCount(filePath);
	vector<string> ids = ShapefileReader::GetFieldValues(filePath,"FID");
	vector<string> areas = ShapefileReader::GetFieldValues(filePath,"SHAPE_Area");
	vector<string> landUseCodes = ShapefileReader::GetFieldValues(filePath,"OldUseCode");

	vector<LandUsePolygon*> polygons;
	for (int i=0; i != featureCount; ++i)
	{
		int id = atoi(ids.at(i).c_str());
		double area = atof(areas.at(i).c_str()) / 10000;
		int landUseCode = atoi(landUseCodes.at(i).c_str());
		LandUsePolygon* polygon = new LandUsePolygon(id, area, landUseCode);
		polygons.push_back(polygon);
	}

	LandUseLayer layer(_useCodeNum, polygons);
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
		int id = atoi(ids.at(i).c_str());
		double area = atof(areas.at(i).c_str()) / 10000;
		int landUseCode = atoi(landUseCodes.at(i).c_str());
		LandUsePolygon* polygon = new LandUsePolygon(id, area, landUseCode);
		polygons.push_back(polygon);
	}

	LandUseLayer* layer = new LandUseLayer(11, polygons);

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


	// 适宜性评价
	vector<double> avgSuitabilities(featureCount * 11, 0);
	string suitFilePath(ui->TxtSuitFile->text().toStdString());
	if (ui->ChkGD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "GDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "YDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 1) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkLD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "LDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 2) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkMCD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "MCDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 3) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkQTNYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "QTNYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 4) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkCXJSYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "CXJSYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 5) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkJTSLYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "JTSLYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 6) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkQTJSYD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "QTJSYDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 7) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkSY->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "SYSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 8) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkTTZZ->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "TTZZSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 9) = atof(values.at(i).c_str());
		}
	}
	if (ui->ChkQTWLYTD->isChecked())
	{		
		vector<string> values = ShapefileReader::GetFieldValues(suitFilePath, "QTWLYTDSYX");
		for (int i=0; i != values.size(); ++i)
		{
			avgSuitabilities.at(i * 11 + 10) = atof(values.at(i).c_str());
		}
	}

	// 空间紧凑度
	vector<vector<int>> avgCompactnesses = ShapefileReader::GetAdjacency(filePath);

	layer->SetAvgBenefits(avgBenefits);
	layer->SetAvgChangeCosts(avgChangeCosts);
	layer->SetAvgSuitabilities(avgSuitabilities);
	layer->SetAvgCompactnesses(avgCompactnesses);

	LayerAssessor* layerAssessor = new LayerAssessor(layer);
	double benefitWeight = ui->TxtEcoTemp->value();
	double changeCostWeight = ui->TxtCostTemp->value();
	double suitabilityWeight = ui->TxtSuitTemp->value();
	double compactnessWeight = ui->TxtCompTemp->value();
	double sum = benefitWeight + changeCostWeight + suitabilityWeight + compactnessWeight;
	layerAssessor->BenefitWeight = benefitWeight / sum;
	layerAssessor->ChangeCostWeight = changeCostWeight / sum;
	layerAssessor->SuitabilityWeight = suitabilityWeight / sum;
	layerAssessor->CompactnessWeight = compactnessWeight / sum;

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

	double maxBenefit = layer->MaxBenefit();
	double maxChangeCost = layer->MaxChangeCost();
	double minSuitability = layer->MinSuitability();
	double maxSuitability = layer->MaxSuitability();
	double maxCompactness = layer->MaxCompactness();
	ui->prgBenefit->setMaximum((int)maxBenefit);
	ui->prgChangeCost->setMaximum((int)maxChangeCost);
	ui->prgSuitability->setMinimum((int)minSuitability);
	ui->prgSuitability->setMaximum((int)maxSuitability);
	ui->prgCompactness->setMaximum((int)maxCompactness);

	vector<int> results;
	double oldBestCost = 1.0e+100;
	double sameCount = 0;
	for (int i = 0; i != maxIteration; ++i)
	{
		swarm->Iteration();
		vector<int> temp;
		vector<double>::iterator iter;
		for (iter=swarm->BestPosition.end(); iter != swarm->BestPosition.end(); ++iter)
		{
			temp.push_back((int)(*iter));

		}
		results.assign(temp.begin(), temp.end());


		double newBestCost = swarm->BestCost;		

		ui->prgBenefit->setValue((int)(layerAssessor->BenefitScore()));
		ui->prgChangeCost->setValue((int)(layerAssessor->ChangeCostScore()));
		ui->prgSuitability->setValue((int)(layerAssessor->SuitabilityScore()));
		ui->prgCompactness->setValue((int)(layerAssessor->CompactnessScore()));
		ui->prgTotalScore->setValue((int)(100-layerAssessor->TotalScore()));
		ui->lcdIterationNum->display(i+1);

		if (fabs(newBestCost - oldBestCost) < 0.01 )
		{
			sameCount++;
		}
		if (sameCount == 20)
		{			
			break;
		}

	}
	
	vector<string> values;	
	vector<int>::iterator iter;
	for (iter=results.begin(); iter != results.end(); ++iter)
	{
		stringstream strStream;
		strStream<<*iter;
		values.push_back(strStream.str());
	}

	bool isSuccess = ShapefileWriter::WriteToFile(filePath, "NewUseCode", values);
	
	delete layer;
	delete layerAssessor;
	delete swarm;
	delete function;
	
}