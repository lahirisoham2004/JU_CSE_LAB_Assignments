#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QStack>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void on_clearCanvas_clicked();

    void on_showAxis_clicked();

    void on_gridlines_clicked();

    void on_setPoint_clicked();

    void on_simple_clicked();

    void on_dda_clicked();

    void on_bresenham_clicked();

    void on_simpleCircle_clicked();

    void on_parametricCircle_clicked();

    void on_bresenhamCircle_clicked();

    void on_simpleEllipse_clicked();

    void on_polarEllipse_clicked();

    void on_bresenhamEllipse_clicked();

    void on_drawPolygon_clicked();

    void on_boundaryFill_clicked();

    void on_floodFill_clicked();

    void on_scanFill_clicked();

    void on_translate_clicked();

    void on_scale_clicked();

    void on_shear_clicked();

    void on_reflect_clicked();

    void on_rotate_clicked();

    void on_gridOffset_valueChanged(int arg1);

    void on_drawWindow_clicked();

    void on_lineClip_clicked();

    void on_polygonClip_clicked();

    void on_drawLine_clicked();

private:
    Ui::MainWindow *ui;
    int gridOff;
    void draw_axes();
    void colorPoint(int x,int y,int r,int g, int b, int penwidth);
    void plotPoint(int x, int y, int r, int g, int b);
    void delay(int ms);
    QPoint getCenter();
    void drawPolygon(int nvertices);
    void poly_draw(QList<QPoint>, int r, int g, int b);
    void drawBresenhamLine(QPoint p1, QPoint p2, int r, int g, int b);
    void drawLine(QPoint p1, QPoint p2, int r, int g, int b);
    void draw_Window();
    QPoint clickedPoint;
    QStack<QPoint> pixels;
    QList<QPoint> polygon;
    QList<QPoint> vertex;
    bool getPixel(int x, int y);
    void boundaryFill(int x, int y, QList<std::pair<int,int>>& visited, QList<std::pair<int,int>>& plotPoints);
    void floodFill(int x, int y, QColor, QList<std::pair<int,int>>& visited, QList<std::pair<int,int>>& plotPoints);
    void scanlineFill(QList<std::pair<int,int>>& plotPoints, int, int, int, int);
    QColor getColor(int, int);
    QPoint p1, p2, cp1, cp2;
    QList<std::pair<QPoint,QPoint>> lines;
    void cohenSutherlandClip(int x1, int y1,int x2, int y2);
    int computeCode(int xa, int ya);
    int clipperPoints[4][2];
    int x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4);
    int y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4);
    void clip(int x1, int y1, int x2, int y2);
    void suthHodgClip();
    void poly_draw_list(std::vector<std::pair<int,int>> polygon, int r, int g, int b);
};
#endif // MAINWINDOW_H
