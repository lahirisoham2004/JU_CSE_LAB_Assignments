/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *workArea;
    QLabel *label;
    QLabel *label_2;
    QLabel *x_coordinate;
    QLabel *y_coordinate;
    QPushButton *showAxis;
    QPushButton *gridlines;
    QSpinBox *gridOffset;
    QSpinBox *axisWidth;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1241, 861);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        workArea = new QLabel(centralwidget);
        workArea->setObjectName("workArea");
        workArea->setGeometry(QRect(20, 20, 731, 631));
        workArea->setFrameShape(QFrame::Shape::Box);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(770, 20, 91, 20));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(950, 21, 91, 21));
        x_coordinate = new QLabel(centralwidget);
        x_coordinate->setObjectName("x_coordinate");
        x_coordinate->setGeometry(QRect(880, 20, 63, 31));
        x_coordinate->setFrameShape(QFrame::Shape::Box);
        y_coordinate = new QLabel(centralwidget);
        y_coordinate->setObjectName("y_coordinate");
        y_coordinate->setGeometry(QRect(1060, 20, 63, 31));
        y_coordinate->setFrameShape(QFrame::Shape::Box);
        showAxis = new QPushButton(centralwidget);
        showAxis->setObjectName("showAxis");
        showAxis->setGeometry(QRect(770, 70, 83, 29));
        gridlines = new QPushButton(centralwidget);
        gridlines->setObjectName("gridlines");
        gridlines->setGeometry(QRect(860, 70, 111, 29));
        gridOffset = new QSpinBox(centralwidget);
        gridOffset->setObjectName("gridOffset");
        gridOffset->setGeometry(QRect(860, 110, 81, 29));
        gridOffset->setMinimum(0);
        gridOffset->setSingleStep(10);
        axisWidth = new QSpinBox(centralwidget);
        axisWidth->setObjectName("axisWidth");
        axisWidth->setGeometry(QRect(860, 150, 81, 29));
        axisWidth->setMinimum(1);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(770, 110, 81, 20));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(770, 160, 81, 20));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(780, 220, 83, 29));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(910, 220, 131, 29));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(780, 280, 121, 29));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(930, 280, 83, 29));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(780, 350, 131, 29));
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(940, 350, 101, 29));
        pushButton_7 = new QPushButton(centralwidget);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(790, 420, 151, 29));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(970, 420, 141, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1241, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        workArea->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "X Coordinate", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Y Coordinate", nullptr));
        x_coordinate->setText(QString());
        y_coordinate->setText(QString());
        showAxis->setText(QCoreApplication::translate("MainWindow", "Show Axes", nullptr));
        gridlines->setText(QCoreApplication::translate("MainWindow", "Show Gridlines", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "GridOffset", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Axis Width", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "DDA Line", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Bressenham Line", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Cartesian Circle", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Polar Circle", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Bressenham Circle", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Polar Ellipse", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Bressenham Ellipse", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Scanline Filling", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
