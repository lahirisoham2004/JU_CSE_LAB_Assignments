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
#include <QtWidgets/QDoubleSpinBox>
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
    QLabel *y_coordinate_6;
    QPushButton *rotateO;
    QSpinBox *angleO;
    QPushButton *translateO;
    QLabel *y_coordinate_7;
    QLabel *r_value_4;
    QSpinBox *x_translateO;
    QSpinBox *y_translateO;
    QPushButton *scaleO;
    QLabel *r_value_5;
    QLabel *y_coordinate_8;
    QPushButton *shearO;
    QLabel *y_coordinate_9;
    QSpinBox *y_shearO;
    QLabel *r_value_6;
    QSpinBox *x_shearO;
    QPushButton *reflectx;
    QPushButton *reflecty;
    QDoubleSpinBox *x_scaleO;
    QDoubleSpinBox *y_scaleO;
    QPushButton *setRotP;
    QPushButton *setScaleP;
    QPushButton *pushButton_2;
    QPushButton *setLineRef;
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
        showAxis->setGeometry(QRect(770, 70, 83, 31));
        gridlines = new QPushButton(centralwidget);
        gridlines->setObjectName("gridlines");
        gridlines->setGeometry(QRect(850, 70, 111, 31));
        gridOffset = new QSpinBox(centralwidget);
        gridOffset->setObjectName("gridOffset");
        gridOffset->setGeometry(QRect(860, 110, 81, 29));
        gridOffset->setMinimum(0);
        gridOffset->setSingleStep(10);
        axisWidth = new QSpinBox(centralwidget);
        axisWidth->setObjectName("axisWidth");
        axisWidth->setGeometry(QRect(1050, 110, 81, 29));
        axisWidth->setMinimum(1);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(770, 109, 81, 31));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(960, 110, 81, 31));
        DDADraw = new QPushButton(centralwidget);
        DDADraw->setObjectName("DDADraw");
        DDADraw->setGeometry(QRect(770, 160, 111, 33));
        store = new QPushButton(centralwidget);
        store->setObjectName("store");
        store->setGeometry(QRect(960, 70, 111, 31));
        reset = new QPushButton(centralwidget);
        reset->setObjectName("reset");
        reset->setGeometry(QRect(1070, 70, 111, 31));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(880, 160, 151, 33));
        DDA_TIME = new QLabel(centralwidget);
        DDA_TIME->setObjectName("DDA_TIME");
        DDA_TIME->setGeometry(QRect(770, 200, 111, 31));
        Bresenham_TIME = new QLabel(centralwidget);
        Bresenham_TIME->setObjectName("Bresenham_TIME");
        Bresenham_TIME->setGeometry(QRect(880, 200, 151, 31));
        y_coordinate_2 = new QLabel(centralwidget);
        y_coordinate_2->setObjectName("y_coordinate_2");
        y_coordinate_2->setGeometry(QRect(760, 250, 63, 31));
        y_coordinate_2->setFrameShape(QFrame::Shape::NoFrame);
        radius = new QSpinBox(centralwidget);
        radius->setObjectName("radius");
        radius->setGeometry(QRect(810, 250, 48, 29));
        parametric = new QPushButton(centralwidget);
        parametric->setObjectName("parametric");
        parametric->setGeometry(QRect(880, 250, 83, 29));
        sqrt_btn = new QPushButton(centralwidget);
        sqrt_btn->setObjectName("sqrt_btn");
        sqrt_btn->setGeometry(QRect(960, 250, 91, 29));
        bresenham_circle = new QPushButton(centralwidget);
        bresenham_circle->setObjectName("bresenham_circle");
        bresenham_circle->setGeometry(QRect(1050, 250, 83, 29));
        major_axis = new QSpinBox(centralwidget);
        major_axis->setObjectName("major_axis");
        major_axis->setGeometry(QRect(840, 320, 48, 29));
        y_coordinate_3 = new QLabel(centralwidget);
        y_coordinate_3->setObjectName("y_coordinate_3");
        y_coordinate_3->setGeometry(QRect(760, 320, 81, 31));
        y_coordinate_3->setFrameShape(QFrame::Shape::NoFrame);
        y_coordinate_4 = new QLabel(centralwidget);
        y_coordinate_4->setObjectName("y_coordinate_4");
        y_coordinate_4->setGeometry(QRect(920, 320, 81, 31));
        y_coordinate_4->setFrameShape(QFrame::Shape::NoFrame);
        minoraxis = new QSpinBox(centralwidget);
        minoraxis->setObjectName("minoraxis");
        minoraxis->setGeometry(QRect(1000, 320, 48, 29));
        e_parametric = new QPushButton(centralwidget);
        e_parametric->setObjectName("e_parametric");
        e_parametric->setGeometry(QRect(760, 370, 83, 29));
        e_sqrt = new QPushButton(centralwidget);
        e_sqrt->setObjectName("e_sqrt");
        e_sqrt->setGeometry(QRect(860, 370, 83, 29));
        e_bresenham = new QPushButton(centralwidget);
        e_bresenham->setObjectName("e_bresenham");
        e_bresenham->setGeometry(QRect(960, 370, 83, 29));
        c_sqrt_time = new QLabel(centralwidget);
        c_sqrt_time->setObjectName("c_sqrt_time");
        c_sqrt_time->setGeometry(QRect(960, 290, 91, 21));
        c_para_time = new QLabel(centralwidget);
        c_para_time->setObjectName("c_para_time");
        c_para_time->setGeometry(QRect(870, 290, 91, 21));
        c_bres_time = new QLabel(centralwidget);
        c_bres_time->setObjectName("c_bres_time");
        c_bres_time->setGeometry(QRect(1060, 290, 91, 21));
        e_para_time = new QLabel(centralwidget);
        e_para_time->setObjectName("e_para_time");
        e_para_time->setGeometry(QRect(760, 410, 91, 21));
        e_sqrt_time = new QLabel(centralwidget);
        e_sqrt_time->setObjectName("e_sqrt_time");
        e_sqrt_time->setGeometry(QRect(860, 410, 91, 21));
        e_bre_time = new QLabel(centralwidget);
        e_bre_time->setObjectName("e_bre_time");
        e_bre_time->setGeometry(QRect(960, 410, 91, 21));
        noOfSides = new QSpinBox(centralwidget);
        noOfSides->setObjectName("noOfSides");
        noOfSides->setGeometry(QRect(820, 450, 51, 29));
        noOfSides->setValue(8);
        y_coordinate_5 = new QLabel(centralwidget);
        y_coordinate_5->setObjectName("y_coordinate_5");
        y_coordinate_5->setGeometry(QRect(770, 450, 41, 31));
        y_coordinate_5->setFrameShape(QFrame::Shape::NoFrame);
        DrawPoly = new QPushButton(centralwidget);
        DrawPoly->setObjectName("DrawPoly");
        DrawPoly->setGeometry(QRect(760, 490, 141, 33));
        fillP = new QPushButton(centralwidget);
        fillP->setObjectName("fillP");
        fillP->setGeometry(QRect(920, 450, 81, 33));
        scanlineP = new QPushButton(centralwidget);
        scanlineP->setObjectName("scanlineP");
        scanlineP->setGeometry(QRect(1010, 450, 81, 33));
        ff_time = new QLabel(centralwidget);
        ff_time->setObjectName("ff_time");
        ff_time->setGeometry(QRect(920, 520, 91, 21));
        sf_time = new QLabel(centralwidget);
        sf_time->setObjectName("sf_time");
        sf_time->setGeometry(QRect(1010, 490, 91, 21));
        bFill = new QPushButton(centralwidget);
        bFill->setObjectName("bFill");
        bFill->setGeometry(QRect(1100, 450, 81, 33));
        r = new QSpinBox(centralwidget);
        r->setObjectName("r");
        r->setGeometry(QRect(1140, 490, 48, 29));
        r->setMaximum(255);
        g = new QSpinBox(centralwidget);
        g->setObjectName("g");
        g->setGeometry(QRect(1140, 520, 48, 29));
        g->setMaximum(255);
        b = new QSpinBox(centralwidget);
        b->setObjectName("b");
        b->setGeometry(QRect(1140, 550, 48, 29));
        b->setMaximum(255);
        r_value = new QLabel(centralwidget);
        r_value->setObjectName("r_value");
        r_value->setGeometry(QRect(1120, 490, 16, 31));
        r_value->setFrameShape(QFrame::Shape::NoFrame);
        r_value_2 = new QLabel(centralwidget);
        r_value_2->setObjectName("r_value_2");
        r_value_2->setGeometry(QRect(1120, 550, 16, 31));
        r_value_2->setFrameShape(QFrame::Shape::NoFrame);
        r_value_3 = new QLabel(centralwidget);
        r_value_3->setObjectName("r_value_3");
        r_value_3->setGeometry(QRect(1120, 520, 16, 31));
        r_value_3->setFrameShape(QFrame::Shape::NoFrame);
        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(920, 490, 81, 26));
        y_coordinate_6 = new QLabel(centralwidget);
        y_coordinate_6->setObjectName("y_coordinate_6");
        y_coordinate_6->setGeometry(QRect(760, 550, 201, 31));
        y_coordinate_6->setFrameShape(QFrame::Shape::NoFrame);
        rotateO = new QPushButton(centralwidget);
        rotateO->setObjectName("rotateO");
        rotateO->setGeometry(QRect(760, 580, 83, 29));
        angleO = new QSpinBox(centralwidget);
        angleO->setObjectName("angleO");
        angleO->setGeometry(QRect(850, 580, 48, 29));
        angleO->setMaximum(360);
        translateO = new QPushButton(centralwidget);
        translateO->setObjectName("translateO");
        translateO->setGeometry(QRect(760, 610, 83, 29));
        y_coordinate_7 = new QLabel(centralwidget);
        y_coordinate_7->setObjectName("y_coordinate_7");
        y_coordinate_7->setGeometry(QRect(860, 610, 16, 31));
        y_coordinate_7->setFrameShape(QFrame::Shape::NoFrame);
        r_value_4 = new QLabel(centralwidget);
        r_value_4->setObjectName("r_value_4");
        r_value_4->setGeometry(QRect(950, 610, 16, 31));
        r_value_4->setFrameShape(QFrame::Shape::NoFrame);
        x_translateO = new QSpinBox(centralwidget);
        x_translateO->setObjectName("x_translateO");
        x_translateO->setGeometry(QRect(880, 610, 48, 29));
        x_translateO->setMinimum(-200);
        x_translateO->setMaximum(200);
        y_translateO = new QSpinBox(centralwidget);
        y_translateO->setObjectName("y_translateO");
        y_translateO->setGeometry(QRect(970, 610, 48, 29));
        y_translateO->setMinimum(-200);
        y_translateO->setMaximum(200);
        scaleO = new QPushButton(centralwidget);
        scaleO->setObjectName("scaleO");
        scaleO->setGeometry(QRect(760, 640, 83, 29));
        r_value_5 = new QLabel(centralwidget);
        r_value_5->setObjectName("r_value_5");
        r_value_5->setGeometry(QRect(960, 640, 16, 31));
        r_value_5->setFrameShape(QFrame::Shape::NoFrame);
        y_coordinate_8 = new QLabel(centralwidget);
        y_coordinate_8->setObjectName("y_coordinate_8");
        y_coordinate_8->setGeometry(QRect(860, 640, 16, 31));
        y_coordinate_8->setFrameShape(QFrame::Shape::NoFrame);
        shearO = new QPushButton(centralwidget);
        shearO->setObjectName("shearO");
        shearO->setGeometry(QRect(760, 670, 83, 29));
        y_coordinate_9 = new QLabel(centralwidget);
        y_coordinate_9->setObjectName("y_coordinate_9");
        y_coordinate_9->setGeometry(QRect(860, 670, 16, 31));
        y_coordinate_9->setFrameShape(QFrame::Shape::NoFrame);
        y_shearO = new QSpinBox(centralwidget);
        y_shearO->setObjectName("y_shearO");
        y_shearO->setGeometry(QRect(970, 670, 48, 29));
        y_shearO->setMinimum(-200);
        y_shearO->setMaximum(200);
        r_value_6 = new QLabel(centralwidget);
        r_value_6->setObjectName("r_value_6");
        r_value_6->setGeometry(QRect(950, 670, 16, 31));
        r_value_6->setFrameShape(QFrame::Shape::NoFrame);
        x_shearO = new QSpinBox(centralwidget);
        x_shearO->setObjectName("x_shearO");
        x_shearO->setGeometry(QRect(880, 670, 48, 29));
        x_shearO->setMinimum(-200);
        x_shearO->setMaximum(200);
        reflectx = new QPushButton(centralwidget);
        reflectx->setObjectName("reflectx");
        reflectx->setGeometry(QRect(760, 700, 83, 29));
        reflecty = new QPushButton(centralwidget);
        reflecty->setObjectName("reflecty");
        reflecty->setGeometry(QRect(840, 700, 83, 29));
        x_scaleO = new QDoubleSpinBox(centralwidget);
        x_scaleO->setObjectName("x_scaleO");
        x_scaleO->setGeometry(QRect(880, 640, 67, 29));
        x_scaleO->setMinimum(-99.989999999999995);
        y_scaleO = new QDoubleSpinBox(centralwidget);
        y_scaleO->setObjectName("y_scaleO");
        y_scaleO->setGeometry(QRect(980, 640, 67, 29));
        y_scaleO->setMinimum(-99.989999999999995);
        setRotP = new QPushButton(centralwidget);
        setRotP->setObjectName("setRotP");
        setRotP->setGeometry(QRect(930, 580, 83, 29));
        setScaleP = new QPushButton(centralwidget);
        setScaleP->setObjectName("setScaleP");
        setScaleP->setGeometry(QRect(1050, 640, 83, 29));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(930, 700, 83, 29));
        setLineRef = new QPushButton(centralwidget);
        setLineRef->setObjectName("setLineRef");
        setLineRef->setGeometry(QRect(1010, 700, 83, 29));
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
        y_coordinate_6->setText(QCoreApplication::translate("MainWindow", "Transformation around Origin:", nullptr));
        rotateO->setText(QCoreApplication::translate("MainWindow", "Rotate", nullptr));
        translateO->setText(QCoreApplication::translate("MainWindow", "Translate", nullptr));
        y_coordinate_7->setText(QCoreApplication::translate("MainWindow", "x :", nullptr));
        r_value_4->setText(QCoreApplication::translate("MainWindow", "y :", nullptr));
        scaleO->setText(QCoreApplication::translate("MainWindow", "Scale", nullptr));
        r_value_5->setText(QCoreApplication::translate("MainWindow", "y :", nullptr));
        y_coordinate_8->setText(QCoreApplication::translate("MainWindow", "x :", nullptr));
        shearO->setText(QCoreApplication::translate("MainWindow", "Shear", nullptr));
        y_coordinate_9->setText(QCoreApplication::translate("MainWindow", "sx :", nullptr));
        r_value_6->setText(QCoreApplication::translate("MainWindow", "sy :", nullptr));
        reflectx->setText(QCoreApplication::translate("MainWindow", "Reflect x", nullptr));
        reflecty->setText(QCoreApplication::translate("MainWindow", "Reflect y", nullptr));
        setRotP->setText(QCoreApplication::translate("MainWindow", "set point", nullptr));
        setScaleP->setText(QCoreApplication::translate("MainWindow", "set point", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Reflect", nullptr));
        setLineRef->setText(QCoreApplication::translate("MainWindow", "Set Line", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
