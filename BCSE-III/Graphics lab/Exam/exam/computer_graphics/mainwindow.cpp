#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <chrono>
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#define Delay delay(1)
#define PI 3.14159265
#include <QPoint>
#include <QStack>

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
    QPainter painter(&canvas);
    QPen pen=QPen(QColor(r,g,b),penwidth);
    painter.setPen(pen);
    painter.drawPoint(x, y);
    ui->workArea->setPixmap(canvas);
}

void MainWindow::plotPoint(int x, int y, int r=0, int g=0, int b=255) {
    QPixmap canvas=ui->workArea->pixmap();
    QPainter painter(&canvas);
    int penwidth = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
    QPen pen=QPen(QColor(r,g,b),penwidth);
    painter.setPen(pen);
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2;
    int centerY=height/2;
    int tempX = centerX + penwidth/2;
    int tempY = centerY - penwidth/2;
    int actualX = tempX + x*penwidth;
    int actualY = tempY - y*penwidth;
    painter.drawPoint(actualX, actualY);
    ui->workArea->setPixmap(canvas);
}

void MainWindow::draw_axes(){
    QPixmap canvas=ui->workArea->pixmap();
    QPainter painter(&canvas);
    int penwidth = (ui->axisWidth->value()==0)?1:ui->axisWidth->value();
    QPen pen=QPen(QColor(255,0,0),penwidth);
    painter.setPen(pen);
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2+penwidth/2;
    int centerY=height/2-penwidth/2;
    painter.drawLine(centerX, 0, centerX, height);
    painter.drawLine(0, centerY, width, centerY);
    ui->workArea->setPixmap(canvas);
}

QPoint MainWindow::getCenter() {
    QPoint center = pixels.top();
    // pixels.pop();
    return center;
}

void MainWindow::on_setPoint_clicked() {
    int gridOffset = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2;
    int centerY=height/2;
    QPoint temp_point;
    temp_point.setX(floor((clickedPoint.x()-centerX)*1.0/gridOffset));
    temp_point.setY(floor((centerY-clickedPoint.y())*1.0/gridOffset));
    pixels.push(temp_point);
    qDebug() << "Point is set at" << temp_point.x()<<","<<temp_point.y();
}

void MainWindow::drawLine(QPoint p1, QPoint p2, int r = 0, int g = 0, int b = 255) {
    int xa = p1.x();
    int ya = p1.y();
    int xb = p2.x();
    int yb = p2.y();
    std::vector<std::pair<int, int>> points;
    int dx = abs(xb - xa);
    int dy = abs(yb - ya);
    // int p = 2 * dy - dx;
    int p = 0;
    if (dx > dy)
        p = 2 * dy - dx;
    else
        p = 2 * dx - dy;
    int twoDy = 2 * dy;
    int twoDyDx = 2 * (dy - dx);

    // Determine the step direction for both x and y
    int xStep = (xa < xb) ? 1 : -1;  // Direction in the x-axis
    int yStep = (ya < yb) ? 1 : -1;  // Direction in the y-axis

    int x = xa;
    int y = ya;

    // Adjust for slope (whether x changes faster or y changes faster)
    if (dx >= dy) {
        // When dx is greater or equal to dy (gentle slope)
        while (x != xb) {
            points.push_back({x, y});
            x += xStep;
            if (p < 0) {
                p += twoDy;
            } else {
                y += yStep;
                p += twoDyDx;
            }
        }
    } else {
        // When dy is greater than dx (steep slope)
        p = 2 * dx - dy;
        int twoDx = 2 * dx;
        int twoDxDy = 2 * (dx - dy);

        while (y != yb) {
            points.push_back({x, y});
            y += yStep;
            if (p < 0) {
                p += twoDx;
            } else {
                x += xStep;
                p += twoDxDy;
            }
        }
    }
    points.push_back({xb, yb});
    for (long long unsigned i=0; i<points.size(); i++) {
        std::pair<int, int> point = points[i];
        plotPoint(point.first, point.second,r,g,b);
        delay(50);
    }
}

void MainWindow::drawBresenhamLine(QPoint p1, QPoint p2, int r = 0, int g = 0, int b = 255) {
    int xa = p1.x();
    int ya = p1.y();
    int xb = p2.x();
    int yb = p2.y();
    std::vector<std::pair<int, int>> points;
    int dx = abs(xb - xa);
    int dy = abs(yb - ya);
    // int p = 2 * dy - dx;
    int p = 0;
    if (dx > dy)
        p = 2 * dy - dx;
    else
        p = 2 * dx - dy;
    int twoDy = 2 * dy;
    int twoDyDx = 2 * (dy - dx);

    // Determine the step direction for both x and y
    int xStep = (xa < xb) ? 1 : -1;  // Direction in the x-axis
    int yStep = (ya < yb) ? 1 : -1;  // Direction in the y-axis

    int x = xa;
    int y = ya;

    // Adjust for slope (whether x changes faster or y changes faster)
    if (dx >= dy) {
        // When dx is greater or equal to dy (gentle slope)
        while (x != xb) {
            points.push_back({x, y});
            x += xStep;
            if (p < 0) {
                p += twoDy;
            } else {
                y += yStep;
                p += twoDyDx;
            }
        }
    } else {
        // When dy is greater than dx (steep slope)
        p = 2 * dx - dy;
        int twoDx = 2 * dx;
        int twoDxDy = 2 * (dx - dy);

        while (y != yb) {
            points.push_back({x, y});
            y += yStep;
            if (p < 0) {
                p += twoDx;
            } else {
                x += xStep;
                p += twoDxDy;
            }
        }
    }

    // Add the last point (xb, yb)
    points.push_back({xb, yb});
    for (long long unsigned i=0; i<points.size(); i++) {
        std::pair<int, int> point = points[i];
        plotPoint(point.first, point.second,r,g,b);
        polygon << QPoint(point.first, point.second);
        delay(50);
    }
}

