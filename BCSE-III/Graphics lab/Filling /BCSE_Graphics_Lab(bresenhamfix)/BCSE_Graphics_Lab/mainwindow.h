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


    void on_parametric_clicked();

    void on_sqrt_btn_clicked();

    void on_bresenham_circle_clicked();

    void on_e_sqrt_clicked();

    void on_e_parametric_clicked();

    void on_e_bresenham_clicked();


    void on_DrawPoly_clicked();

    void on_fillP_clicked();

    void on_scanlineP_clicked();

    void on_bFill_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap temp;
    void colorPoint(int x,int y,int r,int g, int b, int penwidth);
    void delay(int ms);
    void draw_dda_line(float x1,float y1,float x2,float y2);
    void draw_bresenham_line(int x1, int y1, int x2, int y2);
    void draw_circle_parametric(int x, int y, int r);
    void draw_circle_sqrt(int x, int y, int r);
    void draw_circle_bresenham(int x, int y, int r);
    void draw_circle(int xc, int yc, int x, int y, int, int, int);
    void draw_ellipse(int xc, int yc, int x, int y, int, int, int);
    void draw_ellipse_sqrt(int xc, int yc, int a, int b);
    void draw_ellipse_para(int xc, int yc, int a, int b);
    void draw_ellipse_bresenham(int xc, int yc, int a, int b);
    void fill(int x, int y);
    void fill8(int x, int y);
    void boundaryFill(int ,int);
    void scanlineFill(int);
    QPoint clickedPoint;
    QSet<QPoint> filled_points;
    QVector<QPoint> points;
};
#endif // MAINWINDOW_H
