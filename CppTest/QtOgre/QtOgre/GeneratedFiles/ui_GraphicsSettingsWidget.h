/********************************************************************************
** Form generated from reading UI file 'GraphicsSettingsWidget.ui'
**
** Created: Mon Apr 22 16:10:33 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICSSETTINGSWIDGET_H
#define UI_GRAPHICSSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicsSettingsWidget
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_5;
    QRadioButton *mOpenGLRadioButton;
    QSpacerItem *horizontalSpacer_6;
    QRadioButton *mDirect3D9RadioButton;
    QSpacerItem *horizontalSpacer_7;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QComboBox *mWindowModeComboBox;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QCheckBox *mAllowPerfHUDCheckBox;
    QSpacerItem *horizontalSpacer;
    QCheckBox *mEnableGammaCorrectionCheckBox;
    QSpacerItem *horizontalSpacer_2;
    QSpinBox *mFSSAFactorSpinBox;
    QLabel *mFSSAFactorLabel;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *mEnableVerticalSyncCheckBox;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *GraphicsSettingsWidget)
    {
        if (GraphicsSettingsWidget->objectName().isEmpty())
            GraphicsSettingsWidget->setObjectName(QString::fromUtf8("GraphicsSettingsWidget"));
        GraphicsSettingsWidget->resize(407, 272);
        gridLayout_4 = new QGridLayout(GraphicsSettingsWidget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        groupBox = new QGroupBox(GraphicsSettingsWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        mOpenGLRadioButton = new QRadioButton(groupBox);
        mOpenGLRadioButton->setObjectName(QString::fromUtf8("mOpenGLRadioButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logos/OpenGL.png"), QSize(), QIcon::Normal, QIcon::Off);
        mOpenGLRadioButton->setIcon(icon);
        mOpenGLRadioButton->setIconSize(QSize(64, 64));
        mOpenGLRadioButton->setChecked(false);

        horizontalLayout->addWidget(mOpenGLRadioButton);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        mDirect3D9RadioButton = new QRadioButton(groupBox);
        mDirect3D9RadioButton->setObjectName(QString::fromUtf8("mDirect3D9RadioButton"));
        mDirect3D9RadioButton->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/logos/DirectX9.png"), QSize(), QIcon::Normal, QIcon::Off);
        mDirect3D9RadioButton->setIcon(icon1);
        mDirect3D9RadioButton->setIconSize(QSize(64, 64));
        mDirect3D9RadioButton->setChecked(false);

        horizontalLayout->addWidget(mDirect3D9RadioButton);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(GraphicsSettingsWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        mWindowModeComboBox = new QComboBox(groupBox_2);
        mWindowModeComboBox->setObjectName(QString::fromUtf8("mWindowModeComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mWindowModeComboBox->sizePolicy().hasHeightForWidth());
        mWindowModeComboBox->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(mWindowModeComboBox, 0, 1, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(GraphicsSettingsWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        mAllowPerfHUDCheckBox = new QCheckBox(groupBox_3);
        mAllowPerfHUDCheckBox->setObjectName(QString::fromUtf8("mAllowPerfHUDCheckBox"));
        mAllowPerfHUDCheckBox->setEnabled(false);
        mAllowPerfHUDCheckBox->setIconSize(QSize(16, 16));

        gridLayout_3->addWidget(mAllowPerfHUDCheckBox, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(1, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);

        mEnableGammaCorrectionCheckBox = new QCheckBox(groupBox_3);
        mEnableGammaCorrectionCheckBox->setObjectName(QString::fromUtf8("mEnableGammaCorrectionCheckBox"));

        gridLayout_3->addWidget(mEnableGammaCorrectionCheckBox, 0, 3, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(1, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 5, 1, 1);

        mFSSAFactorSpinBox = new QSpinBox(groupBox_3);
        mFSSAFactorSpinBox->setObjectName(QString::fromUtf8("mFSSAFactorSpinBox"));
        mFSSAFactorSpinBox->setMaximum(8);
        mFSSAFactorSpinBox->setSingleStep(2);

        gridLayout_3->addWidget(mFSSAFactorSpinBox, 1, 0, 1, 1);

        mFSSAFactorLabel = new QLabel(groupBox_3);
        mFSSAFactorLabel->setObjectName(QString::fromUtf8("mFSSAFactorLabel"));

        gridLayout_3->addWidget(mFSSAFactorLabel, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(1, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        mEnableVerticalSyncCheckBox = new QCheckBox(groupBox_3);
        mEnableVerticalSyncCheckBox->setObjectName(QString::fromUtf8("mEnableVerticalSyncCheckBox"));

        gridLayout_3->addWidget(mEnableVerticalSyncCheckBox, 1, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(25, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 1, 4, 1, 2);


        gridLayout_4->addWidget(groupBox_3, 2, 0, 1, 1);


        retranslateUi(GraphicsSettingsWidget);

        QMetaObject::connectSlotsByName(GraphicsSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *GraphicsSettingsWidget)
    {
        GraphicsSettingsWidget->setWindowTitle(QApplication::translate("GraphicsSettingsWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GraphicsSettingsWidget", "Rendering Subsystem", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mOpenGLRadioButton->setToolTip(QApplication::translate("GraphicsSettingsWidget", "OpenGL Rendering Subsystem", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mOpenGLRadioButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        mDirect3D9RadioButton->setToolTip(QApplication::translate("GraphicsSettingsWidget", "Direct3D9 Rendering Subsystem", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mDirect3D9RadioButton->setText(QString());
        groupBox_2->setTitle(QApplication::translate("GraphicsSettingsWidget", "Window", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GraphicsSettingsWidget", "Mode: ", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("GraphicsSettingsWidget", "Advanced Settings", 0, QApplication::UnicodeUTF8));
        mAllowPerfHUDCheckBox->setText(QApplication::translate("GraphicsSettingsWidget", "Allow PerfHUD", 0, QApplication::UnicodeUTF8));
        mEnableGammaCorrectionCheckBox->setText(QApplication::translate("GraphicsSettingsWidget", "Enable Gamma Correction", 0, QApplication::UnicodeUTF8));
        mFSSAFactorLabel->setText(QApplication::translate("GraphicsSettingsWidget", "FSAA Factor", 0, QApplication::UnicodeUTF8));
        mEnableVerticalSyncCheckBox->setText(QApplication::translate("GraphicsSettingsWidget", "Enable Vertical Sync", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GraphicsSettingsWidget: public Ui_GraphicsSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICSSETTINGSWIDGET_H
