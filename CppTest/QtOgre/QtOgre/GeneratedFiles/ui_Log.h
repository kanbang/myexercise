/********************************************************************************
** Form generated from reading UI file 'Log.ui'
**
** Created: Mon Apr 22 16:10:33 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOG_H
#define UI_LOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Log
{
public:
    QGridLayout *gridLayout;
    QPushButton *showErrorsButton;
    QPushButton *showWarningsButton;
    QTableWidget *logTable;
    QSpacerItem *horizontalSpacer;
    QPushButton *showInformationButton;
    QPushButton *showDebugButton;
    QLabel *label;
    QPushButton *clearFilterButton;
    QLineEdit *filterLineEdit;

    void setupUi(QWidget *Log)
    {
        if (Log->objectName().isEmpty())
            Log->setObjectName(QString::fromUtf8("Log"));
        Log->resize(500, 400);
        gridLayout = new QGridLayout(Log);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        showErrorsButton = new QPushButton(Log);
        showErrorsButton->setObjectName(QString::fromUtf8("showErrorsButton"));
        showErrorsButton->setMinimumSize(QSize(85, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icons/dialog-error.svg"), QSize(), QIcon::Normal, QIcon::Off);
        showErrorsButton->setIcon(icon);
        showErrorsButton->setIconSize(QSize(16, 16));
        showErrorsButton->setCheckable(true);
        showErrorsButton->setChecked(true);

        gridLayout->addWidget(showErrorsButton, 0, 0, 1, 1);

        showWarningsButton = new QPushButton(Log);
        showWarningsButton->setObjectName(QString::fromUtf8("showWarningsButton"));
        showWarningsButton->setMinimumSize(QSize(85, 20));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/icons/dialog-warning.svg"), QSize(), QIcon::Normal, QIcon::Off);
        showWarningsButton->setIcon(icon1);
        showWarningsButton->setIconSize(QSize(16, 16));
        showWarningsButton->setCheckable(true);
        showWarningsButton->setChecked(true);

        gridLayout->addWidget(showWarningsButton, 0, 1, 1, 1);

        logTable = new QTableWidget(Log);
        if (logTable->columnCount() < 3)
            logTable->setColumnCount(3);
        logTable->setObjectName(QString::fromUtf8("logTable"));
        logTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        logTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        logTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        logTable->setShowGrid(false);
        logTable->setWordWrap(false);
        logTable->setCornerButtonEnabled(false);
        logTable->setRowCount(0);
        logTable->setColumnCount(3);

        gridLayout->addWidget(logTable, 1, 0, 1, 9);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

        showInformationButton = new QPushButton(Log);
        showInformationButton->setObjectName(QString::fromUtf8("showInformationButton"));
        showInformationButton->setMinimumSize(QSize(85, 20));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/icons/dialog-information.svg"), QSize(), QIcon::Normal, QIcon::Off);
        showInformationButton->setIcon(icon2);
        showInformationButton->setIconSize(QSize(16, 16));
        showInformationButton->setCheckable(true);
        showInformationButton->setChecked(true);
        showInformationButton->setFlat(false);

        gridLayout->addWidget(showInformationButton, 0, 2, 1, 1);

        showDebugButton = new QPushButton(Log);
        showDebugButton->setObjectName(QString::fromUtf8("showDebugButton"));
        showDebugButton->setMinimumSize(QSize(85, 20));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/icons/script-error.svg"), QSize(), QIcon::Normal, QIcon::Off);
        showDebugButton->setIcon(icon3);
        showDebugButton->setIconSize(QSize(16, 16));
        showDebugButton->setCheckable(true);

        gridLayout->addWidget(showDebugButton, 0, 3, 1, 1);

        label = new QLabel(Log);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 5, 1, 1);

        clearFilterButton = new QPushButton(Log);
        clearFilterButton->setObjectName(QString::fromUtf8("clearFilterButton"));
        clearFilterButton->setEnabled(false);
        clearFilterButton->setMinimumSize(QSize(20, 20));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/icons/edit-clear-locationbar-rtl.svg"), QSize(), QIcon::Normal, QIcon::Off);
        clearFilterButton->setIcon(icon4);
        clearFilterButton->setIconSize(QSize(16, 16));
        clearFilterButton->setFlat(true);

        gridLayout->addWidget(clearFilterButton, 0, 7, 1, 1);

        filterLineEdit = new QLineEdit(Log);
        filterLineEdit->setObjectName(QString::fromUtf8("filterLineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(filterLineEdit->sizePolicy().hasHeightForWidth());
        filterLineEdit->setSizePolicy(sizePolicy);
        filterLineEdit->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(filterLineEdit, 0, 6, 1, 1);


        retranslateUi(Log);

        QMetaObject::connectSlotsByName(Log);
    } // setupUi

    void retranslateUi(QWidget *Log)
    {
        Log->setWindowTitle(QApplication::translate("Log", "Log Viewer", 0, QApplication::UnicodeUTF8));
        showErrorsButton->setText(QApplication::translate("Log", "Errors", 0, QApplication::UnicodeUTF8));
        showWarningsButton->setText(QApplication::translate("Log", "Warnings", 0, QApplication::UnicodeUTF8));
        showInformationButton->setText(QApplication::translate("Log", "Information", 0, QApplication::UnicodeUTF8));
        showDebugButton->setText(QApplication::translate("Log", "Debug", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Log", "Filter: ", 0, QApplication::UnicodeUTF8));
        clearFilterButton->setText(QString());
        filterLineEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Log: public Ui_Log {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOG_H
