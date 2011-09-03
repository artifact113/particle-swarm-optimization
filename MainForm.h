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
	void FoldOrExpand();		///	折叠或展开参数设置面板
	void ChangeWeight();		///	计算权重值



private:
    Ui::MainForm *ui;
};

#endif // MAINFORM_H