#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <queue>
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
    void on_showAxis_clicked();

    void on_gridlines_clicked();


    void on_setCoord_clicked();

    void on_DDADraw_clicked();

    void on_store_clicked();

    void on_reset_clicked();

    void on_pushButton_clicked();

    void on_polar_circle_clicked();

    void on_mid_circle_clicked();

    void on_Bressenham_circle_clicked();

    void on_polar_ellipse_clicked();

    void on_bressenham_ellipse_clicked();

    void on_Flood_Fill_clicked();
    void on_DrawPolygon_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap temp;
    void colorPoint(int x,int y,int r,int g, int b, int penwidth);
    void delay(int ms);
    void draw_dda_line(float x1,float y1,float x2,float y2);
    void draw_bresenham_line(int x1, int y1, int x2, int y2);
    void floodfillutil(int x, int y, int r, int g, int b, QList<std::pair<int,int>>&);
    void floodfillutil2(int x, int y, int r, int g, int b, QList<std::pair<int,int>>&);
    QPoint clickedPoint;
    QList<QPoint> points;
    QList<QPoint> polygon;
};
#endif // MAINWINDOW_H
