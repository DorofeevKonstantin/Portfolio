#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QPen>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QVector<QVector<QPoint>> polylines;
    QVector<QPen> pens;
    QVector<QPoint> points;
    QPen *pen;

public slots:
    void undoChanges();
    void clearChanges();
    void buttonPressed();
    void buttonReleased();
    void colorButtonPressed();
};
#endif // MAINWINDOW_H
