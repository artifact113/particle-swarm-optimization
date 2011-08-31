/********************************************************************************
** Form generated from reading UI file 'formmain.ui'
**
** Created: Wed Aug 31 21:37:16 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMMAIN_H
#define UI_FORMMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FormMain
{
public:
    QPushButton *btnClose;

    void setupUi(QDialog *FormMain)
    {
        if (FormMain->objectName().isEmpty())
            FormMain->setObjectName(QString::fromUtf8("FormMain"));
        FormMain->resize(400, 300);
        btnClose = new QPushButton(FormMain);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setGeometry(QRect(160, 130, 75, 23));

        retranslateUi(FormMain);
        QObject::connect(btnClose, SIGNAL(clicked()), FormMain, SLOT(close()));

        QMetaObject::connectSlotsByName(FormMain);
    } // setupUi

    void retranslateUi(QDialog *FormMain)
    {
        FormMain->setWindowTitle(QApplication::translate("FormMain", "FormMain", 0, QApplication::UnicodeUTF8));
        btnClose->setText(QApplication::translate("FormMain", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormMain: public Ui_FormMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMAIN_H
