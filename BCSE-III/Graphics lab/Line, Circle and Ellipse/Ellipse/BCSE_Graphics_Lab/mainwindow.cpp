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


void MainWindow::on_pushButton_6_clicked()// Polar Ellipse
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

    // Set the semi-major and semi-minor axes for the ellipse
    int a = 20; // Semi-major axis length
    int b = 4; // Semi-minor axis length

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust the center coordinates to consider the grid center
    int xCenter = (center.x() - centerX) / gridOffset;
    int yCenter = (center.y() - centerY) / gridOffset;

    // Loop over theta from 0 to 360 degrees (in radians)
    for (double theta = 0; theta < 2 * M_PI; theta += 0.01) {
        int x = a * cos(theta);
        int y = b * sin(theta);

        // Map back to the original canvas coordinates
        int canvasX = centerX + (xCenter + x) * gridOffset;
        int canvasY = centerY - (yCenter + y) * gridOffset;

        // Color the point (set to red: RGB (255, 0, 0))
        colorPoint(canvasX, canvasY, 255, 0, 0, gridOffset);
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Polar Ellipse Drawing Time:" << timeElapsed << "ms";
}


void MainWindow::on_pushButton_7_clicked()// Bressenham Ellipse
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

    // Set the semi-major and semi-minor axes for the ellipse
    int a = 20; // Semi-major axis length
    int b = 4; // Semi-minor axis length

    // Grid offset handling
    int gridOffset = (ui->gridOffset->value() == 0) ? 1 : ui->gridOffset->value();
    int width = ui->workArea->width();
    int height = ui->workArea->height();
    int centerX = width / 2;
    int centerY = height / 2;

    // Adjust the center coordinates to consider the grid center
    int xCenter = (center.x() - centerX) / gridOffset;
    int yCenter = (center.y() - centerY) / gridOffset;

    // Bresenham's Ellipse Drawing Algorithm
    auto drawEllipsePoints = [&](int x, int y) {
        int canvasX = centerX + (xCenter + x) * gridOffset;
        int canvasY = centerY - (yCenter + y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset); // Blue color

        canvasX = centerX + (xCenter - x) * gridOffset;
        canvasY = centerY - (yCenter + y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter + x) * gridOffset;
        canvasY = centerY - (yCenter - y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);

        canvasX = centerX + (xCenter - x) * gridOffset;
        canvasY = centerY - (yCenter - y) * gridOffset;
        colorPoint(canvasX, canvasY, 0, 0, 255, gridOffset);
    };

    int x = 0;
    int y = b;
    int b2 = b * b;
    int a2 = a * a;
    int xChange = 0;
    int yChange = 2 * a2 * y;
    int ellipseError = a2 - (2 * a2 * y) + (a2 / 4);

    while (x * b2 <= y * a2) {
        drawEllipsePoints(x, y);

        if (ellipseError > 0) {
            y--;
            yChange -= 2 * a2;
            ellipseError += a2 - yChange;
        }

        x++;
        xChange += 2 * b2;
        ellipseError += xChange + b2;
    }

    x = a;
    y = 0;
    xChange = 2 * b2 * x;
    yChange = 0;
    ellipseError = b2 - (2 * b2 * x) + (b2 / 4);

    while (y * a2 <= x * b2) {
        drawEllipsePoints(x, y);

        if (ellipseError > 0) {
            x--;
            xChange -= 2 * b2;
            ellipseError += b2 - xChange;
        }

        y++;
        yChange += 2 * a2;
        ellipseError += yChange + a2;
    }

    // Print the elapsed time in milliseconds
    qint64 timeElapsed = timer.elapsed();
    qDebug() << "Bresenham Ellipse Drawing Time:" << timeElapsed << "ms";
}

void MainWindow::on_pushButton_8_clicked()
{


    // Choose the fill color (e.g., blue)
    QColor fillColor(0, 0, 255); // Blue color

    // Create a QPixmap to draw on
    QPixmap canvas = ui->workArea->pixmap();
    if (canvas.isNull()) {
        qDebug() << "Canvas is null!";
        return;
    }

    QPainter painter(&canvas);
    if (!painter.isActive()) {
        qDebug() << "Painter is not active!";
        return;
    }

    painter.setBrush(QBrush(fillColor)); // Set the fill color
    painter.setPen(Qt::NoPen); // Disable outline drawing

    // Manually calculate the bounding box
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (const QPoint &point : clickedPoints) {
        minX = std::min(minX, point.x());
        maxX = std::max(maxX, point.x());
        minY = std::min(minY, point.y());
        maxY = std::max(maxY, point.y());
    }

    qDebug() << "Bounding Box:" << "MinX:" << minX << "MaxX:" << maxX << "MinY:" << minY << "MaxY:" << maxY;

    // Iterate over each scanline (horizontal line)
    for (int y = minY; y <= maxY; ++y) {
        QList<int> intersections;

        // Check intersection of each edge of the polygon with the current scanline
        for (int i = 0; i < clickedPoints.size(); ++i) {
            QPoint p1 = clickedPoints.at(i);
            QPoint p2 = clickedPoints.at((i + 1) % clickedPoints.size());

            // Ensure p1.y <= p2.y for consistent edge handling
            if (p1.y() > p2.y()) std::swap(p1, p2);

            // Check if scanline intersects with the edge (p1, p2)
            if (y >= p1.y() && y < p2.y()) {
                // Calculate the x-coordinate of the intersection
                float slope = (float)(p2.x() - p1.x()) / (p2.y() - p1.y());
                int intersectionX = p1.x() + slope * (y - p1.y());
                intersections.append(intersectionX);
            }
        }

        // Sort intersections by x-coordinates
        std::sort(intersections.begin(), intersections.end());

        qDebug() << "Scanline y:" << y << "Intersections:" << intersections;

        // Fill pixels between pairs of intersections
        for (int i = 0; i < intersections.size(); i += 2) {
            if (i + 1 < intersections.size()) {
                int startX = intersections[i];
                int endX = intersections[i + 1];

                // Draw the horizontal line between intersections
                painter.setPen(Qt::NoPen); // Ensure no outline drawing
                painter.setBrush(QBrush(fillColor)); // Ensure fill color is set
                painter.drawRect(QRect(QPoint(startX, y), QPoint(endX, y))); // Fill between intersections
            }
        }
    }

    // Update the canvas
    ui->workArea->setPixmap(canvas);
}
