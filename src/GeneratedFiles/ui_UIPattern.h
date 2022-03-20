/********************************************************************************
** Form generated from reading UI file 'UIPattern.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIPATTERN_H
#define UI_UIPATTERN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_UIPattern
{
public:
    QGroupBox *groupBox;
    QPushButton *pushButtonPreview;
    QGroupBox *groupBox_2;
    QPushButton *pushButton1;
    QPushButton *pushButton2;
    QPushButton *pushButton3;
    QPushButton *pushButton4;
    QPushButton *pushButton5;
    QPushButton *pushButton7;
    QPushButton *pushButton8;
    QPushButton *pushButton6;
    QPushButton *pushButton10;
    QPushButton *pushButton9;
    QPushButton *pushButton12;
    QPushButton *pushButton11;
    QGroupBox *groupBox_3;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButtonForeColor;
    QPushButton *pushButtonBackColor;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *UIPattern)
    {
        if (UIPattern->objectName().isEmpty())
            UIPattern->setObjectName(QStringLiteral("UIPattern"));
        UIPattern->resize(571, 354);
        groupBox = new QGroupBox(UIPattern);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 241, 241));
        pushButtonPreview = new QPushButton(groupBox);
        pushButtonPreview->setObjectName(QStringLiteral("pushButtonPreview"));
        pushButtonPreview->setGeometry(QRect(20, 30, 200, 200));
        groupBox_2 = new QGroupBox(UIPattern);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(270, 20, 271, 201));
        pushButton1 = new QPushButton(groupBox_2);
        pushButton1->setObjectName(QStringLiteral("pushButton1"));
        pushButton1->setGeometry(QRect(10, 30, 40, 40));
        pushButton2 = new QPushButton(groupBox_2);
        pushButton2->setObjectName(QStringLiteral("pushButton2"));
        pushButton2->setGeometry(QRect(80, 30, 40, 40));
        pushButton3 = new QPushButton(groupBox_2);
        pushButton3->setObjectName(QStringLiteral("pushButton3"));
        pushButton3->setGeometry(QRect(150, 30, 40, 40));
        pushButton4 = new QPushButton(groupBox_2);
        pushButton4->setObjectName(QStringLiteral("pushButton4"));
        pushButton4->setGeometry(QRect(220, 30, 40, 40));
        pushButton5 = new QPushButton(groupBox_2);
        pushButton5->setObjectName(QStringLiteral("pushButton5"));
        pushButton5->setGeometry(QRect(10, 90, 40, 40));
        pushButton7 = new QPushButton(groupBox_2);
        pushButton7->setObjectName(QStringLiteral("pushButton7"));
        pushButton7->setGeometry(QRect(150, 90, 40, 40));
        pushButton8 = new QPushButton(groupBox_2);
        pushButton8->setObjectName(QStringLiteral("pushButton8"));
        pushButton8->setGeometry(QRect(220, 90, 40, 40));
        pushButton6 = new QPushButton(groupBox_2);
        pushButton6->setObjectName(QStringLiteral("pushButton6"));
        pushButton6->setGeometry(QRect(80, 90, 40, 40));
        pushButton10 = new QPushButton(groupBox_2);
        pushButton10->setObjectName(QStringLiteral("pushButton10"));
        pushButton10->setGeometry(QRect(80, 150, 40, 40));
        pushButton9 = new QPushButton(groupBox_2);
        pushButton9->setObjectName(QStringLiteral("pushButton9"));
        pushButton9->setGeometry(QRect(10, 150, 40, 40));
        pushButton12 = new QPushButton(groupBox_2);
        pushButton12->setObjectName(QStringLiteral("pushButton12"));
        pushButton12->setGeometry(QRect(220, 150, 40, 40));
        pushButton11 = new QPushButton(groupBox_2);
        pushButton11->setObjectName(QStringLiteral("pushButton11"));
        pushButton11->setGeometry(QRect(150, 150, 40, 40));
        groupBox_3 = new QGroupBox(UIPattern);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 270, 241, 71));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 51, 16));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(120, 30, 51, 16));
        pushButtonForeColor = new QPushButton(groupBox_3);
        pushButtonForeColor->setObjectName(QStringLiteral("pushButtonForeColor"));
        pushButtonForeColor->setGeometry(QRect(70, 20, 40, 40));
        pushButtonBackColor = new QPushButton(groupBox_3);
        pushButtonBackColor->setObjectName(QStringLiteral("pushButtonBackColor"));
        pushButtonBackColor->setGeometry(QRect(180, 20, 40, 40));
        pushButtonOK = new QPushButton(UIPattern);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(320, 300, 93, 28));
        pushButtonCancel = new QPushButton(UIPattern);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(450, 300, 93, 28));

        retranslateUi(UIPattern);

        QMetaObject::connectSlotsByName(UIPattern);
    } // setupUi

    void retranslateUi(QDialog *UIPattern)
    {
        UIPattern->setWindowTitle(QApplication::translate("UIPattern", "\347\275\221\347\272\271\345\241\253\345\205\205", 0));
        groupBox->setTitle(QApplication::translate("UIPattern", "\351\242\204\350\247\210\345\214\272", 0));
        pushButtonPreview->setText(QString());
        groupBox_2->setTitle(QApplication::translate("UIPattern", "\345\241\253\345\205\205\345\233\276\346\241\210\351\200\211\346\213\251", 0));
        pushButton1->setText(QString());
        pushButton2->setText(QString());
        pushButton3->setText(QString());
        pushButton4->setText(QString());
        pushButton5->setText(QString());
        pushButton7->setText(QString());
        pushButton8->setText(QString());
        pushButton6->setText(QString());
        pushButton10->setText(QString());
        pushButton9->setText(QString());
        pushButton12->setText(QString());
        pushButton11->setText(QString());
        groupBox_3->setTitle(QApplication::translate("UIPattern", "\347\275\221\347\272\271\351\242\234\350\211\262", 0));
        label->setText(QApplication::translate("UIPattern", "\345\211\215\346\231\257\350\211\262:", 0));
        label_2->setText(QApplication::translate("UIPattern", "\350\203\214\346\231\257\350\211\262:", 0));
        pushButtonForeColor->setText(QString());
        pushButtonBackColor->setText(QString());
        pushButtonOK->setText(QApplication::translate("UIPattern", "\347\241\256\345\256\232", 0));
        pushButtonCancel->setText(QApplication::translate("UIPattern", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class UIPattern: public Ui_UIPattern {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIPATTERN_H
