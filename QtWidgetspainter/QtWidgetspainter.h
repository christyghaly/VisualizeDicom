#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetspainter.h"
#include "GL/glut.h"
#include "qopenglfunctions.h"
#include "gl/glu.h"
#include "qpoint.h"
#include "qpainter.h"
#include "qtimer.h"




class QtWidgetspainter : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetspainter(QWidget *parent = Q_NULLPTR);
  

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

public slots:
    void Zoom_Out();
    void Zoom_In();
    void incBright();
    void incdarkness();
    void incContrast();
    void choosefile();

private:
    Ui::QtWidgetspainterClass ui;
    QPoint lastpos;
    QPoint trans;
    QOpenGLFunctions* f;
    QPainter painter;
    QTimer m_timer;

    QSize m_scale;
    QImage img;
    QString filepath="";
    int highBrightnessVal=0;
    int lowBrightnessVal = 0;
    int contrastFactor=1;
};
