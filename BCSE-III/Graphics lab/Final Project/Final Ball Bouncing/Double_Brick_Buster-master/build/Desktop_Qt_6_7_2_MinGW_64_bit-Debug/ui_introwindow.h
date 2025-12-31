/********************************************************************************
** Form generated from reading UI file 'introwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTROWINDOW_H
#define UI_INTROWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IntroWindow
{
public:
    QWidget *centralWidget;
    QLabel *title_label;
    QLabel *start_label;
    QPushButton *start_push_button;
    QLabel *title_label_2;
    QLabel *title_label_3;
    QLabel *title_label_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *IntroWindow)
    {
        if (IntroWindow->objectName().isEmpty())
            IntroWindow->setObjectName("IntroWindow");
        IntroWindow->resize(993, 968);
        centralWidget = new QWidget(IntroWindow);
        centralWidget->setObjectName("centralWidget");
        title_label = new QLabel(centralWidget);
        title_label->setObjectName("title_label");
        title_label->setGeometry(QRect(230, 0, 601, 51));
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        title_label->setFont(font);
        start_label = new QLabel(centralWidget);
        start_label->setObjectName("start_label");
        start_label->setGeometry(QRect(560, 90, 301, 31));
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        start_label->setFont(font1);
        start_push_button = new QPushButton(centralWidget);
        start_push_button->setObjectName("start_push_button");
        start_push_button->setGeometry(QRect(560, 140, 361, 431));
        QFont font2;
        font2.setPointSize(40);
        font2.setBold(true);
        start_push_button->setFont(font2);
        title_label_2 = new QLabel(centralWidget);
        title_label_2->setObjectName("title_label_2");
        title_label_2->setGeometry(QRect(60, 120, 601, 51));
        title_label_2->setFont(font);
        title_label_3 = new QLabel(centralWidget);
        title_label_3->setObjectName("title_label_3");
        title_label_3->setGeometry(QRect(60, 210, 601, 51));
        title_label_3->setFont(font);
        title_label_4 = new QLabel(centralWidget);
        title_label_4->setObjectName("title_label_4");
        title_label_4->setGeometry(QRect(60, 300, 601, 51));
        title_label_4->setFont(font);
        IntroWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(IntroWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 993, 25));
        IntroWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(IntroWindow);
        mainToolBar->setObjectName("mainToolBar");
        IntroWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(IntroWindow);
        statusBar->setObjectName("statusBar");
        IntroWindow->setStatusBar(statusBar);

        retranslateUi(IntroWindow);

        QMetaObject::connectSlotsByName(IntroWindow);
    } // setupUi

    void retranslateUi(QMainWindow *IntroWindow)
    {
        IntroWindow->setWindowTitle(QCoreApplication::translate("IntroWindow", "IntroWindow", nullptr));
        title_label->setText(QCoreApplication::translate("IntroWindow", "Ball Bouncing Game", nullptr));
        start_label->setText(QCoreApplication::translate("IntroWindow", "Let's Get Started!", nullptr));
        start_push_button->setText(QCoreApplication::translate("IntroWindow", "Start!", nullptr));
        title_label_2->setText(QCoreApplication::translate("IntroWindow", "Created by Group 2, A3", nullptr));
        title_label_3->setText(QCoreApplication::translate("IntroWindow", "BCSE-UG3", nullptr));
        title_label_4->setText(QCoreApplication::translate("IntroWindow", "Jadavpur University", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IntroWindow: public Ui_IntroWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTROWINDOW_H