void MainWindow::draw_Window(){
    p1.setX(clipperPoints[0][0]);
    p1.setY(clipperPoints[0][1]);
    p2.setX(clipperPoints[1][0]);
    p2.setY(clipperPoints[1][1]);
    drawLine(p1,p2);

    p1.setX(clipperPoints[1][0]);
    p1.setY(clipperPoints[1][1]);
    p2.setX(clipperPoints[2][0]);
    p2.setY(clipperPoints[2][1]);
    drawLine(p1,p2);

    p1.setX(clipperPoints[2][0]);
    p1.setY(clipperPoints[2][1]);
    p2.setX(clipperPoints[3][0]);
    p2.setY(clipperPoints[3][1]);
    drawLine(p1,p2);

    p1.setX(clipperPoints[3][0]);
    p1.setY(clipperPoints[3][1]);
    p2.setX(clipperPoints[0][0]);
    p2.setY(clipperPoints[0][1]);
    drawLine(p1,p2);
}

void MainWindow::on_drawWindow_clicked(){
    cp2 = pixels.top();
    pixels.pop();
    cp1 = pixels.top();
    pixels.pop();
    if (cp2.y() < cp1.y() && cp2.x() < cp2.y()){
        QPoint temp;
        temp = cp1;
        cp1 = cp2;
        cp2 = temp;
    }
    clipperPoints[0][0] = cp1.x();
    clipperPoints[0][1] = cp1.y();
    clipperPoints[1][0] = cp1.x();
    clipperPoints[1][1] = cp2.y();
    clipperPoints[2][0] = cp2.x();
    clipperPoints[2][1] = cp2.y();
    clipperPoints[3][0] = cp2.x();
    clipperPoints[3][1] = cp1.y();
    draw_Window();
}

void MainWindow::on_drawLine_clicked(){
    p1 = pixels.top();
    pixels.pop();
    p2 = pixels.top();
    pixels.pop();
    drawLine(p1,p2);
    lines << std::make_pair(p1,p2);
}

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000


// Function to compute region code for a point(x, y)
int MainWindow::computeCode(int xa, int ya){
    int x_min=cp1.x(),x_max=cp2.x(),y_min=cp1.y(),y_max=cp2.y();

    // initialized as being inside
    int code = INSIDE;
    if (xa < x_min)       // to the left of rectangle
        code |= LEFT;
    else if (xa > x_max)  // to the right of rectangle
        code |= RIGHT;
    if (ya < y_min)       // below the rectangle
        code |= BOTTOM;
    else if (ya > y_max)  // above the rectangle
        code |= TOP;

    return code;
}

void MainWindow::cohenSutherlandClip(int x1, int y1,int x2, int y2){
    int x_min = std::min(clipperPoints[0][0], clipperPoints[2][0]);
    int x_max = std::max(clipperPoints[0][0], clipperPoints[2][0]);
    int y_min = std::min(clipperPoints[0][1], clipperPoints[2][1]);
    int y_max = std::max(clipperPoints[0][1], clipperPoints[2][1]);
    qDebug() << x_min << x_max << y_min << y_max;
    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    qDebug() << code1 << code2 ;
    // Initialize line as outside the rectangular window
    bool accept = false;
    while (true){
        qDebug() << "Stuck in loop!";
        if ((code1 == 0) && (code2 == 0)){
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }else if (code1 & code2){
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }else{
            // Some segment of line lies within the
            // rectangle
            qDebug() << "I am here";
            int code_out;
            int x, y;
            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP){
                // point is above the clip rectangle
                x = x1 + (int)((double)(x2 - x1) *(double)(y_max - y1) /(double)(y2 - y1));
                y = y_max;
            }else if (code_out & BOTTOM){
                // point is below the rectangle
                x = x1 + (int)((double)(x2 - x1) * (double)(y_min - y1) / (double)(y2 - y1));
                y = y_min;
            }else if (code_out & RIGHT){
                // point is to the right of rectangle
                y = y1 + (int)((double)(y2 - y1) * (double)(x_max - x1) / (double)(x2 - x1));
                x = x_max;
            }else if (code_out & LEFT){
                // point is to the left of rectangle
                y = y1 + (int)((double)(y2 - y1) * (double)(x_min - x1) / (double)(x2 - x1));
                x = x_min;
            }
            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1){
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }else{
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
            qDebug() << code1 << code2 << x1 <<y1 << x2 << y2;
        }
    }
    if (accept){
        //If accepted
        //Just reset and draw the boundary and the line
        //Reset the screen and draw the grid
        p1.setX(x1);
        p1.setY(y1);
        p2.setX(x2);
        p2.setY(y2);
        drawLine(p1,p2, 0, 255, 0);
        draw_Window();
    }else{
        //If not accepted
        //Just reset and draw the boundary
        //Reset the screen and draw the grid
        draw_Window();
    }
}

void MainWindow::on_lineClip_clicked(){
    for (std::pair<QPoint, QPoint>& p : lines) {
        p1.setX(p.first.x());
        p1.setY(p.first.y());
        p2.setX(p.second.x());
        p2.setY(p.second.y());

        drawLine(p1,p2,255,255,255);
        on_gridlines_clicked();
        on_showAxis_clicked();
        cohenSutherlandClip(p1.x(),p1.y(),p2.x(),p2.y());
        //cohenSutherlandClip(p.first.first, p.first.second, p.second.first, p.second.second);
    }
}

std::vector<std::pair<int, int>> vertex_list;

