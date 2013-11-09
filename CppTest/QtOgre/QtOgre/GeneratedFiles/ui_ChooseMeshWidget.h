/********************************************************************************
** Form generated from reading UI file 'ChooseMeshWidget.ui'
**
** Created: Mon Apr 22 16:10:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSEMESHWIDGET_H
#define UI_CHOOSEMESHWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChooseMeshWidget
{
public:
    QGridLayout *gridLayout;
    QRadioButton *mJaiquaRadioButton;
    QRadioButton *mRobotRadioButton;

    void setupUi(QWidget *ChooseMeshWidget)
    {
        if (ChooseMeshWidget->objectName().isEmpty())
            ChooseMeshWidget->setObjectName(QString::fromUtf8("ChooseMeshWidget"));
        ChooseMeshWidget->resize(126, 60);
        gridLayout = new QGridLayout(ChooseMeshWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mJaiquaRadioButton = new QRadioButton(ChooseMeshWidget);
        mJaiquaRadioButton->setObjectName(QString::fromUtf8("mJaiquaRadioButton"));
        mJaiquaRadioButton->setChecked(true);

        gridLayout->addWidget(mJaiquaRadioButton, 0, 0, 1, 1);

        mRobotRadioButton = new QRadioButton(ChooseMeshWidget);
        mRobotRadioButton->setObjectName(QString::fromUtf8("mRobotRadioButton"));

        gridLayout->addWidget(mRobotRadioButton, 1, 0, 1, 1);


        retranslateUi(ChooseMeshWidget);

        QMetaObject::connectSlotsByName(ChooseMeshWidget);
    } // setupUi

    void retranslateUi(QWidget *ChooseMeshWidget)
    {
        ChooseMeshWidget->setWindowTitle(QApplication::translate("ChooseMeshWidget", "Mesh", 0, QApplication::UnicodeUTF8));
        mJaiquaRadioButton->setText(QApplication::translate("ChooseMeshWidget", "Jaiqua", 0, QApplication::UnicodeUTF8));
        mRobotRadioButton->setText(QApplication::translate("ChooseMeshWidget", "Robot", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ChooseMeshWidget: public Ui_ChooseMeshWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSEMESHWIDGET_H
