/********************************************************************************
** Form generated from reading UI file 'UIGradient.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIGRADIENT_H
#define UI_UIGRADIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_Gradient
{
public:
    QGroupBox *groupBox;
    QComboBox *comboBoxHis;
    QGroupBox *groupBox_2;
    QPushButton *pushButtonLeftToRight;
    QPushButton *pushButtonTopToBottom;
    QPushButton *pushButtonLeftBottomToRightTop;
    QPushButton *pushButtonLeftTopToRightBottom;
    QPushButton *pushButtonRadient;
    QGroupBox *groupBox_3;
    QLabel *LabelPeView;
    QGroupBox *groupBox_4;
    QGroupBox *groupBox_5;
    QPushButton *pushButtonHeadToTail;
    QLabel *LabelMultiColor;
    QGroupBox *groupBox_6;
    QPushButton *pushButtonColorPick;
    QGroupBox *groupBox_7;
    QSlider *horizontalSlider;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *Gradient)
    {
        if (Gradient->objectName().isEmpty())
            Gradient->setObjectName(QStringLiteral("Gradient"));
        Gradient->resize(672, 410);
        groupBox = new QGroupBox(Gradient);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 251, 81));
        comboBoxHis = new QComboBox(groupBox);
        comboBoxHis->setObjectName(QStringLiteral("comboBoxHis"));
        comboBoxHis->setGeometry(QRect(10, 20, 231, 51));
        groupBox_2 = new QGroupBox(Gradient);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(280, 10, 371, 81));
        pushButtonLeftToRight = new QPushButton(groupBox_2);
        pushButtonLeftToRight->setObjectName(QStringLiteral("pushButtonLeftToRight"));
        pushButtonLeftToRight->setGeometry(QRect(20, 20, 48, 48));
        pushButtonLeftToRight->setAutoFillBackground(false);
        pushButtonTopToBottom = new QPushButton(groupBox_2);
        pushButtonTopToBottom->setObjectName(QStringLiteral("pushButtonTopToBottom"));
        pushButtonTopToBottom->setGeometry(QRect(90, 20, 48, 48));
        pushButtonLeftBottomToRightTop = new QPushButton(groupBox_2);
        pushButtonLeftBottomToRightTop->setObjectName(QStringLiteral("pushButtonLeftBottomToRightTop"));
        pushButtonLeftBottomToRightTop->setGeometry(QRect(160, 20, 48, 48));
        pushButtonLeftTopToRightBottom = new QPushButton(groupBox_2);
        pushButtonLeftTopToRightBottom->setObjectName(QStringLiteral("pushButtonLeftTopToRightBottom"));
        pushButtonLeftTopToRightBottom->setGeometry(QRect(230, 20, 48, 48));
        pushButtonRadient = new QPushButton(groupBox_2);
        pushButtonRadient->setObjectName(QStringLiteral("pushButtonRadient"));
        pushButtonRadient->setGeometry(QRect(300, 20, 48, 48));
        groupBox_3 = new QGroupBox(Gradient);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 100, 241, 251));
        LabelPeView = new QLabel(groupBox_3);
        LabelPeView->setObjectName(QStringLiteral("LabelPeView"));
        LabelPeView->setGeometry(QRect(20, 30, 200, 200));
        groupBox_4 = new QGroupBox(Gradient);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(280, 110, 371, 241));
        groupBox_5 = new QGroupBox(groupBox_4);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 20, 351, 101));
        pushButtonHeadToTail = new QPushButton(groupBox_5);
        pushButtonHeadToTail->setObjectName(QStringLiteral("pushButtonHeadToTail"));
        pushButtonHeadToTail->setGeometry(QRect(300, 30, 41, 51));
        LabelMultiColor = new QLabel(groupBox_5);
        LabelMultiColor->setObjectName(QStringLiteral("LabelMultiColor"));
        LabelMultiColor->setGeometry(QRect(20, 30, 281, 51));
        groupBox_6 = new QGroupBox(groupBox_4);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 130, 101, 101));
        pushButtonColorPick = new QPushButton(groupBox_6);
        pushButtonColorPick->setObjectName(QStringLiteral("pushButtonColorPick"));
        pushButtonColorPick->setGeometry(QRect(20, 20, 64, 64));
        groupBox_7 = new QGroupBox(groupBox_4);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(130, 130, 221, 101));
        horizontalSlider = new QSlider(groupBox_7);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 40, 181, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        pushButtonOK = new QPushButton(Gradient);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(440, 367, 93, 31));
        pushButtonCancel = new QPushButton(Gradient);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(560, 367, 93, 31));
        QWidget::setTabOrder(pushButtonColorPick, horizontalSlider);
        QWidget::setTabOrder(horizontalSlider, pushButtonHeadToTail);
        QWidget::setTabOrder(pushButtonHeadToTail, pushButtonRadient);
        QWidget::setTabOrder(pushButtonRadient, pushButtonLeftTopToRightBottom);
        QWidget::setTabOrder(pushButtonLeftTopToRightBottom, pushButtonLeftBottomToRightTop);
        QWidget::setTabOrder(pushButtonLeftBottomToRightTop, pushButtonTopToBottom);
        QWidget::setTabOrder(pushButtonTopToBottom, pushButtonLeftToRight);
        QWidget::setTabOrder(pushButtonLeftToRight, comboBoxHis);

        retranslateUi(Gradient);

        QMetaObject::connectSlotsByName(Gradient);
    } // setupUi

    void retranslateUi(QDialog *Gradient)
    {
        Gradient->setWindowTitle(QApplication::translate("Gradient", "\346\270\220\345\217\230\350\211\262\345\241\253\345\205\205", 0));
        groupBox->setTitle(QApplication::translate("Gradient", "\344\273\216\346\250\241\346\235\277\351\200\211\346\213\251", 0));
        groupBox_2->setTitle(QApplication::translate("Gradient", "\345\241\253\345\205\205\346\226\271\345\220\221\351\200\211\346\213\251", 0));
        pushButtonLeftToRight->setText(QString());
        pushButtonTopToBottom->setText(QString());
        pushButtonLeftBottomToRightTop->setText(QString());
        pushButtonLeftTopToRightBottom->setText(QString());
        pushButtonRadient->setText(QString());
        groupBox_3->setTitle(QApplication::translate("Gradient", "\351\242\204\350\247\210", 0));
        LabelPeView->setText(QApplication::translate("Gradient", "Label", 0));
        groupBox_4->setTitle(QApplication::translate("Gradient", "\346\270\220\345\217\230\350\260\203\346\225\264", 0));
        groupBox_5->setTitle(QApplication::translate("Gradient", "\345\267\246\351\224\256\345\215\225\345\207\273\345\242\236\345\212\240/\345\217\263\351\224\256\345\215\225\345\207\273\344\270\211\350\247\222\345\275\242\345\210\240\351\231\244", 0));
        pushButtonHeadToTail->setText(QApplication::translate("Gradient", "<->", 0));
        LabelMultiColor->setText(QApplication::translate("Gradient", "TextLabel", 0));
        groupBox_6->setTitle(QApplication::translate("Gradient", "\351\242\234\350\211\262\351\200\211\346\213\251", 0));
        pushButtonColorPick->setText(QString());
        groupBox_7->setTitle(QApplication::translate("Gradient", "\351\200\217\346\230\216\345\272\246", 0));
        pushButtonOK->setText(QApplication::translate("Gradient", "\347\241\256\345\256\232", 0));
        pushButtonCancel->setText(QApplication::translate("Gradient", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class Gradient: public Ui_Gradient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIGRADIENT_H
