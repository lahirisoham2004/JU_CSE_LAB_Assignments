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
    QPushButton *DDADraw;
    QPushButton *store;
    QPushButton *reset;
    QPushButton *pushButton;
    QLabel *DDA_TIME;
    QLabel *Bresenham_TIME;
    QPushButton *polar_circle;
    QSpinBox *radius;
    QLabel *label_5;
    QLabel *polarCircleTime;
    QPushButton *mid_circle;
    QLabel *midpointCircleTime;
    QLabel *major_4;
    QSpinBox *major;
    QLabel *major_3;
    QSpinBox *minor;
    QPushButton *polar_ellipse;
    QPushButton *bressenham_ellipse;
    QLabel *polarEllipseTime;
    QLabel *bressenhamEllepseTime;
    QPushButton *Flood_Fill;
    QPushButton *DrawPolygon;
    QSpinBox *Vertices;
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
        DDADraw->setGeometry(QRect(760, 190, 141, 33));
        store = new QPushButton(centralwidget);
        store->setObjectName("store");
        store->setGeometry(QRect(1010, 70, 121, 31));
        reset = new QPushButton(centralwidget);
        reset->setObjectName("reset");
        reset->setGeometry(QRect(1010, 110, 121, 31));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(760, 240, 181, 33));
        DDA_TIME = new QLabel(centralwidget);
        DDA_TIME->setObjectName("DDA_TIME");
        DDA_TIME->setGeometry(QRect(930, 190, 271, 31));
        Bresenham_TIME = new QLabel(centralwidget);
        Bresenham_TIME->setObjectName("Bresenham_TIME");
        Bresenham_TIME->setGeometry(QRect(960, 240, 301, 31));
        polar_circle = new QPushButton(centralwidget);
        polar_circle->setObjectName("polar_circle");
        polar_circle->setGeometry(QRect(770, 340, 83, 29));
        radius = new QSpinBox(centralwidget);
        radius->setObjectName("radius");
        radius->setGeometry(QRect(880, 290, 81, 29));
        radius->setMinimum(0);
        radius->setSingleStep(10);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(770, 300, 81, 20));
        polarCircleTime = new QLabel(centralwidget);
        polarCircleTime->setObjectName("polarCircleTime");
        polarCircleTime->setGeometry(QRect(900, 330, 271, 31));
        mid_circle = new QPushButton(centralwidget);
        mid_circle->setObjectName("mid_circle");
        mid_circle->setGeometry(QRect(770, 380, 83, 29));
        midpointCircleTime = new QLabel(centralwidget);
        midpointCircleTime->setObjectName("midpointCircleTime");
        midpointCircleTime->setGeometry(QRect(900, 380, 271, 31));
        major_4 = new QLabel(centralwidget);
        major_4->setObjectName("major_4");
        major_4->setGeometry(QRect(770, 420, 81, 20));
        major = new QSpinBox(centralwidget);
        major->setObjectName("major");
        major->setGeometry(QRect(760, 460, 81, 29));
        major->setMinimum(0);
        major->setSingleStep(10);
        major_3 = new QLabel(centralwidget);
        major_3->setObjectName("major_3");
        major_3->setGeometry(QRect(880, 420, 81, 20));
        minor = new QSpinBox(centralwidget);
        minor->setObjectName("minor");
        minor->setGeometry(QRect(880, 460, 81, 29));
        minor->setMinimum(0);
        minor->setSingleStep(10);
        polar_ellipse = new QPushButton(centralwidget);
        polar_ellipse->setObjectName("polar_ellipse");
        polar_ellipse->setGeometry(QRect(1040, 420, 83, 29));
        bressenham_ellipse = new QPushButton(centralwidget);
        bressenham_ellipse->setObjectName("bressenham_ellipse");
        bressenham_ellipse->setGeometry(QRect(760, 500, 83, 29));
        polarEllipseTime = new QLabel(centralwidget);
        polarEllipseTime->setObjectName("polarEllipseTime");
        polarEllipseTime->setGeometry(QRect(1010, 460, 191, 31));
        bressenhamEllepseTime = new QLabel(centralwidget);
        bressenhamEllepseTime->setObjectName("bressenhamEllepseTime");
        bressenhamEllepseTime->setGeometry(QRect(880, 500, 231, 31));
        Flood_Fill = new QPushButton(centralwidget);
        Flood_Fill->setObjectName("Flood_Fill");
        Flood_Fill->setGeometry(QRect(760, 540, 83, 29));
        DrawPolygon = new QPushButton(centralwidget);
        DrawPolygon->setObjectName("DrawPolygon");
        DrawPolygon->setGeometry(QRect(880, 540, 83, 29));
        Vertices = new QSpinBox(centralwidget);
        Vertices->setObjectName("Vertices");
        Vertices->setGeometry(QRect(990, 540, 81, 29));
        Vertices->setMinimum(0);
        Vertices->setSingleStep(10);
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
        polar_circle->setText(QCoreApplication::translate("MainWindow", "polar circle", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "radius", nullptr));
        polarCircleTime->setText(QString());
        mid_circle->setText(QCoreApplication::translate("MainWindow", "mid circle", nullptr));
        midpointCircleTime->setText(QString());
        major_4->setText(QCoreApplication::translate("MainWindow", "major", nullptr));
        major_3->setText(QCoreApplication::translate("MainWindow", "minor", nullptr));
        polar_ellipse->setText(QCoreApplication::translate("MainWindow", "polar ellipse", nullptr));
        bressenham_ellipse->setText(QCoreApplication::translate("MainWindow", "bressenham", nullptr));
        polarEllipseTime->setText(QString());
        bressenhamEllepseTime->setText(QString());
        Flood_Fill->setText(QCoreApplication::translate("MainWindow", "Flood Fill", nullptr));
        DrawPolygon->setText(QCoreApplication::translate("MainWindow", "DrawPolygon", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
