/********************************************************************************
** Form generated from reading UI file 'SettingDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGDLG_H
#define UI_SETTINGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingDlg
{
public:
    QPushButton *renderPushButton;
    QPushButton *savePushButton;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *timeLabel;
    QLineEdit *timeLineEdit;

    void setupUi(QDialog *SettingDlg)
    {
        if (SettingDlg->objectName().isEmpty())
            SettingDlg->setObjectName(QStringLiteral("SettingDlg"));
        SettingDlg->resize(240, 320);
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(12);
        SettingDlg->setFont(font);
        renderPushButton = new QPushButton(SettingDlg);
        renderPushButton->setObjectName(QStringLiteral("renderPushButton"));
        renderPushButton->setGeometry(QRect(80, 160, 81, 31));
        savePushButton = new QPushButton(SettingDlg);
        savePushButton->setObjectName(QStringLiteral("savePushButton"));
        savePushButton->setGeometry(QRect(60, 210, 141, 41));
        widget = new QWidget(SettingDlg);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 260, 212, 29));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        timeLabel = new QLabel(widget);
        timeLabel->setObjectName(QStringLiteral("timeLabel"));

        horizontalLayout->addWidget(timeLabel);

        timeLineEdit = new QLineEdit(widget);
        timeLineEdit->setObjectName(QStringLiteral("timeLineEdit"));

        horizontalLayout->addWidget(timeLineEdit);


        retranslateUi(SettingDlg);

        QMetaObject::connectSlotsByName(SettingDlg);
    } // setupUi

    void retranslateUi(QDialog *SettingDlg)
    {
        SettingDlg->setWindowTitle(QApplication::translate("SettingDlg", "Dialog", nullptr));
        renderPushButton->setText(QApplication::translate("SettingDlg", "Render", nullptr));
        savePushButton->setText(QApplication::translate("SettingDlg", "Save To Image", nullptr));
        timeLabel->setText(QApplication::translate("SettingDlg", "Time:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingDlg: public Ui_SettingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGDLG_H
