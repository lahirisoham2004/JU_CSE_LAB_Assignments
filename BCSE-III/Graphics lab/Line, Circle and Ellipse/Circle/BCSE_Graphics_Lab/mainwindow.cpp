#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QList>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QDebug>

#define Delay delay(0)
QList<QPoint> clickedPoints;

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

void MainWindow::colorPoint(int x, int y, int r, int g, int b, int penwidth = 1) {
    QPixmap canvas = ui->workArea->pixmap();
    QPainter painter(&canvas);
    QPen pen = QPen(QColor(r, g, b), penwidth);
    painter.setPen(pen);

    // Calculate the top-left corner of the square based on grid size
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int startX = x - (x % gridOffset);
    int startY = y - (y % gridOffset);

    // Draw the square
    painter.fillRect(startX, startY, gridOffset, gridOffset, QColor(r, g, b));
    ui->workArea->setPixmap(canvas);
}

void MainWindow::on_showAxis_clicked() {
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;
    int axisWidth = ui->axisWidth->value();

    // Draw horizontal axis
    for (int x = 0; x < width; ++x) {
        colorPoint(x, centerY, 255, 0, 0, axisWidth); // Red color
    }

    // Draw vertical axis
    for (int y = 0; y < height; ++y) {
        colorPoint(centerX, y, 255, 0, 0, axisWidth); // Red color
    }
}

void MainWindow::on_gridlines_clicked() {
    int gridOffset = ui->gridOffset->value();
    if (gridOffset <= 0) return; // Prevent invalid grid offset

    int width = ui->workArea->width();
    int height = ui->workArea->height();
    QPixmap canvas = ui->workArea->pixmap();
    QPainter painter(&canvas);
    QPen pen(QColor(200, 200, 200), 1); // Light gray color for gridlines
    painter.setPen(pen);

    // Draw vertical gridlines
    for (int x = 0; x < width; x += gridOffset) {
        painter.drawLine(x, 0, x, height);
    }

    // Draw horizontal gridlines
    for (int y = 0; y < height; y += gridOffset) {
        painter.drawLine(0, y, width, y);
    }

    ui->workArea->setPixmap(canvas);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->workArea && event->type() == QEvent::MouseMove) {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();
        int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
        int width = ui->workArea->width();
        int height = ui->workArea->height();
        int centerX = width / 2;
        int centerY = height / 2;
        ui->x_coordinate->setText(QString::number(floor((x - centerX) * 1.0 / gridOffset)));
        ui->y_coordinate->setText(QString::number(floor((centerY - y) * 1.0 / gridOffset)));
        return true; // Event handled
    }
    if (watched == ui->workArea && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *cursor = static_cast<QMouseEvent*>(event);
        int x = cursor->pos().x();
        int y = cursor->pos().y();

        // Store the clicked point
        clickedPoints.append(QPoint(x, y));

        // Keep only the last two points
        if (clickedPoints.size() > 2) {
            clickedPoints.removeFirst();
        }

        // Highlight the clicked point
        int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
        colorPoint(x, y, 0, 0, 255, gridOffset);

        return true; // Event handled
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_axisWidth_valueChanged(int arg1) {
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Draw horizontal axis
    for (int x = 0; x < width; ++x) {
        colorPoint(x, centerY, 255, 0, 0, arg1); // Red color
    }

    // Draw vertical axis
    for (int y = 0; y < height; ++y) {
        colorPoint(centerX, y, 255, 0, 0, arg1); // Red color
    }
}

void MainWindow::on_pushButton_clicked() // DDA Line Drawing Algorithm
{
    if (clickedPoints.size() < 2) {
        // Not enough points to draw a line
        return;
    }

    // Start the timer
    QElapsedTimer timer;
    timer.start();

    // Get the last two clicked points
    QPoint p1 = clickedPoints.at(0); // Second last point
    QPoint p2 = clickedPoints.at(1); // Last point

    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust coordinates to consider the grid center
    x1 = (x1 - centerX) / gridOffset;
    y1 = (centerY - y1) / gridOffset;
    x2 = (x2 - centerX) / gridOffset;
    y2 = (centerY - y2) / gridOffset;

    // Calculate dx and dy
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Determine the number of steps
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // Calculate the increment in x and y for each step
    float xIncrement = dx / static_cast<float>(steps);
    float yIncrement = dy / static_cast<float>(steps);

    // Start drawing the line
    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        // Map back to the original canvas coordinates
        int canvasX = centerX + static_cast<int>(x * gridOffset);
        int canvasY = centerY - static_cast<int>(y * gridOffset);

        // Color the point
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        // Increment x and y
        x += xIncrement;
        y += yIncrement;
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "DDA Line Drawing Time:" << timeElapsed << "ms";
}
/*
void MainWindow::on_pushButton_2_clicked() // Bresenham Line Drawing Algorithm
{
    if (clickedPoints.size() < 2) {
        // Not enough points to draw a line
        return;
    }

    // Start the timer
    QElapsedTimer timer;
    timer.start();

    // Get the last two clicked points
    QPoint p1 = clickedPoints.at(0); // Second last point
    QPoint p2 = clickedPoints.at(1); // Last point

    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust coordinates to consider the grid center
    x1 = (x1 - centerX) / gridOffset;
    y1 = (centerY - y1) / gridOffset;
    x2 = (x2 - centerX) / gridOffset;
    y2 = (centerY - y2) / gridOffset;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Map back to the original canvas coordinates
        int canvasX = centerX + x1 * gridOffset;
        int canvasY = centerY - y1 * gridOffset;

        // Color the point
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Bresenham Line Drawing Time:" << timeElapsed << "ms";
}
*/
void MainWindow::on_pushButton_2_clicked() // Bresenham Line Drawing Algorithm
{
    if (clickedPoints.size() < 2) {
        // Not enough points to draw a line
        return;
    }

    // Start the timer
    QElapsedTimer timer;
    timer.start();

    // Get the last two clicked points
    QPoint p1 = clickedPoints.at(0); // Second last point
    QPoint p2 = clickedPoints.at(1); // Last point

    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust coordinates to consider the grid center
    x1 = (x1 - centerX) / gridOffset;
    y1 = (centerY - y1) / gridOffset;
    x2 = (x2 - centerX) / gridOffset;
    y2 = (centerY - y2) / gridOffset;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Map back to the original canvas coordinates
        int canvasX = centerX + x1 * gridOffset;
        int canvasY = centerY - y1 * gridOffset;

        // Color the point (set to red: RGB (255, 0, 0))
        colorPoint(canvasX, canvasY, 255, 0, 0, gridOffset);

        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Bresenham Line Drawing Time:" << timeElapsed << "ms";
}

void MainWindow::on_pushButton_5_clicked() // Bresenham Circle Drawing Algorithm
{
    if (clickedPoints.size() < 1) {
        // No point has been clicked to set the center
        return;
    }

    // Start the timer
    QElapsedTimer timer;
    timer.start();

    // Get the center point from the last clicked point
    QPoint center = clickedPoints.last();

    // Set the radius for the circle (preset value)
    int radius = 10; // For example, set the radius to 50 pixels

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust the center coordinates to consider the grid center
    int xCenter = (center.x() - centerX) / gridOffset;
    int yCenter = (centerY - center.y()) / gridOffset;

    // Implementing Bresenham Circle Drawing Algorithm
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    auto drawSymmetricPoints = [&](int x, int y) {
        int canvasX, canvasY;

        // Drawing all symmetric points of the circle
        canvasX = centerX + (xCenter + x) * gridOffset;
        canvasY = centerY - (yCenter + y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter - x) * gridOffset;
        canvasY = centerY - (yCenter + y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter + x) * gridOffset;
        canvasY = centerY - (yCenter - y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter - x) * gridOffset;
        canvasY = centerY - (yCenter - y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter + y) * gridOffset;
        canvasY = centerY - (yCenter + x) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter - y) * gridOffset;
        canvasY = centerY - (yCenter + x) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter + y) * gridOffset;
        canvasY = centerY - (yCenter - x) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter - y) * gridOffset;
        canvasY = centerY - (yCenter - x) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);
    };

    while (y >= x) {
        drawSymmetricPoints(x, y);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Bresenham Circle Drawing Time:" << timeElapsed << "ms";
}



