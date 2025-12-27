#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <bits/stdc++.h>
#include <QMouseEvent>
#include <QElapsedTimer>
#define Delay delay(0)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->workArea->setMouseTracking(true);
    ui->workArea->installEventFilter(this);

    QPixmap canvas = ui->workArea->pixmap(Qt::ReturnByValue);
    if (canvas.isNull()) {
        canvas = QPixmap(ui->workArea->size());
        canvas.fill(Qt::white);
        ui->workArea->setPixmap(canvas);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::delay(int ms){
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::colorPoint(int x, int y, int r, int g, int b, int penwidth=1) {
    QPixmap canvas=ui->workArea->pixmap();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int gridCenterX=width/2;
    int gridCenterY=height/2;
    double gridOffset=ui->gridOffset->value();
    int X = floor((x-gridCenterX)*1.0/gridOffset);
    int Y = floor((gridCenterY-y)*1.0/gridOffset);
    int calcX = gridCenterX+ X*gridOffset + gridOffset/2;
    int calcY = gridCenterY -  Y*gridOffset - gridOffset/2;
    QPainter painter(&canvas);
    QPen pen=QPen(QColor(r,g,b),penwidth);
    painter.setPen(pen);
    painter.drawPoint(calcX, calcY);
    ui->workArea->setPixmap(canvas);
}

void MainWindow::on_showAxis_clicked() {
    int gridOffset = (ui->gridOffset->value() == 0) ? 1  : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;
    int axisWidth=ui->gridOffset->value();
    // qDebug()<<width<<height<<centerX<<centerY<<axisWidth;
    // Draw horizontal axis
    for (int x = 0; x < width; ++x) {
        colorPoint(x, (centerY - gridOffset/2.0), 255, 0, 0, axisWidth); // Black color
    }

    // Draw vertical axis
    for (int y = 0; y < height; ++y) {
        colorPoint(centerX + gridOffset/2.0, y, 255, 0, 0, axisWidth); // Black color
    }

}

void MainWindow::on_gridlines_clicked() {
    int gridOffset = ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    if (gridOffset <= 0) return; // Prevent invalid grid offset

    int centerX=width/2;
    int centerY=height/2;
    QPixmap canvas=ui->workArea->pixmap();
    QPainter painter(&canvas);
    for(int i =  0; (centerX+i<width && centerX-i>0) || (centerY+i<height && centerY-i>0); i+= gridOffset)
    {
        QPoint qp1 = QPoint(centerX + i, 0);
        QPoint qp2 = QPoint(centerX + i, height);
        QPoint qp3 = QPoint(centerX - i, 0);
        QPoint qp4 = QPoint(centerX - i, height);

        QPoint qp5 = QPoint(0, centerY + i);
        QPoint qp6 = QPoint(width, centerY + i);
        QPoint qp7 = QPoint(0, centerY-i);
        QPoint qp8 = QPoint(width, centerY - i);
        painter.drawLine(qp1, qp2);
        painter.drawLine(qp3, qp4);
        painter.drawLine(qp5, qp6);
        painter.drawLine(qp7, qp8);
    }

    ui->workArea->setPixmap(canvas);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->workArea && event->type() == QEvent::MouseMove) {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();
        int gridOffset = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
        int width = ui->workArea->width();
        int height = ui->workArea->height();
        int centerX=width/2;
        int centerY=height/2;
        ui->x_coordinate->setText(QString::number(floor((x-centerX)*1.0/gridOffset)));
        ui->y_coordinate->setText(QString::number(floor((centerY-y)*1.0/gridOffset)));
        return true; // Event handled
    }
    if(watched == ui->workArea && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();
        int gridOffset = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
        int width = ui->workArea->width();
        int height = ui->workArea->height();
        int centerX=width/2;
        int centerY=height/2;
        clickedPoint.setX(x);
        clickedPoint.setY(y);
        // points.push_back({})
        int X = floor((x-centerX)*1.0/gridOffset);
        int Y = floor((centerY-y)*1.0/gridOffset);
        points.push_back({X, Y});
        int calcX = centerX+ X*gridOffset + gridOffset/2;
        int calcY = centerY -  Y*gridOffset - gridOffset/2;
        // points.push_back({calcX, calcY});
        colorPoint(calcX, calcY, 0,0, 255, gridOffset);

    }
    return QMainWindow::eventFilter(watched, event);

}

void MainWindow::draw_dda_line(float x1, float y1, float x2, float y2)
{
    float dx, dy, xinc, yinc, steps;

    dx = x2 - x1;
    dy = y2 - y1;
    steps = std::max(abs(dx), abs(dy));  // Determine the number of steps based on the larger difference

    xinc = dx / steps;  // Increment in x
    yinc = dy / steps;  // Increment in y

    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    float x_float = centerX + x1 * gridOffset + gridOffset / 2.0;
    float y_float = centerY - y1 * gridOffset - gridOffset / 2.0;

    int xn = static_cast<int>(x_float);  // Initial x position in the grid
    int yn = static_cast<int>(y_float);  // Initial y position in the grid

    qDebug() << xn << yn;  // Print the x, y values for debugging
    colorPoint(xn, yn, 0, 255, 0, gridOffset);  // Color the initial point
    QVector<QPoint> pts;

    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < steps; i++)  // Loop to complete the straight line
    {
        x_float += xinc * gridOffset;
        y_float -= yinc * gridOffset;

        int x_new = static_cast<int>(x_float);  // New x position in the grid
        int y_new = static_cast<int>(y_float);  // New y position in the grid

        if (x_new != xn || y_new != yn)  // If there is a change in the grid position
        {
            xn = x_new;
            yn = y_new;
            int X = floor((xn-centerX)*1.0/gridOffset);
            int Y = floor((centerY-yn)*1.0/gridOffset);
            int calcX = centerX+ X*gridOffset + gridOffset/2;
            int calcY = centerY -  Y*gridOffset - gridOffset/2;
            pts.push_back(QPoint(calcX, calcY));
            // colorPoint(calcX, calcY, 0, 255, 0, gridOffset);  // Color the new point
        }
        qint64 elapsedTime = timer.nsecsElapsed();
        ui->DDA_TIME->setText(QString("Time Taken: ") +QString::number(elapsedTime) + QString(" ns"));

        /*qDebug() << x_new << y_new;*/  // Print the updated x, y values for debugging
    }
    for(auto x: pts)
    {
        colorPoint(x.x(), x.y(), 255, 255, 0, gridOffset);
    }
}


