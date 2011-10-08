#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>

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
	void FoldOrExpand();		///	折叠或展开参数设置面板;
	void ChangeWeight();		///	计算权重值;
	void OpenFile();			///	打开文件;
	void UpdateInfo(const QString &);			/// 更新文件相关信息;
	void StartPSO();			/// 开始PSO计算



private:
    Ui::MainForm *ui;

	double minEverEco;		/// 标记单位面积经济效益最低值;
	double maxEverEco;		/// 标记单位面积经济效益最高值;
};

#endif // MAINFORM_H