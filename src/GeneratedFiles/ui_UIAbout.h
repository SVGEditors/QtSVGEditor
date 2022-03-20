/********************************************************************************
** Form generated from reading UI file 'UIAbout.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIABOUT_H
#define UI_UIABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_UIAbout
{
public:
    QPushButton *ButtonOK;
    QPushButton *ButtonIcon;

    void setupUi(QDialog *UIAbout)
    {
        if (UIAbout->objectName().isEmpty())
            UIAbout->setObjectName(QStringLiteral("UIAbout"));
        UIAbout->resize(501, 179);
        ButtonOK = new QPushButton(UIAbout);
        ButtonOK->setObjectName(QStringLiteral("ButtonOK"));
        ButtonOK->setGeometry(QRect(370, 120, 101, 31));
        ButtonIcon = new QPushButton(UIAbout);
        ButtonIcon->setObjectName(QStringLiteral("ButtonIcon"));
        ButtonIcon->setGeometry(QRect(40, 40, 32, 32));

        retranslateUi(UIAbout);

        QMetaObject::connectSlotsByName(UIAbout);
    } // setupUi

    void retranslateUi(QDialog *UIAbout)
    {
        UIAbout->setWindowTitle(QApplication::translate("UIAbout", "\345\205\263\344\272\216", 0));
        ButtonOK->setText(QApplication::translate("UIAbout", "OK", 0));
        ButtonIcon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class UIAbout: public Ui_UIAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIABOUT_H