void MainWindow::draw_bresenham_line(int x1, int y1, int x2, int y2)
{

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x, y;

    int xsign = (x2 > x1) ? 1 : -1;
    int ysign = (y2 > y1) ? 1 : -1;

    bool isSteep = dy > dx;  // Check if the slope is steep

    if (isSteep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(dx, dy);
        std::swap(xsign,ysign);
    }

    int p = 2 * dy - dx;
    int twoDy = 2 * dy;
    int twoDyDx = 2 * (dy - dx);

    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    x = x1;
    y = y1;

    // Convert to grid coordinates
    int xGrid = centerX + (isSteep ? y : x) * gridOffset + gridOffset / 2;
    int yGrid = centerY - (isSteep ? x : y) * gridOffset - gridOffset / 2;

    colorPoint(xGrid, yGrid, 0, 255, 0, gridOffset);  // Color the initial point
    QVector<QPoint> pts;

    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < dx; ++i)
    {
        x += xsign;
        if (p < 0)
        {
            p += twoDy;
        }
        else
        {
            y += ysign;
            p += twoDyDx;
        }

        xGrid = centerX + (isSteep ? y : x) * gridOffset + gridOffset / 2;
        yGrid = centerY - (isSteep ? x : y) * gridOffset - gridOffset / 2;
        pts.push_back(QPoint(xGrid, yGrid));
        polygon.append(QPoint(xGrid,yGrid));
        //qDebug()<<xGrid<<yGrid;
        // colorPoint(xGrid, yGrid, 0, 255, 255, gridOffset);  // Color the current point
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->Bresenham_TIME->setText(QString("Time Taken: ") + QString::number(elapsedTime) + QString(" ns"));

    for(auto x: pts)
    {
        colorPoint(x.x(), x.y(), 0, 233, 59, gridOffset);
    }



}

void MainWindow::on_setCoord_clicked()
{

}


void MainWindow::on_DDADraw_clicked()
{
    if(points.size()< 2 ) return;
    qint64 n  = points.size();
    auto coords1 = points[n-1];
    auto coords2 = points[n-2];
    draw_dda_line(coords1.x(), coords1.y(), coords2.x(), coords2.y());
}


void MainWindow::on_store_clicked()
{
    temp = ui->workArea->pixmap(Qt::ReturnByValue);
}


void MainWindow::on_reset_clicked()
{
    ui->workArea->setPixmap(temp);
    points.clear();
}


void MainWindow::on_pushButton_clicked()
{
    if(points.size()< 2 ) return;
    int n = points.size();
    auto coords1 = points[n-1];
    auto coords2 = points[n-2];
    draw_bresenham_line(coords1.x(), coords1.y(), coords2.x(), coords2.y());
}