// Returns x-value of point of intersection of two lines
int MainWindow::x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4){
    y1=ui->workArea->height()-y1+1;
    y2=ui->workArea->height()-y2+1;
    y3=ui->workArea->height()-y3+1;
    y4=ui->workArea->height()-y4+1;
    int num = (x1*y2 - y1*x2) * (x3-x4) -  (x1-x2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    int retx=num/den;
    return retx;
}

// Returns y-value of point of intersection of two lines
int MainWindow::y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4){
    y1=ui->workArea->height()-y1+1;
    y2=ui->workArea->height()-y2+1;
    y3=ui->workArea->height()-y3+1;
    y4=ui->workArea->height()-y4+1;
    int num = (x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    int rety= (ui->workArea->height()-num/den+1);
    return rety;
}

// This functions clips all the edges w.r.t one clip edge of clipping area
void MainWindow::clip(int x1, int y1, int x2, int y2){
    qDebug() << "Inside individual clip";
    if (vertex_list.size() == 0) return;
    int poly_size=vertex_list.size();
    int new_poly_size = 0;

    std::vector<std::pair<int,int>> new_points;

    // (ix,iy),(kx,ky) are the co-ordinate values of
    // the points
    for (int i = 0; i < poly_size; i++){
        // i and k form a line in polygon
        int k = (i+1) % poly_size;
        int ix = vertex_list[i].first, iy = vertex_list[i].second;
        int kx = vertex_list[k].first, ky = vertex_list[k].second;
        // Calculating position of first and second point

        int i_pos,k_pos;
        if(x2==x1 && ix>x1) i_pos=1;
        else if(x2==x1 && ix<x1) i_pos=-1;
        else if(y2==y1 && iy<y1) i_pos=1;
        else i_pos=-1;

        if(x2==x1 && kx>x1) k_pos=1;
        else if(x2==x1 && kx<x1) k_pos=-1;
        else if(y2==y1 && ky<y1) k_pos=1;
        else k_pos=-1;

        if(y1>y2||x1>x2) {
            i_pos=(-1)*i_pos;
            k_pos=(-1)*k_pos;
        }

        // Case 1 : When both points are inside
        if (i_pos >= 0  && k_pos >= 0) {
            //Only second point is added
            new_points.push_back(std::make_pair(kx,ky));
            new_poly_size++;
        }

        // Case 2: When only first point is outside
        else if (i_pos < 0  && k_pos >= 0) {
            // Point of intersection with edge
            // and the second point is added

            new_points.push_back(std::make_pair(x_intersect(x1,y1, x2, y2, ix, iy, kx, ky),y_intersect(x1,y1, x2, y2, ix, iy, kx, ky)));
            new_poly_size++;

            new_points.push_back(std::make_pair(kx,ky));
            new_poly_size++;
        }

        // Case 3: When only second point is outside
        else if (i_pos >= 0  && k_pos < 0) {
            //Only point of intersection with edge is added
            new_points.push_back(std::make_pair(x_intersect(x1,y1, x2, y2, ix, iy, kx, ky),y_intersect(x1,y1, x2, y2, ix, iy, kx, ky)));
            new_poly_size++;
        }

        // Case 4: When both points are outside
        else {
            //No points are added
            //continue;
        }
    }
    // Copying new points into original array
    // and changing the no. of vertices
    //poly_size = new_poly_size;
    vertex_list.clear();
    for (int i = 0; i < int(new_points.size()); i++) {
        vertex_list.push_back(new_points[i]);
    }
    vertex_list.push_back(new_points[0]);
}

// Implements Sutherland–Hodgman algorithm
void MainWindow::suthHodgClip(){
    qDebug() << "Inside Clipping function";
    std::vector<std::pair<int,int> > old_vertex;
    for(long long int i=0;i<vertex.size();i++)  {
        old_vertex.push_back(std::make_pair(vertex[i].x(), vertex[i].y()));
        vertex_list.push_back(std::make_pair(vertex[i].x(), vertex[i].y()));
    }
    int clipper_size=4;
    //i and k are two consecutive indexes
    for (int i=0; i<clipper_size; i++){
        int k = (i+1) % clipper_size;
        // We pass the current array of vertices, it's size
        // and the end points of the selected clipper line
        clip(clipperPoints[i][0],
             clipperPoints[i][1], clipperPoints[k][0],
             clipperPoints[k][1]);
    }
    poly_draw_list(old_vertex,255,255,255);
    on_gridlines_clicked();
    on_showAxis_clicked();
    draw_Window();
    poly_draw_list(vertex_list,0,255,0);
}

void MainWindow::on_polygonClip_clicked(){
    suthHodgClip();
}

void MainWindow::drawPolygon(int vertex_count) {
    std::vector<QPoint> vertices;
    vertex.clear();
    while(!pixels.empty() && vertex_count > 0) {
        vertices.push_back(pixels.top());
        vertex << pixels.top();
        pixels.pop();
        vertex_count--;
    }
    if (vertices.size() < 3) {
        qDebug() << "Polygon cannot be constructed" ;
    }
    for (long long unsigned int i=0 ; i<vertices.size(); i++) {
        drawBresenhamLine(vertices[i], vertices[(i+1)%vertices.size()]);
    }
}

void MainWindow::poly_draw(QList<QPoint> polygon, int r=0, int g=0, int b=255) {
    for (long long int i=0 ; i<polygon.size(); i++) {
        drawBresenhamLine(polygon[i], polygon[(i+1)%polygon.size()], r, g, b);
    }
}

void MainWindow::poly_draw_list(std::vector<std::pair<int,int>> polygon, int r=0, int g=0, int b=255) {
    for (long long int i=0 ; i<polygon.size(); i++) {
        QPoint temp1;
        QPoint temp2;
        temp1.setX(polygon[i].first);
        temp1.setY(polygon[i].second);
        temp2.setX(polygon[(i+1)%polygon.size()].first);
        temp2.setY(polygon[(i+1)%polygon.size()].second);
        drawLine(temp1, temp2, r, g, b);
    }
}

void MainWindow::on_drawPolygon_clicked() {
    int vertex_count = ui->vertexCount->value();
    drawPolygon(vertex_count);
}

void MainWindow::on_showAxis_clicked() {
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;
    int axisWidth=ui->axisWidth->value();
    qDebug()<<width<<height<<centerX<<centerY<<axisWidth;
    // Draw horizontal axis
    // for (int x = 0; x < width; ++x) {
    //     colorPoint(x, centerY, 255, 0, 0, axisWidth); // Black color
    // }

    // // Draw vertical axis
    // for (int y = 0; y < height; ++y) {
    //     colorPoint(centerX, y, 255, 0, 0, axisWidth); // Black color
    // }
    draw_axes();
}

void MainWindow::on_gridlines_clicked() {
    QPixmap canvas=ui->workArea->pixmap();
    QPainter painter(&canvas);
    // int penwidth = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
    QPen pen=QPen(QColor(200,200,200),1);
    painter.setPen(pen);
    int gridOffset = ui->gridOffset->value();
    gridOff = gridOffset;
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    if (gridOffset <= 0) return; // Prevent invalid grid offset

    int centerX=width/2;
    int centerY=height/2;

    for(int i=0;(centerX+i<=width && centerX-i>=0) || (centerY+i<=height && centerY-i>=0);i+=gridOffset){
        // for (int y = 0; y < height; ++y) {
        //     colorPoint(centerX+i,y,200,200,200);
        //     colorPoint(centerX-i,y,200,200,200);
        // }
        painter.drawLine(centerX+i,0,centerX+i,height);
        painter.drawLine(centerX-i,0,centerX-i,height);
        // for (int x = 0; x < width; ++x) {
        //     colorPoint(x,centerY+i,200,200,200);
        //     colorPoint(x,centerY-i,200,200,200);
        // }
        painter.drawLine(0,centerY+i,width,centerY+i);
        painter.drawLine(0,centerY-i,width,centerY-i);
        // Delay;
        ui->workArea->setPixmap(canvas);
    }
}

void MainWindow::on_clearCanvas_clicked() {
    QPixmap canvas=ui->workArea->pixmap();
    QPainter painter(&canvas);
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    painter.eraseRect(0,0,width,height);
    ui->workArea->setPixmap(canvas);
    polygon.clear();
    pixels.clear();
    vertex.clear();
}

void MainWindow::on_simple_clicked() {
    int x1 = ui->abcissa1->value();
    int y1 = ui->ordinate1->value();
    int x2 = ui->abcissa2->value();
    int y2 = ui->ordinate2->value();
    std::vector<std::pair<int, int>> points;
    // Starting the timer
    auto start = std::chrono::high_resolution_clock::now();

    for(int x=x1 ; x<=x2 ; ++x) {
        float fy = y1 + float(y2-y1)/float(x2-x1)*(x-x1);
        int y = (int)(fy+0.5);
        points.push_back(std::make_pair(x,y));
    }
    // Stoping the timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    qDebug() << "The slow operation took" << duration << "seconds";

    ui->time->setText(QString::number(duration.count()));
    for (long long unsigned i=0; i<points.size(); i++) {
        std::pair<int, int> point = points[i];
        // qDebug()<<point.first<<point.second;
        plotPoint(point.first, point.second,255,0,0);
        delay(50);
    }
}

void MainWindow::on_bresenham_clicked() {
    int x0 = ui->abcissa1->value();
    int y0 = ui->ordinate1->value();
    int x1 = ui->abcissa2->value();
    int y1 = ui->ordinate2->value();
    std::vector<std::pair<int, int>> points;

    // Starting the timer
    // auto start = std::chrono::high_resolution_clock::now();
    QElapsedTimer timer;
    timer.start();

    // int dx = abs(x0-x1), dy = abs(y0-y1);
    // int p = 2*dy -dx;
    // int twoDy = 2*dy, twoDyDx = 2*(dy-dx);
    // int x,y,xEnd;

    // if (x0>x1){
    //     x = x1;
    //     y = y1;
    //     xEnd = x0;
    // }
    // else {
    //     x = x0;
    //     y = y0;
    //     xEnd = x1;
    // }
    // points.push_back(std::make_pair(x,y));

    // while (x<xEnd) {
    //     x++;
    //     if (p<0) p+= twoDy;
    //     else {
    //         y++;
    //         p += twoDyDx;
    //     }
    //     points.push_back(std::make_pair(x,y));
    // }
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int p = 2 * dy - dx;

    int x = x0;
    int y = y0;

    if (dx > dy) {
        while (x != x1) {
            points.push_back(std::make_pair(x, y));
            x += sx;
            if (p >= 0) {
                y += sy;
                p -= 2 * dx;
            }
            p += 2 * dy;
        }
    } else {
        while (y != y1) {
            points.push_back(std::make_pair(x, y));
            y += sy;
            if (p >= 0) {
                x += sx;
                p -= 2 * dy;
            }
            p += 2 * dx;
        }
    }
    points.push_back(std::make_pair(x1, y1));
    // Stopping the timer
    qint64 time_taken = timer.nsecsElapsed();
    qDebug() << time_taken << "ns";
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = end - start;
    // qDebug() << "The slow operation took" << duration << "seconds";
    ui->time->setText(QString::number(time_taken));
    for (long long unsigned i=0; i<points.size(); i++) {
        std::pair<int, int> point = points[i];
        // qDebug()<<point.first<<point.second;
        plotPoint(point.first, point.second,0,0,255);
        delay(50);
    }
}

void MainWindow::on_dda_clicked() {
    int x1 = ui->abcissa1->value();
    int y1 = ui->ordinate1->value();
    int x2 = ui->abcissa2->value();
    int y2 = ui->ordinate2->value();
    std::vector<std::pair<int,int>> points;
    // Starting the timer
    // auto start = std::chrono::high_resolution_clock::now();
    QElapsedTimer timer;
    timer.start();

    float dx = float(x2-x1);
    float dy = float(y2-y1);
    float y = float(y1);
    float x = float(x1);
    float Dx, Dy;
    if (dx > dy) {
        Dx = 1;
        Dy = dy/dx;
    }
    else {
        Dx = dx/dy;
        Dy = 1;
    }
    int ix, iy;
    for (int k=0; k<=((dx>dy)? dx:dy); k++){
        ix = (int)(x + 0.5);
        iy = (int)(y + 0.5);
        points.push_back(std::make_pair(ix,iy));
        x += Dx ;
        y += Dy ;
    }
    // Stopping the timer
    qint64 time_taken = timer.nsecsElapsed();
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = end - start;
    // qDebug() << "The slow operation took" << duration << "seconds";
    ui->time->setText(QString::number(time_taken));
    for (long long unsigned i=0; i<points.size(); i++) {
        std::pair<int, int> point = points[i];
        // qDebug()<<point.first<<point.second;
        plotPoint(point.first, point.second,0,255,0);
        delay(50);
    }
}

void MainWindow::on_simpleCircle_clicked() {
    int x1 = ui->center_abcissa->value();
    int y1 = ui->center_ordinate->value();
    int r = ui->radius->value();
    std::vector<std::pair<int,int>> points;
    // Starting the timer
    // auto start = std::chrono::high_resolution_clock::now();
    QElapsedTimer timer;
    timer.start();

    for (int x = 0, y = r ; x <= y ; ++x) {
        float fy = sqrt(pow(r,2) - pow((x),2));
        y = ceil(fy);
        points.push_back(std::make_pair(x1+x,y1+y));
        points.push_back(std::make_pair(x1-x,y1+y));
        points.push_back(std::make_pair(x1+x,y1-y));
        points.push_back(std::make_pair(x1-x,y1-y));
        points.push_back(std::make_pair(x1+y,y1+x));
        points.push_back(std::make_pair(x1-y,y1+x));
        points.push_back(std::make_pair(x1+y,y1-x));
        points.push_back(std::make_pair(x1-y,y1-x));
    }

    // Stopping the timer
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = end - start;
    // qDebug() << "The slow operation took" << duration << "seconds";
    qint64 time_taken = timer.nsecsElapsed();
    ui->time->setText(QString::number(time_taken));
    plotPoint(x1, y1, 255, 0, 0);
    for (long long unsigned i=0; i<points.size(); i++) {
        std::pair<int, int> point = points[i];
        // qDebug()<<point.first<<point.second;
        plotPoint(point.first, point.second,0,255,0);
        delay(50);
    }
}

void MainWindow::on_parametricCircle_clicked() {
    int x1 = ui->center_abcissa->value();
    int y1 = ui->center_ordinate->value();
    int r = ui->radius->value();
    std::vector<std::pair<int,int>> points;
    // Starting the timer
    // auto start = std::chrono::high_resolution_clock::now();
    QElapsedTimer timer;
    timer.start();

    float theta = PI/2 ;
    while (theta >= PI/4) {
        float fx = r*cos(theta);
        float fy = r*sin(theta);
        int x = ceil(fx);
        int y = ceil(fy);
        theta = theta - 1.0/r;
        // points.insert(std::make_pair(x1+x,y1+y));
        // points.insert(std::make_pair(x1-x,y1+y));
        // points.insert(std::make_pair(x1+x,y1-y));
        // points.insert(std::make_pair(x1-x,y1-y));
        // points.insert(std::make_pair(x1+y,y1+x));
        // points.insert(std::make_pair(x1-y,y1+x));
        // points.insert(std::make_pair(x1+y,y1-x));
        // points.insert(std::make_pair(x1-y,y1-x));
        points.push_back(std::make_pair(x1+x,y1+y));
        points.push_back(std::make_pair(x1-x,y1+y));
        points.push_back(std::make_pair(x1+x,y1-y));
        points.push_back(std::make_pair(x1-x,y1-y));
        points.push_back(std::make_pair(x1+y,y1+x));
        points.push_back(std::make_pair(x1-y,y1+x));
        points.push_back(std::make_pair(x1+y,y1-x));
        points.push_back(std::make_pair(x1-y,y1-x));
    }

    // Stopping the timer
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = end - start;
    // qDebug() << "The slow operation took" << duration << "seconds";
    qint64 time_taken = timer.nsecsElapsed();
    ui->time->setText(QString::number(time_taken));

    plotPoint(x1, y1, 0, 255, 0);
    for (const std::pair<int, int>& point : points) {
        // qDebug() << point.first << point.second;
        plotPoint(point.first, point.second, 0, 0, 255);
        delay(50);
    }
}

void MainWindow::on_bresenhamCircle_clicked() {
    int x1 = ui->center_abcissa->value();
    int y1 = ui->center_ordinate->value();
    int r = ui->radius->value();
    std::vector<std::pair<int,int>> points;

    // auto start = std::chrono::high_resolution_clock::now();
    int x = 0;
    int y = r;
    int p = 1-r;
    points.push_back(std::make_pair(x1+x,y1+y));
    points.push_back(std::make_pair(x1-x,y1+y));
    points.push_back(std::make_pair(x1+x,y1-y));
    points.push_back(std::make_pair(x1-x,y1-y));
    points.push_back(std::make_pair(x1+y,y1+x));
    points.push_back(std::make_pair(x1-y,y1+x));
    points.push_back(std::make_pair(x1+y,y1-x));
    points.push_back(std::make_pair(x1-y,y1-x));

    // Starting the timer
    QElapsedTimer timer;
    timer.start();

    while (x<y) {
        x++;
        if (p<0) {
            p += 2*x+1;
        }
        else {
            y--;
            p += 2*(x-y)+1;
        }
        // points.push_back(std::make_pair(x1+x,y1+y));
        // points.push_back(std::make_pair(x1-x,y1+y));
        // points.push_back(std::make_pair(x1+x,y1-y));
        // points.push_back(std::make_pair(x1-x,y1-y));
        // points.push_back(std::make_pair(x1+y,y1+x));
        // points.push_back(std::make_pair(x1-y,y1+x));
        // points.push_back(std::make_pair(x1+y,y1-x));
        // points.push_back(std::make_pair(x1-y,y1-x));
        points.push_back({x1+x,y1+y});
        points.push_back({x1-x,y1+y});
        points.push_back({x1+x,y1-y});
        points.push_back({x1-x,y1-y});
        points.push_back({x1+y,y1+x});
        points.push_back({x1-y,y1+x});
        points.push_back({x1+y,y1-x});
        points.push_back({x1-y,y1-x});
    }

    // Stopping the timer
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = end - start;
    // qDebug() << "The slow operation took" << duration << "seconds";
    // ui->time->setText(QString::number(duration.count()));
    qint64 time_taken = timer.nsecsElapsed();
    ui->time->setText(QString::number(time_taken));
    plotPoint(x1, y1, 0, 0, 255);
    for (const std::pair<int, int>& point : points) {
        // qDebug() << point.first << point.second;
        plotPoint(point.first, point.second, 255, 0, 0);
        delay(50);
    }
}

void MainWindow::on_simpleEllipse_clicked() {
    // int x1 = ui->ellipse_center_abcissa->value();
    // int y1 = ui->ellipse_center_ordinate->value();
    // int rx = ui->x_radius->value();
    // int ry = ui->y_radius->value();
    // Need not be implemedted for assignment
}

void MainWindow::on_polarEllipse_clicked() {
    // int x1 = ui->ellipse_center_abcissa->value();
    // int y1 = ui->ellipse_center_ordinate->value();
    int rx = ui->x_radius->value();
    int ry = ui->y_radius->value();

    QPoint center = getCenter();
    int x1 = center.x();
    int y1 = center.y();

    double step = PI / 100;
    std::vector<std::pair<int, int>> points;
    // Starting the timer
    QElapsedTimer timer;
    timer.start();
    float theta = 0;
    while(theta <= PI / 2) {
        // Parametric equations for ellipse
        int x = ceil(rx * cos(theta));
        int y = ceil(ry * sin(theta));
        theta += step;
        // Use symmetry to generate points for all four quadrants
        points.push_back({x1+x, y1+y});     // First quadrant
        points.push_back({x1-x, y1+y});    // Second quadrant
        points.push_back({x1-x, y1-y});   // Third quadrant
        points.push_back({x1+x, y1-y});    // Fourth quadrant
    }
    points.push_back({x1, y1+ry});
    points.push_back({x1, y1-ry});
    // Stopping the timer
    qint64 time_taken = timer.nsecsElapsed();
    ui->time->setText(QString::number(time_taken));
    plotPoint(x1, y1, 0, 0, 255);
    for (const std::pair<int, int>& point : points) {
        // qDebug() << point.first << point.second;
        plotPoint(point.first, point.second, 0, 0, 255);
        delay(25);
    }
}

void MainWindow::on_bresenhamEllipse_clicked() {
    // int x1 = ui->ellipse_center_abcissa->value();
    // int y1 = ui->ellipse_center_ordinate->value();
    int rx = ui->x_radius->value();
    int ry = ui->y_radius->value();

    QPoint center = getCenter();
    int x1 = center.x();
    int y1 = center.y();

    // Starting the timer
    QElapsedTimer timer;
    timer.start();

    int rx2 = rx * rx ;
    int ry2 = ry * ry ;
    int tworx2 = 2 * rx2 ;
    int twory2 = 2 * ry2 ;
    int p ;
    int x = 0 ;
    int y = ry ;
    int px = 0 ;
    int py = tworx2 * y ;
    std::vector<std::pair<int, int>> points;
    points.push_back({x1+x, y1+y});
    points.push_back({x1-x, y1+y});
    points.push_back({x1+x, y1-y});
    points.push_back({x1-x, y1-y});

    // Region 1
    p = ceil(ry2 - (rx2 * ry) + (0.25 * rx2));
    while (px < py){
        x++ ;
        px += twory2 ;
        if (p<0)
            p += ry2 + px ;
        else {
            y-- ;
            py -= tworx2 ;
            p += ry2 + px - py ;
        }
        points.push_back({x1+x, y1+y});
        points.push_back({x1-x, y1+y});
        points.push_back({x1+x, y1-y});
        points.push_back({x1-x, y1-y});
    }

    // Region 2
    p = ceil(ry2 * (x+0.5)*(x+0.5) + rx2 * (y-1) * (y-1) - rx2 * ry2);
    while (y>0){
        y--;
        py -= tworx2 ;
        if (p > 0)
            p += rx2 - py ;
        else{
            x++;
            px += twory2;
            p += rx2 -py + px ;
        }
        points.push_back({x1+x, y1+y});
        points.push_back({x1-x, y1+y});
        points.push_back({x1+x, y1-y});
        points.push_back({x1-x, y1-y});
    }
    // Stopping the timer
    qint64 time_taken = timer.nsecsElapsed();
    ui->time->setText(QString::number(time_taken));
    plotPoint(x1, y1, 0, 255, 0);
    for (const std::pair<int, int>& point : points) {
        // qDebug() << point.first << point.second;
        plotPoint(point.first, point.second, 0, 255, 0);
        delay(50);
    }
}

bool MainWindow::getPixel(int x, int y){
    QPoint searchPoint(x,y);
    if (polygon.contains(searchPoint))
        return true;
    else
        return false;
}

void MainWindow::boundaryFill(int x, int y, QList<std::pair<int,int>>& visited_points, QList<std::pair<int,int>>& plotPoints) {
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2;
    int centerY=height/2;
    int gridOffset = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
    int max_width = abs(floor((width-centerX)*1.0/gridOffset));
    int max_height = abs(floor((centerY-height)*1.0/gridOffset));
    bool filled = getPixel(x,y);
    while (!filled && !visited_points.contains({x,y}) && ((abs(x) <= max_width) && (abs(y) <= max_height))){
        plotPoints << (std::make_pair(x,y));
        visited_points << (std::make_pair(x,y));
        boundaryFill(x+1,y, visited_points, plotPoints);
        boundaryFill(x,y+1, visited_points, plotPoints);
        boundaryFill(x-1,y, visited_points, plotPoints);
        boundaryFill(x,y-1, visited_points, plotPoints);
        if(ui->use8connected->isChecked()){
            boundaryFill(x+1,y+1, visited_points, plotPoints);
            boundaryFill(x-1,y-1, visited_points, plotPoints);
            boundaryFill(x+1,y-1, visited_points, plotPoints);
            boundaryFill(x-1,y+1, visited_points, plotPoints);
        }
    }
}

void MainWindow::floodFill(int x, int y, QColor org, QList<std::pair<int,int>>& visited_points, QList<std::pair<int,int>>& plotPoints) {
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2;
    int centerY=height/2;
    int gridOffset = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
    int max_width = abs(floor((width-centerX)*1.0/gridOffset));
    int max_height = abs(floor((centerY-height)*1.0/gridOffset));
    bool filled = getPixel(x,y);
    // if (getColor(x,y) != QColor(1, 0.784314, 0.784314, 0.784314)){
    //     qDebug() << getColor(x,y);
    //     return;
    // }
    // if (getColor(x,y) != org)
    //     return;
    QColor color = getColor(x,y);
    qDebug() << getColor(x,y);
    while (!filled && !visited_points.contains({x,y}) && ((abs(x) <= max_width) && (abs(y) <= max_height))){
        plotPoints << (std::make_pair(x,y));
        visited_points << (std::make_pair(x,y));
        // floodFill(x+1,y, visited_points, plotPoints);
        // floodFill(x,y+1, visited_points, plotPoints);
        // floodFill(x-1,y, visited_points, plotPoints);
        // floodFill(x,y-1, visited_points, plotPoints);
        // if(ui->use8connected->isChecked()){
        //     floodFill(x+1,y+1, visited_points, plotPoints);
        //     floodFill(x-1,y-1, visited_points, plotPoints);
        //     floodFill(x+1,y-1, visited_points, plotPoints);
        //     floodFill(x-1,y+1, visited_points, plotPoints);
        // }
        floodFill(x+1,y, org, visited_points, plotPoints);
        floodFill(x,y+1, org, visited_points, plotPoints);
        floodFill(x-1,y, org, visited_points, plotPoints);
        floodFill(x,y-1, org, visited_points, plotPoints);
        if(ui->use8connected->isChecked()){
            floodFill(x+1,y+1, org, visited_points, plotPoints);
            floodFill(x-1,y-1, org, visited_points, plotPoints);
            floodFill(x+1,y-1, org, visited_points, plotPoints);
            floodFill(x-1,y+1, org, visited_points, plotPoints);
        }
    }
}

void MainWindow::on_boundaryFill_clicked() {
    // Implementing Boundary Fill using 4 connected
    QPoint seed = pixels.top();
    QList<std::pair<int,int>> visited;
    QList<std::pair<int,int>> plotPoints;
    boundaryFill(seed.x(), seed.y(), visited, plotPoints);
    for (auto point : plotPoints){
        plotPoint(point.first, point.second,0,0,255);
        delay(50);
    }
}

void MainWindow::on_floodFill_clicked() {
    QPoint seed = pixels.top();
    QList<std::pair<int,int>> visited;
    QList<std::pair<int,int>> plotPoints;
    floodFill(seed.x(), seed.y(), getColor(seed.x(), seed.y()), visited, plotPoints);
    for (auto point : plotPoints){
        plotPoint(point.first, point.second,0,255,0);
        delay(50);
    }
}

void MainWindow::scanlineFill(QList<std::pair<int,int>>& plotPoints, int min_x, int max_x, int min_y, int max_y) {
    int c = 0;
    // Iterate over each scan line in the image
    for (int i = min_y+1; i <= max_y-1; i++){
        c = 0;
        for (int j = min_x; j<= max_x; j++){
            if (polygon.contains({j,i})){
                // int poly_idx = polygon.indexOf({j,i});
                // if (polygon[(poly_idx+1)%polygon.size()].y() == i)
                //     continue;
                // c++;
                if (vertex.contains({j,i})){
                    int idx = vertex.indexOf({j,i});
                    if ((vertex[(idx-1+vertex.size())%vertex.size()].y()-i)*(vertex[(idx+1)%vertex.size()].y()-i) > 0)
                        c++;
                }
                int poly_idx = polygon.indexOf({j,i});
                if (polygon[(poly_idx+1)%polygon.size()].y() == i)
                    continue;
                c++;
            }
            else {
                if (c % 2 != 0) {
                    plotPoints << std::make_pair(j,i);
                }
            }
        }
    }
}

void MainWindow::on_scanFill_clicked() {
    QList<std::pair<int,int>> plotPoints;
    std::vector<int> abscissa;
    std::vector<int> ordinate;
    for (int i=0; i<polygon.size(); i++){
        QPoint point = polygon[i];
        abscissa.push_back(point.x());
        ordinate.push_back(point.y());
    }
    std::sort(abscissa.begin(), abscissa.end());
    std::sort(ordinate.begin(), ordinate.end());
    int min_x = abscissa[0];
    int max_x = abscissa[abscissa.size() - 1];
    int min_y = ordinate[0];
    int max_y = ordinate[ordinate.size() - 1];
    scanlineFill(plotPoints, min_x, max_x, min_y, max_y);
    for (auto point : plotPoints){
        plotPoint(point.first, point.second,0,255,0);
        delay(50);
    }
}

void matrixMult(double a[3][3], int b[3], double res[3]){
    int i,j;
    for(i=0;i<3;i++){
        res[i]=0;
        for(j=0;j<3;j++){
            res[i]+=(a[i][j]*b[j]);
        }
    }
}

void MainWindow::on_translate_clicked(){
    poly_draw(vertex, 255, 255, 255);
    on_gridlines_clicked();
    on_showAxis_clicked();
    int tx = ui->x_translate->value();
    int ty = ui->y_translate->value();

    // matrix for translation
    double mat[3][3]={{1,0,tx*1.0},{0,1,ty*1.0},{0,0,1}};

    int coord[3];
    double res[3];
    for(long long int i=0;i<vertex.size();i++){
        coord[0]=vertex[i].x();
        coord[1]=vertex[i].y();
        coord[2]=1;
        matrixMult(mat,coord,res);
        vertex[i] = QPoint((res[0]/res[2]),(res[1]/res[2]));
    }
    poly_draw(vertex);
}

void MainWindow::on_scale_clicked(){
    poly_draw(vertex, 255, 255, 255);
    on_gridlines_clicked();
    on_showAxis_clicked();
    double sx = (ui->x_scale->value());
    double sy = (ui->y_scale->value());

    int piv_x = 0;
    int piv_y = 0;

    if (!(ui->useOrigin->isChecked())) {
        piv_x = pixels.top().x();
        piv_y = pixels.top().y();
    }
    // matrix for scaling
    double mat[3][3]={{sx,0,0},{0,sy,0},{0,0,1}};
    int coord[3];
    double res[3];
    for(long long i=0;i<vertex.size();i++){
        coord[0]=vertex[i].x()-piv_x;
        coord[1]=vertex[i].y()-piv_y;
        coord[2]=1;
        matrixMult(mat,coord,res);
        vertex[i] = QPoint((res[0]/res[2])+piv_x,piv_y+(res[1]/res[2]));
    }
    poly_draw(vertex);
}

void MainWindow::on_shear_clicked(){
    poly_draw(vertex, 255, 255, 255);
    on_gridlines_clicked();
    on_showAxis_clicked();
    int shx = ui->x_shear->value();
    int shy = ui->y_shear->value();

    int piv_x = 0;
    int piv_y = 0;

    if (!(ui->useOrigin->isChecked())) {
        piv_x = pixels.top().x();
        piv_y = pixels.top().y();
    }

    double mat[3][3]={{1,shx*1.0,0},{shy*1.0,1,0},{0,0,1}};
    int coord[3];
    double res[3];
    for(long long i=0;i<vertex.size();i++){
        coord[0]=vertex[i].x()-piv_x;
        coord[1]=vertex[i].y()-piv_y;
        coord[2]=1;
        matrixMult(mat,coord,res);
        vertex[i] =QPoint((res[0]/res[2])+piv_x,piv_y+(res[1]/res[2]));
    }
    poly_draw(vertex);
}

void MainWindow::on_reflect_clicked(){
    int rx, ry;
    if (ui->useXaxis->isChecked()) {
        rx = 1, ry = -1;
    }
    else if (ui->useYaxis->isChecked()) {
        rx = -1, ry = 1;
    }
    else if (ui->reflectLine->isChecked()) {
        // Getting the arbitrary line, as end points from the stack
        QPoint p1 = pixels.top();
        pixels.pop();
        QPoint p2 = pixels.top();
        pixels.pop();
        drawBresenhamLine(p1, p2);
        double dx=p1.x()-p2.x();
        double dy=p1.y()-p2.y();

        double a=-dy;
        double b=dx;
        double c=p1.x()*dy-p1.y()*dx;

        for(int i=0;i<vertex.size();i++) {
            int x1=vertex[i].x();
            int y1=vertex[i].y();

            int first=(int)((double)x1-(double)(2*a*(a*x1+b*y1+c))/(double)((a*a+b*b)));
            int second=(int)((double)y1-(double)(2*b*(a*x1+b*y1+c))/(double)((a*a+b*b)));
            vertex[i] = QPoint(first, second);
        }
        poly_draw(vertex);
        return;
    }
    else {
        return ;
    }

    // matrix for reflection
    double mat[3][3]={{rx*1.0,0,0},{0,ry*1.0,0},{0,0,1}};

    int piv_x = pixels.top().x();
    int piv_y = pixels.top().y();

    int coord[3];
    double res[3];
    for(long long i=0;i<vertex.size();i++) {
       coord[0]=vertex[i].x()-piv_x;
       coord[1]=vertex[i].y()-piv_y;
       coord[2]=1;
       matrixMult(mat,coord,res);
       vertex[i] = QPoint(res[0]/res[2]+piv_x,piv_y+res[1]/res[2]);
    }
    poly_draw(vertex);
}

void MainWindow::on_rotate_clicked(){
    double angle = ui->angle->value();
    double dang=(double)angle*M_PI/180.0;
    double sinang=sin(dang);
    double cosang=cos(dang);

    int piv_x = 0;
    int piv_y = 0;

    if (!(ui->useOrigin->isChecked())) {
        piv_x = pixels.top().x();
        piv_y = pixels.top().y();
    }

    // matrix for rotation
    double mat[3][3]={{cosang,-sinang,0},{sinang,cosang,0},{0,0,1}};
    int coord[3];
    double res[3];
    for(long long i=0;i<vertex.size();i++){
        coord[0]=vertex[i].x()-piv_x;
        coord[1]=vertex[i].y()-piv_y;
        coord[2]=1;
        matrixMult(mat,coord,res);
        vertex[i]=QPoint((res[0]/res[2])+piv_x,piv_y+(res[1]/res[2]));
    }
    poly_draw(vertex);
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
    if(watched == ui->workArea && event->type() == QEvent::MouseButtonPress){
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();
        int gridOffset = (ui->gridOffset->value()==0)?1:ui->gridOffset->value();
        int width = ui->workArea->width();
        int height = ui->workArea->height();
        int centerX=width/2;
        int centerY=height/2;
        // int tempX = centerX + gridOffset/2;
        // int tempY = centerY - gridOffset/2;
        int cordX = floor((x-centerX)*1.0/gridOffset);
        int cordY = floor((centerY-y)*1.0/gridOffset);
        // int actualX = tempX + cordX*gridOffset;
        // int actualY = tempY - cordY*gridOffset;
        clickedPoint.setX(x);
        clickedPoint.setY(y);
        // colorPoint(actualX,actualY,0,0,255,gridOffset);
        // int x1 = ui->abcissa1->value();
        // int y1 = ui->ordinate1->value();
        // qDebug()<<x1<<y1;
        // plotPoint(x1,y1);
        plotPoint(cordX, cordY);
        // QPixmap pixmap = ui->workArea->pixmap();
        // QImage image = pixmap.toImage();
        // QColor color = image.pixel(x+gridOffset, y+gridOffset);
        // if (color == qRgb(255,255,255))
        //     qDebug() << color << x << y << cordX << cordY << pixmap;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_gridOffset_valueChanged(int arg1)
{
    if (arg1 == 0)
        return;
    qDebug() << gridOff ;
    double level = arg1;
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2;
    int centerY=height/2;
    QPixmap pixmap = ui->workArea->pixmap();
    QImage image = pixmap.toImage();
    QColor color;
    std::vector<QPoint> activePoints;
    for (int i = gridOff/2; i <= width; i += gridOff){
        for (int j = gridOff/2; j <= height; j += gridOff){
            color = image.pixel(i,j);
            if (color == qRgb(0,0,255)){
                activePoints.push_back({floor((i-centerX)*1.0/gridOff), floor((centerY-j)*1.0/gridOff)});
            }
        }
    }
    pixmap.fill();
    ui->workArea->setPixmap(pixmap);
    on_gridlines_clicked();
    ui->axisWidth->setValue(arg1);
    on_showAxis_clicked();
    for (QPoint pt : activePoints){
        plotPoint(pt.x(), pt.y());
    }
    gridOff = arg1;
}

QColor MainWindow::getColor(int x, int y){
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX=width/2;
    int centerY=height/2;
    QPixmap pixmap = ui->workArea->pixmap();
    QImage image = pixmap.toImage();
    QColor color = image.pixel(centerX + x*gridOff, centerY - y*gridOff);
    return color;
}

