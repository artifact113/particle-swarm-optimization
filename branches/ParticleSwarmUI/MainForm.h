#ifndef MAINFORM_H
#define MAINFORM_H

#include <string>
#include <vector>
#include <QDialog>
#include "LandUseLayer.h"
#include "SpatialPSO.h"

namespace Ui {
    class MainForm;
}

class MainForm : public QDialog
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

public slots:
	void FoldOrExpand();						///	折叠或展开参数设置面板;
	void OpenFile();							///	打开文件;
	void UpdateInfo(const QString &);			/// 更新文件相关信息;

	void setBenefitWeight(int value);
	void setChangeCostWeight(int value);
	void setSuitabilityWeight(int value);
	void setCompactnessWeight(int value);

	void StartPSO();							/// 开始PSO计算



private:
    Ui::MainForm *ui;

	static const int _useCodeNum = 11;
};

#endif // MAINFORM_H