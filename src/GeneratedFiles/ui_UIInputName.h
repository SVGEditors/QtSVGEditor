/********************************************************************************
** Form generated from reading UI file 'UIInputName.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIINPUTNAME_H
#define UI_UIINPUTNAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_UIINPUTNAME
{
public:
    QLabel *label;
    QLineEdit *lineEditName;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *UIINPUTNAME)
    {
        if (UIINPUTNAME->objectName().isEmpty())
            UIINPUTNAME->setObjectName(QStringLiteral("UIINPUTNAME"));
        UIINPUTNAME->resize(350, 139);
        label = new QLabel(UIINPUTNAME);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 50, 41, 16));
        lineEditName = new QLineEdit(UIINPUTNAME);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setGeometry(QRect(70, 40, 261, 31));
        pushButtonOk = new QPushButton(UIINPUTNAME);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonOk"));
        pushButtonOk->setGeometry(QRect(130, 100, 93, 28));
        pushButtonCancel = new QPushButton(UIINPUTNAME);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(240, 100, 93, 28));

        retranslateUi(UIINPUTNAME);

        QMetaObject::connectSlotsByName(UIINPUTNAME);
    } // setupUi

    void retranslateUi(QDialog *UIINPUTNAME)
    {
        UIINPUTNAME->setWindowTitle(QApplication::translate("UIINPUTNAME", "\350\276\223\345\205\245\345\220\215\347\247\260", 0));
        label->setText(QApplication::translate("UIINPUTNAME", "\345\220\215\347\247\260:", 0));
        pushButtonOk->setText(QApplication::translate("UIINPUTNAME", "\347\241\256\345\256\232", 0));
        pushButtonCancel->setText(QApplication::translate("UIINPUTNAME", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class UIINPUTNAME: public Ui_UIINPUTNAME {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIINPUTNAME_H
