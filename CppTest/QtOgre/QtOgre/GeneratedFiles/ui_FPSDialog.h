/********************************************************************************
** Form generated from reading UI file 'FPSDialog.ui'
**
** Created: Mon Apr 22 16:10:33 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FPSDIALOG_H
#define UI_FPSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>

QT_BEGIN_NAMESPACE

class Ui_FPSDialog
{
public:
    QGridLayout *gridLayout;
    QLCDNumber *mLcdNumber;

    void setupUi(QDialog *FPSDialog)
    {
        if (FPSDialog->objectName().isEmpty())
            FPSDialog->setObjectName(QString::fromUtf8("FPSDialog"));
        FPSDialog->resize(150, 100);
        gridLayout = new QGridLayout(FPSDialog);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mLcdNumber = new QLCDNumber(FPSDialog);
        mLcdNumber->setObjectName(QString::fromUtf8("mLcdNumber"));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 128, 128, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        QBrush brush2(QColor(128, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        QBrush brush3(QColor(0, 0, 0, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush3);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        QBrush brush4(QColor(255, 255, 255, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        mLcdNumber->setPalette(palette);
        mLcdNumber->setAutoFillBackground(true);
        mLcdNumber->setFrameShape(QFrame::Box);
        mLcdNumber->setFrameShadow(QFrame::Raised);
        mLcdNumber->setNumDigits(3);
        mLcdNumber->setSegmentStyle(QLCDNumber::Filled);

        gridLayout->addWidget(mLcdNumber, 0, 0, 1, 1);


        retranslateUi(FPSDialog);

        QMetaObject::connectSlotsByName(FPSDialog);
    } // setupUi

    void retranslateUi(QDialog *FPSDialog)
    {
        FPSDialog->setWindowTitle(QApplication::translate("FPSDialog", "Frames Per Second", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FPSDialog: public Ui_FPSDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FPSDIALOG_H