void MainWindow::on_polar_circle_clicked()
{

        const double PI = 3.14159;
        double radius=ui->radius->value();
        double gridOffset=ui->gridOffset->value();
        const int numSteps = radius*gridOffset;
        double angleStep = 2 * PI / numSteps;
        double centerX=clickedPoint.x();
        double centerY=clickedPoint.y();
        QElapsedTimer timer;
        timer.start();
        for (int i = 0; i <= numSteps; ++i) {
            double angle = i * angleStep;
            int x = centerX + static_cast<int>(radius*gridOffset * cos(angle));
            int y = centerY + static_cast<int>(radius*gridOffset * sin(angle));
            int width = ui->workArea->width();
            int height = ui->workArea->height();
            int gridCenterX=width/2;
            int gridCenterY=height/2;
            int X = floor((x-gridCenterX)*1.0/gridOffset);
            int Y = floor((gridCenterY-y)*1.0/gridOffset);
            int calcX = gridCenterX+ X*gridOffset + gridOffset/2;
            int calcY = gridCenterY -  Y*gridOffset - gridOffset/2;
            colorPoint(calcX,calcY,255,255,0,gridOffset);
        }
        qint64 elapsedTime = timer.nsecsElapsed();
        ui->polarCircleTime->setText(QString("Time Taken:") + QString::number(elapsedTime) + QString(" ns"));

}


