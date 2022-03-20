/********************************************************************************
** Form generated from reading UI file 'UIRotateSlider.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIROTATESLIDER_H
#define UI_UIROTATESLIDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_UIRotateSlider
{
public:
    QSlider *horizontalSlider;
    QSpinBox *spinBox;

    void setupUi(QDialog *UIRotateSlider)
    {
        if (UIRotateSlider->objectName().isEmpty())
            UIRotateSlider->setObjectName(QStringLiteral("UIRotateSlider"));
        UIRotateSlider->resize(409, 55);
        horizontalSlider = new QSlider(UIRotateSlider);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(80, 10, 311, 31));
        horizontalSlider->setOrientation(Qt::Horizontal);
        spinBox = new QSpinBox(UIRotateSlider);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(10, 10, 61, 31));

        retranslateUi(UIRotateSlider);

        QMetaObject::connectSlotsByName(UIRotateSlider);
    } // setupUi

    void retranslateUi(QDialog *UIRotateSlider)
    {
        UIRotateSlider->setWindowTitle(QApplication::translate("UIRotateSlider", "\346\227\213\350\275\254\346\273\221\345\212\250\345\235\227", 0));
    } // retranslateUi

};

namespace Ui {
    class UIRotateSlider: public Ui_UIRotateSlider {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIROTATESLIDER_H
