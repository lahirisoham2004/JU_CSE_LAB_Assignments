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
#define PI 3.14159265358979323846264338327950288419716939937510F
bool axes = false;
bool drawingAxes = false;
bool rotP = false;
bool scP = false;
QPoint rot = {0,0};
QPoint scp = {0,0};
bool refP = false;
int refC = 0;
QVector<QPoint> reflP;
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
    //qDebug() << x<< y<< '\n';
    if(!drawingAxes)
        filled_points.insert({x, y});
    QPixmap canvas=ui->workArea->pixmap();
    QPainter painter(&canvas);
    QPen pen=QPen(QColor(r,g,b),penwidth);
    painter.setPen(pen);
    painter.drawPoint(x, y);
    ui->workArea->setPixmap(canvas);

}

void MainWindow::on_showAxis_clicked() {
    axes = true;
    int gridOffset = (ui->gridOffset->value() == 0) ? 1  : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;
    int axisWidth=ui->gridOffset->value();
    // qDebug()<<width<<height<<centerX<<centerY<<axisWidth;
    // Draw horizontal axis
    drawingAxes = true;
    for (int x = 0; x < width; ++x) {
        colorPoint(x, (centerY - gridOffset/2.0), 255, 0, 0, axisWidth); // Black color
    }

    // Draw vertical axis
    for (int y = 0; y < height; ++y) {
        colorPoint(centerX + gridOffset/2.0, y, 255, 0, 0, axisWidth); // Black color
    }
    drawingAxes = false;
}

void MainWindow::on_gridlines_clicked() {
    int gridOffset = ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    if (gridOffset <= 0) return; // Prevent invalid grid offset
    if(points.size() > 0){
        resetPoly();
    }
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
    on_store_clicked();
    if(points.size()>0){
        on_DrawPoly_clicked();
    }
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
        if(rotP)
            rot = {X, Y};
        else if(scP)
            scp = {X,Y};
        else if(refP){
            refC++;
            reflP.insert(reflP.begin(), {X, Y});
            if(refC == 2){
                refP = false;
                refC = 0;
            }
        }else
            points.insert(points.begin(), {X, Y});
        //qDebug() << X << Y <<'\n';
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
        colorPoint(x.x(), x.y(), 0, 255, 0, gridOffset);
    }
}


void MainWindow::draw_bresenham_line(int x1, int y1, int x2, int y2, int r = 0, int g = 233, int b = 59)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    bool isSteep = dy > dx;  // Check if the slope is steep

    // Swap x and y if the line is steep
    if (isSteep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(dx, dy);
    }

    // Ensure we are drawing from left to right
    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    dx = x2 - x1;
    dy = y2 - y1;

    int xsign = 1;
    int ysign = (dy < 0) ? -1 : 1;
    dy = abs(dy);

    int p = 2 * dy - dx;
    int twoDy = 2 * dy;
    int twoDyDx = 2 * (dy - dx);

    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    int x = x1;
    int y = y1;

    QVector<QPoint> pts;

    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i <= dx; ++i)
    {
        // Map back to original coordinates if steep
        int plotX = isSteep ? y : x;
        int plotY = isSteep ? x : y;

        int xGrid = centerX + plotX * gridOffset + gridOffset / 2;
        int yGrid = centerY - plotY * gridOffset - gridOffset / 2;

        pts.push_back(QPoint(xGrid, yGrid));

        x += 1;  // Since x1 <= x2 after swapping

        if (p < 0)
        {
            p += twoDy;
        }
        else
        {
            y += ysign;
            p += twoDyDx;
        }
    }

    qint64 elapsedTime = timer.nsecsElapsed();
    ui->Bresenham_TIME->setText(QString("Time Taken: ") + QString::number(elapsedTime) + QString(" ns"));

    // Draw the collected points
    for (auto point : pts)
    {
        colorPoint(point.x(), point.y(), r, g, b, gridOffset);
    }
}

void MainWindow::on_setCoord_clicked()
{

}