void MainWindow::on_mid_circle_clicked()
{
    int r = ui->radius->value();
    double gridOffset=ui->gridOffset->value();
    double centerX=clickedPoint.x();
    double centerY=clickedPoint.y();

    int x0 = centerX;
    int y0 = centerY;
    x0 = x0 + gridOffset/2;
    y0 = y0 + gridOffset/2;

    int x = r*gridOffset;
    int y = 0;
    int p = (1 - r)*gridOffset;
    QElapsedTimer timer;
    timer.start();
    while(x > y) {
        colorPoint(x0 - x, y0 - y,0,255,0,gridOffset);
        colorPoint(x0 + x, y0 - y,0,255,0,gridOffset);
        colorPoint(x0 - x, y0 + y,0,255,0,gridOffset);
        colorPoint(x0 + x, y0 + y,0,255,0,gridOffset);
        colorPoint(x0 - y, y0 - x,0,255,0,gridOffset);
        colorPoint(x0 + y, y0 - x,0,255,0,gridOffset);
        colorPoint(x0 - y, y0 + x,0,255,0,gridOffset);
        colorPoint(x0 + y, y0 + x,0,255,0,gridOffset);

        y += gridOffset;
        if(p <= 0) {
            p += 2*y + 1;
        } else {
            p += 2*y + 1 - 2*x;
            x -= gridOffset;
        }
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->midpointCircleTime->setText(QString("Time Taken:") + QString::number(elapsedTime) + QString(" ns"));

}




void MainWindow::on_Bressenham_circle_clicked()
{
    int r = ui->radius->value();
    double gridOffset=ui->gridOffset->value();
    double centerX=clickedPoint.x();
    double centerY=clickedPoint.y();
    int x0 = centerX/gridOffset;
    int y0 = centerY/gridOffset;
    x0 = x0*gridOffset + gridOffset/2;
    y0 = y0*gridOffset +gridOffset/2;

    int x = 0;
    int y = r*gridOffset;
    int p = (3 - 2*r)*gridOffset;
    QElapsedTimer timer;
    timer.start();
    while(y > x) {
        colorPoint(x0 - x, y0 - y,0,255,255,gridOffset);
        colorPoint(x0 + x, y0 - y,0,255,255,gridOffset);
        colorPoint(x0 - x, y0 + y,0,255,255,gridOffset);
        colorPoint(x0 + x, y0 + y,0,255,255,gridOffset);
        colorPoint(x0 - y, y0 - x,0,255,255,gridOffset);
        colorPoint(x0 + y, y0 - x,0,255,255,gridOffset);
        colorPoint(x0 - y, y0 + x,0,255,255,gridOffset);
        colorPoint(x0 + y, y0 + x,0,255,255,gridOffset);

        x += gridOffset;
        if(p <= 0) {
            p += 4*x + 6;
        } else {
            p += 4*(x-y) + 10;
            y -= gridOffset;
        }
    }
    //qint64 elapsedTime = timer.nsecsElapsed();
   // ui->bressCircleTime->setText(QString("Time Taken:") + QString::number(elapsedTime) + QString(" ns"));
}



void MainWindow::on_polar_ellipse_clicked()
{
    double gridOffset=ui->gridOffset->value();
    double a=ui->major->value()*gridOffset;
    double b=ui->minor->value()*gridOffset;
    double centerX=clickedPoint.x();
    double centerY=clickedPoint.y();
    QElapsedTimer timer;
    timer.start();
    const double PI = 3.14159;
    for (double theta = 0; theta <= 2 * PI; theta += 0.01) {
        int x = centerX + a * cos(theta);
        int y = centerY + b * sin(theta);
        colorPoint(x,y,255,0,0,gridOffset);
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->polarEllipseTime->setText(QString("Time Taken:") + QString::number(elapsedTime) + QString(" ns"));
}


void MainWindow::on_bressenham_ellipse_clicked()
{

    int rx = ui->major->value();
    int ry = ui->minor->value();
    double centerX=clickedPoint.x();
    double centerY=clickedPoint.y();
    int k = ui->gridOffset->value();//GridSize

    int x_centre=centerX+k/2;
    int y_centre=centerY+k/2;

    int x=0;
    int y=ry;

    int rx2=rx*rx;
    int ry2=ry*ry;
    int tworx2=2*rx2;
    int twory2=2*ry2;
    int px=0.0;
    int py=tworx2*y;
    //For first region
    int p1=ry2-rx2*ry+(0.25)*rx2; //Initial value of decision paramemter
    QElapsedTimer timer;
    timer.start();

    while(px<py)
    {
        colorPoint(x_centre+x*k,y_centre+y*k,0,255,255,k);
        colorPoint(x_centre-x*k,y_centre+y*k,0,255,255,k);
        colorPoint(x_centre-x*k,y_centre-y*k,0,255,255,k);
        colorPoint(x_centre+x*k,y_centre-y*k,0,255,255,k);

        x++;
        px+=twory2;
        if(p1>=0) {
            y--;
            py-=tworx2;
            p1=p1+ry2+px-py;

        }  else{
            p1=p1+ry2+px;
        }
    }

    //For second region
    p1=ry2*((double)x+0.5)*((double)x+0.5)+rx2*(y-1)*(y-1)-rx2*ry2; //Initial value of decision paramemter


    while(y>=0)
    {
        colorPoint(x_centre+x*k,y_centre+y*k,0,255,255,k);
        colorPoint(x_centre-x*k,y_centre+y*k,0,255,255,k);
        colorPoint(x_centre-x*k,y_centre-y*k,0,255,255,k);
        colorPoint(x_centre+x*k,y_centre-y*k,0,255,255,k);

        y--;
        py-=tworx2;
        if(p1<=0){
            x++;
            px+=twory2;
            p1=p1+rx2-py+px;

        }  else  {
            p1=p1+rx2-py;
        }
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->bressenhamEllepseTime->setText(QString("Time Taken:") + QString::number(elapsedTime) + QString(" ns"));
}

void MainWindow :: floodfillutil(int x, int y, int r, int g, int b, QList<std::pair<int,int>>& visited) {
    int height=ui->workArea->height();
    int width=ui->workArea->width();

    if(x<0 || y<0 ||x>width||y>height|| polygon.contains(QPoint(x,y)) || visited.contains({x,y})) return;
    qDebug()<<x<<y;
    int gridsize=ui->gridOffset->value();
    colorPoint(x,y,r,g,b,gridsize);
    visited.append(std::make_pair(x,y));
    //delay(1000);
     floodfillutil(x - gridsize, y, r,g,b, visited);
     floodfillutil(x + gridsize, y, r,g,b, visited);
     floodfillutil(x, y - gridsize, r,g,b, visited);
     floodfillutil(x, y + gridsize, r,g,b, visited);
}

void MainWindow::on_Flood_Fill_clicked()
{
        QList<std::pair<int,int>> visited;
        int gridOffset=ui->gridOffset->value();
        int gridsize = gridOffset;
        int x = clickedPoint.x();
        int y = clickedPoint.y();
         //qDebug()<<x<<y;
         x = x/gridsize*gridsize ;
         y = y/gridsize*gridsize ;
        // qDebug()<<x<<y;
        colorPoint(x,y,255,255,0,gridOffset);
        // visited.append(std::make_pair(x,y));
        floodfillutil(x,y,255,255,0, visited);
}


void MainWindow::on_DrawPolygon_clicked()
{
    int n=ui->Vertices->value();
    if(points.size()<n)return;
    QVector<QPoint> vertices(points.begin()+points.size()-n,points.end());
    qDebug()<<"polygon";
    for(int i=0;i<n;i++){
        QPoint q1=vertices[i],q2=vertices[(i+1)%n];
        //qDebug()<<q1.x()<<q1.y();
        draw_bresenham_line(q1.x(),q1.y(),q2.x(),q2.y());
    }
    qDebug()<<"polygon";
}

