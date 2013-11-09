/********************************************************************************
** Form generated from reading UI file 'MainMenu.ui'
**
** Created: Mon Apr 22 16:10:32 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINMENU_H
#define UI_MAINMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MainMenu
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *mResumeButton;
    QPushButton *mSettingsButton;
    QPushButton *mViewLogsButton;
    QPushButton *mQuitButton;

    void setupUi(QDialog *MainMenu)
    {
        if (MainMenu->objectName().isEmpty())
            MainMenu->setObjectName(QString::fromUtf8("MainMenu"));
        MainMenu->resize(258, 308);
        verticalLayout = new QVBoxLayout(MainMenu);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(MainMenu);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/QtOgreLogo.png")));
        label->setScaledContents(true);

        verticalLayout->addWidget(label);

        mResumeButton = new QPushButton(MainMenu);
        mResumeButton->setObjectName(QString::fromUtf8("mResumeButton"));
        mResumeButton->setMinimumSize(QSize(0, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/go-previous.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mResumeButton->setIcon(icon);
        mResumeButton->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(mResumeButton);

        mSettingsButton = new QPushButton(MainMenu);
        mSettingsButton->setObjectName(QString::fromUtf8("mSettingsButton"));
        mSettingsButton->setEnabled(true);
        mSettingsButton->setMinimumSize(QSize(0, 40));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/preferences-system.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mSettingsButton->setIcon(icon1);
        mSettingsButton->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(mSettingsButton);

        mViewLogsButton = new QPushButton(MainMenu);
        mViewLogsButton->setObjectName(QString::fromUtf8("mViewLogsButton"));
        mViewLogsButton->setMinimumSize(QSize(0, 40));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/utilities-log-viewer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mViewLogsButton->setIcon(icon2);
        mViewLogsButton->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(mViewLogsButton);

        mQuitButton = new QPushButton(MainMenu);
        mQuitButton->setObjectName(QString::fromUtf8("mQuitButton"));
        mQuitButton->setMinimumSize(QSize(0, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/application-exit.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mQuitButton->setIcon(icon3);
        mQuitButton->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(mQuitButton);


        retranslateUi(MainMenu);

        QMetaObject::connectSlotsByName(MainMenu);
    } // setupUi

    void retranslateUi(QDialog *MainMenu)
    {
        MainMenu->setWindowTitle(QApplication::translate("MainMenu", "Main Menu", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        mResumeButton->setText(QApplication::translate("MainMenu", "Resume", 0, QApplication::UnicodeUTF8));
        mSettingsButton->setText(QApplication::translate("MainMenu", "Settings...", 0, QApplication::UnicodeUTF8));
        mViewLogsButton->setText(QApplication::translate("MainMenu", "View Logs...", 0, QApplication::UnicodeUTF8));
        mQuitButton->setText(QApplication::translate("MainMenu", "Quit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainMenu: public Ui_MainMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMENU_H