void MainWindow::on_DDADraw_clicked()
{
    if(points.size()< 2 ) return;
    //qint64 n  = points.size();
    auto coords1 = points[0];
    auto coords2 = points[1];
    draw_dda_line(coords1.x(), coords1.y(), coords2.x(), coords2.y());
    axes = false;
}


void MainWindow::on_store_clicked()
{
    temp = ui->workArea->pixmap(Qt::ReturnByValue);
}


void MainWindow::on_reset_clicked()
{
    ui->workArea->setPixmap(temp);
    points.clear();
    filled_points.clear();
}


void MainWindow::on_pushButton_clicked()
{
    if(points.size()< 2) return;
    //int n = points.size();
    auto coords1 = points[0];
    auto coords2 = points[1];
    draw_bresenham_line(coords1.x(), coords1.y(), coords2.x(), coords2.y());
}

void MainWindow::draw_circle(int xc, int yc, int x, int y, int r=255, int g=0, int b=0){
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    float x_float = static_cast<int>(centerX + x * gridOffset + gridOffset / 2.0);
    float y_float = static_cast<int>(centerY - y * gridOffset - gridOffset / 2.0);

    colorPoint(static_cast<int>(centerX + (xc + x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + y) * gridOffset - gridOffset / 2.0), 255, 255, 0, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc - x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc + x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc - x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc + y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc - y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc + y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc - y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    //delay(100);
}



void MainWindow::draw_circle_parametric(int xc, int yc, int r){
    const float delta = 1.0F/r, thetaLimit = PI/4;
    QElapsedTimer timer;
    timer.start();
    for(float theta = 0; theta <= thetaLimit; theta += delta){
        draw_circle(xc, yc, int(r*std::cos(theta) + 0.5), int(r*std::sin(theta) + 0.5), 0, 255, 0);
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->c_para_time->setText(QString::number(elapsedTime) + QString(" ns"));
}


void MainWindow::on_parametric_clicked()
{
    int r = ui->radius->value();
    if(r<=0) return;
    auto centre = points[0];
    draw_circle_parametric(centre.x(),centre.y(),r);
}


void MainWindow::draw_circle_sqrt(int xc, int yc, int r){
    int x = 0, y;
    QElapsedTimer timer;
    timer.start();
    do{
        y = int(std::sqrt(float(r*r - x*x))+ 0.5);
        draw_circle(xc, yc, x, y, 0, 0 ,255);
        x++;
    }while(x<y);
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->c_sqrt_time->setText(QString::number(elapsedTime) + QString(" ns"));
}

void MainWindow::on_sqrt_btn_clicked()
{
    int r = ui->radius->value();
    if(r<=0) return;
    auto centre = points[0];
    draw_circle_sqrt(centre.x(),centre.y(),r);
}


void MainWindow::draw_circle_bresenham(int xc, int yc, int r){
    qDebug() << xc << yc << r << '\n';
    int x = 0, y = r;
    int d = 1 - r;
    draw_circle(xc, yc, x, y,255,0,0);
    QElapsedTimer timer;
    timer.start();
    while(y>=x){
        x++;
        if(d >= 0){
            y--;
            d += 2*x + 1 - 2*y;
        }else
            d += 2*x + 1;

        draw_circle(xc, yc , x, y);
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->c_bres_time->setText(QString::number(elapsedTime) + QString(" ns"));
}

void MainWindow::on_bresenham_circle_clicked()
{
    int r = ui->radius->value();
    if(r<=0) return;
    auto centre = points[0];
    draw_circle_bresenham(centre.x(),centre.y(),r);
}

void MainWindow::draw_ellipse(int xc, int yc, int x, int y, int r=255, int g=0, int b=0){
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    //float x_float = static_cast<int>(centerX + x * gridOffset + gridOffset / 2.0);
    //float y_float = static_cast<int>(centerY - y * gridOffset - gridOffset / 2.0);

    colorPoint(static_cast<int>(centerX + (xc + x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc - x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc + x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    colorPoint(static_cast<int>(centerX + (xc - x) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - y) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    //colorPoint(static_cast<int>(centerX + (xc + y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    //colorPoint(static_cast<int>(centerX + (xc - y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc + x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    //colorPoint(static_cast<int>(centerX + (xc + y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    //colorPoint(static_cast<int>(centerX + (xc - y) * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - (yc - x) * gridOffset - gridOffset / 2.0), r, g, b, gridOffset);
    //delay(50);
}

void MainWindow::draw_ellipse_sqrt(int xc, int yc, int a, int b){
    int x = 0, y;
    QElapsedTimer timer;
    timer.start();
    do{
        y = int(std::sqrt(float(a*a - x*x))*float(b)/float(a)+ 0.5);
        draw_ellipse(xc, yc, x, y, 0, 0 ,255);
        x++;
    }while(x<=a);
    y = 0;
    do{
        x = int(std::sqrt(float(b*b - y*y))*float(a)/float(b)+ 0.5);
        draw_ellipse(xc, yc, x, y, 0, 0 ,255);
        y++;
    }while(y<=b);
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->e_sqrt_time->setText(QString::number(elapsedTime) + QString(" ns"));
}

void MainWindow::on_e_sqrt_clicked()
{
    int a = ui->major_axis->value();
    int b = ui->minoraxis->value();
    if(a<0 || b<0 || points.size() == 0) return;
    auto centre = points[0];
    draw_ellipse_sqrt(centre.x(), centre.y(), a, b);
}


void MainWindow::draw_ellipse_para(int xc, int yc, int a, int b){
    const float thetaLimit = PI/2;
    float theta = 0, sin_theta, cos_theta;
    QElapsedTimer timer;
    timer.start();
    while(theta<=thetaLimit){
        cos_theta = std::cos(theta);
        sin_theta = std::sin(theta);
        draw_ellipse(xc, yc, int(a*cos_theta+0.5), int(b*sin_theta+0.5));
        theta += 1.0F/(std::sqrt(a*a + b*b));
    }
    draw_ellipse(xc, yc, 0, b);
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->e_para_time->setText(QString::number(elapsedTime) + QString(" ns"));
}

void MainWindow::on_e_parametric_clicked()
{
    int a = ui->major_axis->value();
    int b = ui->minoraxis->value();
    if(a<0 || b<0 || points.size() == 0) return;
    auto centre = points[0];
    draw_ellipse_para(centre.x(), centre.y(), a, b);
}

void MainWindow::draw_ellipse_bresenham(int xc, int yc, int a, int b){
    int x = 0;
    int y = b;
    float d1 = b*b - (a*a*b) + (0.25 * a * a);
    float d2;
    draw_ellipse(xc, yc, x, y, 255, 255, 0);
    QElapsedTimer timer;
    timer.start();
    while((a*a)*(y-0.5) > (b*b)*(x+1)){
        if(d1 < 0){
            d1 += (b*b)*(2*x+3);
        }else{
            d1 += (b*b)*(2*x+3) + (a*a)*(2 - 2*y);
            y--;
        }
        x++;
        draw_ellipse(xc, yc, x, y, 255, 255);
        //delay(50);
    }

    d2 = (b*b)*(x + 0.5)*(x + 0.5) + (a*a)*(y-1)*(y-1) - a*a*b*b;
    while(y>0){
        if(d2 < 0){
            d2 += (b*b)*(2*x + 2) + (a*a)*(3 - 2*y);
            x++;
        }else{
            d2 += (a*a)*(3 - 2*y);
        }
        y--;
        draw_ellipse(xc, yc, x, y, 255, 0);
        //delay(50);
    }
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->e_bre_time->setText(QString::number(elapsedTime) + QString(" ns"));
}

void MainWindow::on_e_bresenham_clicked()
{
    int a = ui->major_axis->value();
    int b = ui->minoraxis->value();
    if(a<0 || b<0 || points.size() == 0) return;
    auto centre = points[0];
    draw_ellipse_bresenham(centre.x(), centre.y(), a, b);
}


void MainWindow::onDrawPoly(int r=0, int g=233, int b=59){
    int sides = ui->noOfSides->value();
    if(sides < 3 || sides > points.size()) return;
    for(int i = 0; i<sides-1; i++){
        auto coord1 = points[i];
        auto coord2 = points[i+1];
        draw_bresenham_line(coord1.x(), coord1.y(), coord2.x(), coord2.y());
    }
    auto coord1 = points[sides-1];
    auto coord2 = points[0];
    draw_bresenham_line(coord1.x(), coord1.y(), coord2.x(), coord2.y(), r, g, b);
}


void MainWindow::on_DrawPoly_clicked()
{
    onDrawPoly();
    // int sides = ui->noOfSides->value();
    // if(sides < 3 || sides > points.size()) return;
    // for(int i = 0; i<sides-1; i++){
    //     auto coord1 = points[i];
    //     auto coord2 = points[i+1];
    //     draw_bresenham_line(coord1.x(), coord1.y(), coord2.x(), coord2.y());
    // }
    // auto coord1 = points[sides-1];
    // auto coord2 = points[0];
    // draw_bresenham_line(coord1.x(), coord1.y(), coord2.x(), coord2.y(), r, g, b);
}

void MainWindow::fill(int x, int y){
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    if(filled_points.contains({x, y}) || x>width || y>height || x<0 || y<0){
        return;
    }
    //qDebug()<<x<<y<<'\n';
    colorPoint(x, y, ui->r->value(), ui->g->value(), ui->b->value(), gridOffset);
    delay(10);
    fill(x+gridOffset, y);
    fill(x-gridOffset, y);
    fill(x, y+gridOffset);
    fill(x, y-gridOffset);
}

void MainWindow::fill8(int x, int y){
    // int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int gridOffset = ui -> gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    if(filled_points.contains({x, y}) || x>width || y>height || x<0 || y<0){
        return;
    }
    //qDebug()<<x<<y<<'\n';
    colorPoint(x, y, ui->r->value(), ui->g->value(), ui->b->value(), gridOffset);
    delay(10);

    fill8(x-gridOffset, y-gridOffset);
    fill8(x+gridOffset, y+gridOffset);
    fill8(x+gridOffset, y-gridOffset);
    fill8(x-gridOffset, y+gridOffset);

    fill8(x+gridOffset, y);
    fill8(x-gridOffset, y);
    fill8(x, y+gridOffset);
    fill8(x, y-gridOffset);

}

void MainWindow::on_fillP_clicked()
{

    int xc = points[0].x();
    int yc = points[0].y();
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;
    int x = centerX + xc * gridOffset + gridOffset / 2;
    int y = centerY - yc * gridOffset + gridOffset / 2;

    //colorPoint(x-gridOffset, y-2*gridOffset, 0, 0, 0, gridOffset);

    // for(auto s: filled_points){
    //     qDebug() <<"s"<< s.x()<<s.y()<<'\n';
    // }
    QElapsedTimer timer;
    timer.start();
    if(ui->radioButton->isChecked()){
        qDebug() << "here\n";
        fill8(x, y);
    }
    else
        fill(x, y);
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->ff_time->setText(QString::number(elapsedTime) + QString(" ns"));
}


void MainWindow::scanlineFill(int size)
{
    // int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    // int width = ui->workArea->width();
    // int height = ui->workArea->height();
    // int centerX = width / 2;
    // int centerY = height / 2;

    // if (points.size() < 3) return; // Ensure at least 3 points to form a polygon

    // // Get the bounding box of the polygon
    // QElapsedTimer timer;
    // timer.start();
    // int minY = INT_MAX, maxY = INT_MIN;
    // for (const auto& point : points) {
    //     if(size<=0) break;
    //     minY = std::min(minY, point.y());
    //     maxY = std::max(maxY, point.y());
    //     size--;
    // }
    // // Scan each horizontal line between the minY and maxY
    // for (int y = minY+1; y < maxY-1; y+=1) {
    //     std::vector<int> intersections;

    //     // Find intersections of the scanline with the polygon edges
    //     for (int i = 0; i < points.size(); i++) {
    //         QPoint p1 = points[i];
    //         QPoint p2 = points[(i + 1) % points.size()]; // Get the next point
    //         int tx1 = p1.x();
    //         int ty1 = p1.y();
    //         int tx2 = p2.x();
    //         int ty2 = p2.y();

    //         // Check if the scanline intersects with the edge
    //         if ((ty1 <= y && ty2 > y) || (ty2 <= y && ty1 > y)) {
    //             // Calculate the intersection point on this edge
    //             int x = tx1 + (y - ty1) * (tx2 - tx1) / (ty2 - ty1);
    //             // if(x != p1.x() && x!=p2.x())

    //             intersections.push_back(x);
    //             qDebug() << x<<y<<'\n';
    //         }
    //     }

    //     // Sort intersections by x coordinate
    //     std::sort(intersections.begin(), intersections.end());

    //     // Fill between pairs of intersections

    //     for (size_t i = 0; i < intersections.size(); i += 2) {
    //         if (i + 1 < intersections.size()) {
    //             int xStart = intersections[i]+1;
    //             int xEnd = intersections[i + 1];

    //             // Fill between xStart and xEnd on the current scanline y
    //             for (int x = xStart; x < xEnd; x+=1) {
    //                 //int nx = centerX + x * gridOffset + gridOffset / 2;
    //                 // int ny = centerY - (y) * gridOffset - gridOffset / 2;
    //                 //int nx = static_cast<int>((x - centerX-gridOffset/2)/gridOffset);
    //                 //int ny = static_cast<int>((centerY - y-gridOffset/2)/gridOffset);
    //                 if(!filled_points.contains({static_cast<int>(centerX + (x) * gridOffset+gridOffset/2),static_cast<int>(centerY - (y) * gridOffset - gridOffset / 2.0)})){
    //                     colorPoint(static_cast<int>(centerX + (x) * gridOffset+gridOffset/2), static_cast<int>(centerY - (y) * gridOffset - gridOffset / 2.0), ui->r->value(), ui->g->value(), ui->b->value(), gridOffset); // Fill with green color
    //                     //colorPoint(x, y, ui->r->value(), ui->g->value(), ui->b->value(), gridOffset);
    //                     delay(5);
    //                 }else{
    //                     qDebug() << "here\n";
    //                 }
    //             }
    //         }
    //     }
    //     qint64 elapsedTime = timer.nsecsElapsed();
    //     ui->sf_time->setText(QString::number(elapsedTime) + QString(" ns"));
    // }
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    if (points.size() < 3) return; // Ensure at least 3 points to form a polygon

    // Get the bounding box of the polygon
    QElapsedTimer timer;
    timer.start();
    int minY = INT_MAX, maxY = INT_MIN;
    for (const auto& point : points) {
        if(size<=0) break;
        int temp_y = static_cast<int>(centerY - (point.y()) * gridOffset - gridOffset / 2.0);
        minY = std::min(minY, temp_y);
        maxY = std::max(maxY, temp_y);
        size--;
    }
    // Scan each horizontal line between the minY and maxY
    for (int y = minY; y < maxY; y+=gridOffset) {
        std::vector<int> intersections;

        // Find intersections of the scanline with the polygon edges
        for (int i = 0; i < points.size(); i++) {
            QPoint p1 = points[i];
            QPoint p2 = points[(i + 1) % points.size()]; // Get the next point
            int tx1 = static_cast<int>(centerX + p1.x() * gridOffset + gridOffset / 2.0);
            int ty1 = static_cast<int>(centerY - p1.y() * gridOffset - gridOffset / 2.0);
            int tx2 = static_cast<int>(centerX + p2.x() * gridOffset + gridOffset / 2.0);
            int ty2 = static_cast<int>(centerY - p2.y() * gridOffset - gridOffset / 2.0);

            // Check if the scanline intersects with the edge
            if ((ty1 <= y && ty2 > y) || (ty2 <= y && ty1 > y)) {
                // Calculate the intersection point on this edge
                int x = tx1 + (y - ty1) * (tx2 - tx1) / (ty2 - ty1);
                // if(x != p1.x() && x!=p2.x())

                intersections.push_back(x);
                qDebug() << x<<y<<'\n';
            }
        }

        // Sort intersections by x coordinate
        std::sort(intersections.begin(), intersections.end());

        // Fill between pairs of intersections

        for (size_t i = 0; i < intersections.size(); i += 2) {
            if (i + 1 < intersections.size()) {
                int xStart = intersections[i]+gridOffset/2;
                int xEnd = intersections[i + 1];

                int g = 1;
                // Fill between xStart and xEnd on the current scanline y
                for (int x = xStart; x < xEnd; x+=gridOffset/2) {
                    //int nx = centerX + x * gridOffset + gridOffset / 2;
                    // int ny = centerY - (y) * gridOffset - gridOffset / 2;
                    int nx = static_cast<int>((x - centerX-gridOffset/2)/gridOffset);
                    int ny = static_cast<int>((centerY - y-gridOffset/2)/gridOffset);
                    if(!filled_points.contains({static_cast<int>(centerX + nx * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - ny * gridOffset - gridOffset/2.0)})){
                    //if(!filled_points.contains({x, y})){
                        colorPoint(static_cast<int>(centerX + nx * gridOffset + gridOffset / 2.0), static_cast<int>(centerY - ny * gridOffset - gridOffset/2.0), ui->r->value(), ui->g->value(), ui->b->value(), gridOffset); // Fill with green color
                        //colorPoint(x, y, ui->r->value(), ui->g->value(), ui->b->value(), gridOffset);
                        delay(5);
                        //g = gridOffset;

                    }else{
                        qDebug() << "here\n";
                    }
                }
            }
        }
        qint64 elapsedTime = timer.nsecsElapsed();
        ui->sf_time->setText(QString::number(elapsedTime) + QString(" ns"));
    }

}

void MainWindow::on_scanlineP_clicked()
{
    qDebug() << "here"<<'\n';
    int sides = ui->noOfSides->value();
    if(sides < 3 || sides > points.size()) return;
    scanlineFill(sides);
}


void MainWindow::boundaryFill(int x, int y){
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    QImage img = ui->workArea->pixmap(Qt::ReturnByValue).toImage();
    QColor clr = img.pixelColor(x, y).toRgb();

    //QColor clr1(0, 255, 0);
    QColor cl(qRgb(0, 233, 59));
    QColor cl2(qRgb(ui->r->value(), ui->g->value(), ui->b->value()));
    //qDebug()<<clr << cl << cl2 << QPoint(x,y);
    qDebug() << clr.red() <<clr.green()<<clr.blue();
    if((clr.red()==cl.red() && clr.blue()==cl.blue() && clr.green()==cl.green()) || x>width || y>height || x<0 || y<0 || clr == cl2){
        return;
    }
    //qDebug()<<x<<y<<'\n';
    colorPoint(x, y, ui->r->value(), ui->g->value(), ui->b->value(), gridOffset);
    delay(10);
    boundaryFill(x+gridOffset, y);
    boundaryFill(x-gridOffset, y);
    boundaryFill(x, y+gridOffset);
    boundaryFill(x, y-gridOffset);
}


void MainWindow::on_bFill_clicked()
{
    int xc = points[0].x();
    int yc = points[0].y();
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;
    int x = centerX + xc * gridOffset + gridOffset / 2;
    int y = centerY - yc * gridOffset + gridOffset / 2;
    qDebug() << "here\n";
    QElapsedTimer timer;
    timer.start();
    boundaryFill(x, y);
    qint64 elapsedTime = timer.nsecsElapsed();
    ui->ff_time->setText(QString::number(elapsedTime) + QString(" ns"));
}



void MainWindow::resetPoly(){
    ui->workArea->setPixmap(temp);
    filled_points.clear();
    if(axes) {on_showAxis_clicked();}
}

void MainWindow::on_translateO_clicked()
{
    int tx = ui->x_translateO->value();
    int ty = ui->y_translateO->value();
    int sides = ui->noOfSides->value();
    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x();
        int y = p.y();
        x+=tx;
        y+=ty;

        points[i] = {x,y};
    }
    on_DrawPoly_clicked();
}


void MainWindow::on_reflectx_clicked()
{
    int sides = ui->noOfSides->value();

    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x();
        int y = p.y();
        y*=-1;

        points[i] = {x,y};
    }
    on_DrawPoly_clicked();
}


void MainWindow::on_reflecty_clicked()
{
    int sides = ui->noOfSides->value();
    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x();
        int y = p.y();
        x*=-1;

        points[i] = {x,y};
    }
    on_DrawPoly_clicked();
}


void MainWindow::on_scaleO_clicked()
{
    float sx = ui->x_scaleO->value();
    float sy = ui->y_scaleO->value();
    int sides = ui->noOfSides->value();
    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x()-scp.x();
        int y = p.y()-scp.y();
        if(sx!=0)
            x*=sx;
        if(sy!=0)
            y*=sy;
        points[i] = {x+scp.x(),y+scp.y()};
    }
    on_DrawPoly_clicked();
}


void MainWindow::on_rotateO_clicked()
{
    int theta = ui->angleO->value();
    float rTheta = (PI/180)*theta;
    float sinTheta = std::sin(rTheta);
    float cosTheta = std::cos(rTheta);
    qDebug() << sinTheta << cosTheta;
    int sides = ui->noOfSides->value();
    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x() - rot.x();
        int y = p.y() - rot.y();
        //qDebug() << x << y;
        int x1 = static_cast<int>(x*cosTheta - y*sinTheta);
        int y1 = static_cast<int>(y*cosTheta + x*sinTheta);

        points[i] = {x1 + rot.x(),y1 + rot.y()};
    }
    on_DrawPoly_clicked();
}


void MainWindow::on_shearO_clicked()
{
    int sx = ui->x_shearO->value();
    int sy = ui->y_shearO->value();
    int sides = ui->noOfSides->value();
    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x();
        int y = p.y();
        int x1 = x + sx*y;
        int y1 = y + sy*x;
        points[i] = {x1,y1};
    }
    on_DrawPoly_clicked();
}


void MainWindow::on_setRotP_clicked()
{
    rotP = true;
}


void MainWindow::on_setScaleP_clicked()
{
    scP = true;
}


void MainWindow::on_setLineRef_clicked()
{
    refP = true;
}


void MainWindow::on_pushButton_2_clicked()
{
    if(reflP.size() <2) return;
    QPoint p1 = reflP[0];
    QPoint p2 = reflP[1];
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();
    int a = y2-y1;
    int b = x1-x2;
    int c = -(a*x1 + b*y1);
    int sides = ui->noOfSides->value();
    resetPoly();
    // onDrawPoly(255,255,255);
    for(int i = 0; i<sides; i++){
        QPoint p = points[i];
        int x = p.x();
        int y = p.y();
        int xx = static_cast<int>(static_cast<float>(x*(b*b-a*a) - 2*y*a*b-2*a*c)/static_cast<float>(a*a+b*b) + 0.5);
        int yy = static_cast<int>(static_cast<float>(y*(a*a-b*b) - 2*x*a*b-2*b*c)/static_cast<float>(a*a+b*b) + 0.5);
        points[i] = {xx,yy};
    }
    on_DrawPoly_clicked();
}

