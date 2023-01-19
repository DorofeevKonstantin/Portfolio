#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
       ui->setupUi(this);
       this->setWindowState(Qt::WindowMaximized);
       pen = new QPen(Qt::black);
       pen->setWidth(3);
       QObject::connect(this->ui->pushButton, SIGNAL(clicked()),this,SLOT(undoChanges()));
       QObject::connect(this->ui->pushButton_2, SIGNAL(clicked()),this,SLOT(clearChanges()));

       QObject::connect(this->ui->pushButton, SIGNAL(pressed()),this,SLOT(buttonPressed()));
       QObject::connect(this->ui->pushButton_2, SIGNAL(pressed()),this,SLOT(buttonPressed()));
       QObject::connect(this->ui->colorButton, SIGNAL(pressed()),this,SLOT(buttonPressed()));

       QObject::connect(this->ui->pushButton, SIGNAL(released()),this,SLOT(buttonReleased()));
       QObject::connect(this->ui->pushButton_2, SIGNAL(released()),this,SLOT(buttonReleased()));
       QObject::connect(this->ui->colorButton, SIGNAL(released()),this,SLOT(buttonReleased()));

       QObject::connect(this->ui->colorButton, SIGNAL(clicked()),this,SLOT(colorButtonPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(polylines.size()>0)
      {
        for(int i=0;i<polylines.size();++i)
          {
            if(polylines[i].size() > 0)
              {
                painter.setPen(pens[i]);
                painter.drawPolyline(&polylines[i][0],polylines[i].size());
              }
          }
      }
    if(points.size() > 0)
      {
        painter.setPen(*pen);
        painter.drawPolyline(&points[0],points.size());
      }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  points.append(event->pos());
  repaint();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  points.clear();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
  polylines.append(points);
  pens.append(*pen);
  points.clear();
  repaint();
}

void MainWindow::undoChanges()
{
  if(polylines.size()>0)
    polylines.removeLast();
  if(pens.size() > 0)
    pens.removeLast();
  repaint();
}

void MainWindow::buttonPressed()
{
  this->ui->pushButton->setCursor(Qt::ClosedHandCursor);
  this->ui->pushButton_2->setCursor(Qt::ClosedHandCursor);
  this->ui->colorButton->setCursor(Qt::ClosedHandCursor);
}

void MainWindow::buttonReleased()
{
  this->ui->pushButton->setCursor(Qt::OpenHandCursor);
  this->ui->pushButton_2->setCursor(Qt::OpenHandCursor);
  this->ui->colorButton->setCursor(Qt::OpenHandCursor);
}

void MainWindow::colorButtonPressed()
{
  pen->setColor(QColor(qrand()%255,qrand()%255,qrand()%255));
}

void MainWindow::clearChanges()
{
  pens.clear();
  polylines.clear();
  repaint();
}