void MainWindow::on_pushButton_3_clicked()// Cartesian Circle
{
    if (clickedPoints.size() < 1) {
        // No point has been clicked to set the center
        return;
    }

    // Start the timer
    QElapsedTimer timer;
    timer.start();

    // Get the center point from the last clicked point
    QPoint center = clickedPoints.last();

    // Set the radius for the circle (preset value)
    int radius = 10; // For example, set the radius to 50 pixels

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust the center coordinates to consider the grid center
    int xCenter = (center.x() - centerX) / gridOffset;
    int yCenter = (centerY - center.y()) / gridOffset;

    int x = 0;
    int y;

    while (x <= radius) {
        y = sqrt(radius * radius - x * x); // Calculate y using the circle equation

        // Drawing all symmetric points of the circle
        auto drawSymmetricPoints = [&](int x, int y) {
            int canvasX, canvasY;

            canvasX = centerX + (xCenter + x) * gridOffset;
            canvasY = centerY - (yCenter + y) * gridOffset;
            colorPoint(canvasX, canvasY, 0, 255, 0, gridOffset); // Green color

            canvasX = centerX + (xCenter - x) * gridOffset;
            canvasY = centerY - (yCenter + y) * gridOffset;
            colorPoint(canvasX, canvasY, 0, 255, 0, gridOffset);

            canvasX = centerX + (xCenter + x) * gridOffset;
            canvasY = centerY - (yCenter - y) * gridOffset;
            colorPoint(canvasX, canvasY, 0, 255, 0, gridOffset);

            canvasX = centerX + (xCenter - x) * gridOffset;
            canvasY = centerY - (yCenter - y) * gridOffset;
            colorPoint(canvasX, canvasY, 0, 255, 0, gridOffset);
        };

        drawSymmetricPoints(x, y);

        // Move to the next x coordinate
        x++;
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Cartesian Circle Drawing Time:" << timeElapsed << "ms";
}


void MainWindow::on_pushButton_4_clicked()//Polar Circle
{
    if (clickedPoints.size() < 1) {
        // No point has been clicked to set the center
        return;
    }

    // Start the timer
    QElapsedTimer timer;
    timer.start();

    // Get the center point from the last clicked point
    QPoint center = clickedPoints.last();

    // Set the radius for the circle (preset value)
    int radius = 10; // For example, set the radius to 50 pixels

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust the center coordinates to consider the grid center
    int xCenter = (center.x() - centerX) / gridOffset;
    int yCenter = (centerY - center.y()) / gridOffset;

    // Loop over theta from 0 to 360 degrees (in radians)
    for (double theta = 0; theta < 2 * M_PI; theta += 0.01) {
        int x = radius * cos(theta);
        int y = radius * sin(theta);

        // Map back to the original canvas coordinates
        int canvasX = centerX + (xCenter + x) * gridOffset;
        int canvasY = centerY - (yCenter + y) * gridOffset;

        // Color the point (set to red: RGB (255, 0, 0))
        colorPoint(canvasX, canvasY, 255, 0, 0, gridOffset);
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Polar Circle Drawing Time:" << timeElapsed << "ms";
}

