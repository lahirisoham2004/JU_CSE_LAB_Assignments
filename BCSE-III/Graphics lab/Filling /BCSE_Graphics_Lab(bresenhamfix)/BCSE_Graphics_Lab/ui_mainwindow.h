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
#include <QtWidgets/QRadioButton>
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
    QPushButton *DDADraw;
    QPushButton *store;
    QPushButton *reset;
    QPushButton *pushButton;
    QLabel *DDA_TIME;
    QLabel *Bresenham_TIME;
    QLabel *y_coordinate_2;
    QSpinBox *radius;
    QPushButton *parametric;
    QPushButton *sqrt_btn;
    QPushButton *bresenham_circle;
    QSpinBox *major_axis;
    QLabel *y_coordinate_3;
    QLabel *y_coordinate_4;
    QSpinBox *minoraxis;
    QPushButton *e_parametric;
    QPushButton *e_sqrt;
    QPushButton *e_bresenham;
    QLabel *c_sqrt_time;
    QLabel *c_para_time;
    QLabel *c_bres_time;
    QLabel *e_para_time;
    QLabel *e_sqrt_time;
    QLabel *e_bre_time;
    QSpinBox *noOfSides;
    QLabel *y_coordinate_5;
    QPushButton *DrawPoly;
    QPushButton *fillP;
    QPushButton *scanlineP;
    QLabel *ff_time;
    QLabel *sf_time;
    QPushButton *bFill;
    QSpinBox *r;
    QSpinBox *g;
    QSpinBox *b;
    QLabel *r_value;
    QLabel *r_value_2;
    QLabel *r_value_3;
    QRadioButton *radioButton;
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
        DDADraw = new QPushButton(centralwidget);
        DDADraw->setObjectName("DDADraw");
        DDADraw->setGeometry(QRect(760, 210, 141, 33));
        store = new QPushButton(centralwidget);
        store->setObjectName("store");
        store->setGeometry(QRect(1030, 110, 121, 31));
        reset = new QPushButton(centralwidget);
        reset->setObjectName("reset");
        reset->setGeometry(QRect(1030, 150, 121, 31));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(760, 270, 181, 33));
        DDA_TIME = new QLabel(centralwidget);
        DDA_TIME->setObjectName("DDA_TIME");
        DDA_TIME->setGeometry(QRect(920, 210, 221, 31));
        Bresenham_TIME = new QLabel(centralwidget);
        Bresenham_TIME->setObjectName("Bresenham_TIME");
        Bresenham_TIME->setGeometry(QRect(960, 270, 191, 31));
        y_coordinate_2 = new QLabel(centralwidget);
        y_coordinate_2->setObjectName("y_coordinate_2");
        y_coordinate_2->setGeometry(QRect(770, 330, 63, 31));
        y_coordinate_2->setFrameShape(QFrame::Shape::NoFrame);
        radius = new QSpinBox(centralwidget);
        radius->setObjectName("radius");
        radius->setGeometry(QRect(840, 330, 48, 29));
        parametric = new QPushButton(centralwidget);
        parametric->setObjectName("parametric");
        parametric->setGeometry(QRect(760, 370, 83, 29));
        sqrt_btn = new QPushButton(centralwidget);
        sqrt_btn->setObjectName("sqrt_btn");
        sqrt_btn->setGeometry(QRect(880, 370, 91, 29));
        bresenham_circle = new QPushButton(centralwidget);
        bresenham_circle->setObjectName("bresenham_circle");
        bresenham_circle->setGeometry(QRect(1000, 370, 83, 29));
        major_axis = new QSpinBox(centralwidget);
        major_axis->setObjectName("major_axis");
        major_axis->setGeometry(QRect(840, 450, 48, 29));
        y_coordinate_3 = new QLabel(centralwidget);
        y_coordinate_3->setObjectName("y_coordinate_3");
        y_coordinate_3->setGeometry(QRect(760, 450, 81, 31));
        y_coordinate_3->setFrameShape(QFrame::Shape::NoFrame);
        y_coordinate_4 = new QLabel(centralwidget);
        y_coordinate_4->setObjectName("y_coordinate_4");
        y_coordinate_4->setGeometry(QRect(910, 450, 81, 31));
        y_coordinate_4->setFrameShape(QFrame::Shape::NoFrame);
        minoraxis = new QSpinBox(centralwidget);
        minoraxis->setObjectName("minoraxis");
        minoraxis->setGeometry(QRect(990, 450, 48, 29));
        e_parametric = new QPushButton(centralwidget);
        e_parametric->setObjectName("e_parametric");
        e_parametric->setGeometry(QRect(760, 490, 83, 29));
        e_sqrt = new QPushButton(centralwidget);
        e_sqrt->setObjectName("e_sqrt");
        e_sqrt->setGeometry(QRect(870, 490, 83, 29));
        e_bresenham = new QPushButton(centralwidget);
        e_bresenham->setObjectName("e_bresenham");
        e_bresenham->setGeometry(QRect(1000, 490, 83, 29));
        c_sqrt_time = new QLabel(centralwidget);
        c_sqrt_time->setObjectName("c_sqrt_time");
        c_sqrt_time->setGeometry(QRect(880, 410, 91, 21));
        c_para_time = new QLabel(centralwidget);
        c_para_time->setObjectName("c_para_time");
        c_para_time->setGeometry(QRect(760, 410, 91, 21));
        c_bres_time = new QLabel(centralwidget);
        c_bres_time->setObjectName("c_bres_time");
        c_bres_time->setGeometry(QRect(1000, 410, 91, 21));
        e_para_time = new QLabel(centralwidget);
        e_para_time->setObjectName("e_para_time");
        e_para_time->setGeometry(QRect(760, 530, 91, 21));
        e_sqrt_time = new QLabel(centralwidget);
        e_sqrt_time->setObjectName("e_sqrt_time");
        e_sqrt_time->setGeometry(QRect(870, 530, 91, 21));
        e_bre_time = new QLabel(centralwidget);
        e_bre_time->setObjectName("e_bre_time");
        e_bre_time->setGeometry(QRect(1000, 530, 91, 21));
        noOfSides = new QSpinBox(centralwidget);
        noOfSides->setObjectName("noOfSides");
        noOfSides->setGeometry(QRect(800, 600, 51, 29));
        noOfSides->setValue(8);
        y_coordinate_5 = new QLabel(centralwidget);
        y_coordinate_5->setObjectName("y_coordinate_5");
        y_coordinate_5->setGeometry(QRect(760, 600, 41, 31));
        y_coordinate_5->setFrameShape(QFrame::Shape::NoFrame);
        DrawPoly = new QPushButton(centralwidget);
        DrawPoly->setObjectName("DrawPoly");
        DrawPoly->setGeometry(QRect(910, 600, 141, 33));
        fillP = new QPushButton(centralwidget);
        fillP->setObjectName("fillP");
        fillP->setGeometry(QRect(760, 640, 81, 33));
        scanlineP = new QPushButton(centralwidget);
        scanlineP->setObjectName("scanlineP");
        scanlineP->setGeometry(QRect(850, 640, 81, 33));
        ff_time = new QLabel(centralwidget);
        ff_time->setObjectName("ff_time");
        ff_time->setGeometry(QRect(750, 720, 91, 21));
        sf_time = new QLabel(centralwidget);
        sf_time->setObjectName("sf_time");
        sf_time->setGeometry(QRect(850, 680, 91, 21));
        bFill = new QPushButton(centralwidget);
        bFill->setObjectName("bFill");
        bFill->setGeometry(QRect(940, 640, 81, 33));
        r = new QSpinBox(centralwidget);
        r->setObjectName("r");
        r->setGeometry(QRect(1100, 640, 48, 29));
        r->setMaximum(255);
        g = new QSpinBox(centralwidget);
        g->setObjectName("g");
        g->setGeometry(QRect(1100, 670, 48, 29));
        g->setMaximum(255);
        b = new QSpinBox(centralwidget);
        b->setObjectName("b");
        b->setGeometry(QRect(1100, 700, 48, 29));
        b->setMaximum(255);
        r_value = new QLabel(centralwidget);
        r_value->setObjectName("r_value");
        r_value->setGeometry(QRect(1080, 640, 16, 31));
        r_value->setFrameShape(QFrame::Shape::NoFrame);
        r_value_2 = new QLabel(centralwidget);
        r_value_2->setObjectName("r_value_2");
        r_value_2->setGeometry(QRect(1080, 700, 16, 31));
        r_value_2->setFrameShape(QFrame::Shape::NoFrame);
        r_value_3 = new QLabel(centralwidget);
        r_value_3->setObjectName("r_value_3");
        r_value_3->setGeometry(QRect(1080, 670, 16, 31));
        r_value_3->setFrameShape(QFrame::Shape::NoFrame);
        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(760, 680, 81, 26));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1241, 25));
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
        DDADraw->setText(QCoreApplication::translate("MainWindow", "Draw DDA Line", nullptr));
        store->setText(QCoreApplication::translate("MainWindow", "Store Canvas", nullptr));
        reset->setText(QCoreApplication::translate("MainWindow", "Reset Canvas", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Draw Bresenham Line", nullptr));
        DDA_TIME->setText(QString());
        Bresenham_TIME->setText(QString());
        y_coordinate_2->setText(QCoreApplication::translate("MainWindow", "Circle : ", nullptr));
        parametric->setText(QCoreApplication::translate("MainWindow", "Parametric", nullptr));
        sqrt_btn->setText(QCoreApplication::translate("MainWindow", "Square root", nullptr));
        bresenham_circle->setText(QCoreApplication::translate("MainWindow", "Bresenham", nullptr));
        y_coordinate_3->setText(QCoreApplication::translate("MainWindow", "Major axis:", nullptr));
        y_coordinate_4->setText(QCoreApplication::translate("MainWindow", "Minor axis:", nullptr));
        e_parametric->setText(QCoreApplication::translate("MainWindow", "Parametric", nullptr));
        e_sqrt->setText(QCoreApplication::translate("MainWindow", "Square root", nullptr));
        e_bresenham->setText(QCoreApplication::translate("MainWindow", "Bresenham", nullptr));
        c_sqrt_time->setText(QString());
        c_para_time->setText(QString());
        c_bres_time->setText(QString());
        e_para_time->setText(QString());
        e_sqrt_time->setText(QString());
        e_bre_time->setText(QString());
        y_coordinate_5->setText(QCoreApplication::translate("MainWindow", "Sides : ", nullptr));
        DrawPoly->setText(QCoreApplication::translate("MainWindow", "Draw Polygon", nullptr));
        fillP->setText(QCoreApplication::translate("MainWindow", "Flood Fill", nullptr));
        scanlineP->setText(QCoreApplication::translate("MainWindow", "Scanline", nullptr));
        ff_time->setText(QString());
        sf_time->setText(QString());
        bFill->setText(QCoreApplication::translate("MainWindow", "Boundary", nullptr));
        r_value->setText(QCoreApplication::translate("MainWindow", "r :", nullptr));
        r_value_2->setText(QCoreApplication::translate("MainWindow", "b :", nullptr));
        r_value_3->setText(QCoreApplication::translate("MainWindow", "g :", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "8-point", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
