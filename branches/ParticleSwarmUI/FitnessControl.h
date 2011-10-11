#ifndef FITNESSCONTROL_H
#define FITNESSCONTROL_H

#include <string>
#include <vector>
#include <QWidget>

namespace Ui {
    class FitnessControl;
}

class FitnessControl : public QWidget
{
    Q_OBJECT

public:
    explicit FitnessControl(QWidget *parent = 0);
    ~FitnessControl();

public slots:


private:
	Ui::FitnessControl *ui;
};

#endif // MAINFORM_H