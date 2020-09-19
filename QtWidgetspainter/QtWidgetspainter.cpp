#include "QtWidgetspainter.h"
#include "qpainter.h"
#include "qmouseeventtransition.h"
#include "qevent.h"
#include "qdebug.h"
#include "qpixmap.h"
#include "qscrollarea.h"
#include "qlistview.h"
#include "qsignalmapper.h"
#include "qmenu.h"
#include "qopenglwidget.h"
#include "qcolor.h"
#include "iostream"
#include "qalgorithms.h"
#include "qfiledialog.h"
#include "qmessagebox.h"


QtWidgetspainter::QtWidgetspainter(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_MouseTracking,true);
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::NoProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setVersion(2, 1);
    ui.openglWidget->setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
    ui.openglWidget->setFormat(format);
    trans.setX(0);
    trans.setY(0);
    m_scale.setWidth(1);
    m_scale.setHeight(1);
   ui.mainToolBar->setFixedHeight(30);
   ui.mainToolBar->setEnabled(true);
   ui.mainToolBar->addAction( QIcon("upload_photo.png"),"Choose File", [=] {choosefile(); });
   ui.mainToolBar->show();
    

}


void QtWidgetspainter::initializeGL()
{

    glClearColor(0.0, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT );
    glClearColor(0.0, 0.0, 0.0,0.0);
    resizeGL(this->width(), this->height());


}

void QtWidgetspainter::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    qreal aspectratio = qreal(w) / qreal(h);
    glOrtho(-1 * aspectratio, 1 * aspectratio, -1, 1, 1, -1);

}
void QtWidgetspainter::paintGL()
{
    painter.begin(ui.openglWidget);
    img.load(filepath);
    QImage imagscaled;
    qreal newWidth = img.size().width() * m_scale.width();
    qreal newHeight = img.size().height() * m_scale.height();
    QSize finalImgSize;
    finalImgSize.setWidth(newWidth);
    finalImgSize.setHeight(newHeight);
    imagscaled=img.scaled(finalImgSize, Qt::KeepAspectRatio, Qt::FastTransformation); //scaling the image to bigger size
    painter.drawImage(trans, imagscaled);
    painter.end();
    ui.openglWidget->updateBehavior();
   
}
void QtWidgetspainter::resizeEvent(QResizeEvent* event)
{
    resizeGL(this->width(), this->height());
    this->update();
}
void QtWidgetspainter::paintEvent(QPaintEvent* e)
{
    
    paintGL();
    ui.openglWidget->makeCurrent();


}

void QtWidgetspainter::mousePressEvent(QMouseEvent* event)
{
    if (filepath.operator==("")) {
        QMessageBox::critical(this, "Empty Screen", "Please upload a photo", QMessageBox::Ok);
    }
    else {
        if (event->button() == Qt::RightButton) {
            QMenu* myMenu = new QMenu(this);
            QAction* zoomIn = myMenu->addAction(tr("Zoom in"));
            QAction* zoomOut = myMenu->addAction(tr("Zoom out"));
            QAction* Brightness = myMenu->addAction(tr("Increase Brightness "));
            QAction* Darkness = myMenu->addAction(tr("Decrease Brightness"));
            QAction* ContrastInc = myMenu->addAction(tr("Increase Contrast"));
            if (highBrightnessVal == 100)
                Brightness->setEnabled(false);
            if (lowBrightnessVal == -100)
                Darkness->setEnabled(false);
            if (contrastFactor > 1)
                ContrastInc->setEnabled(false);

            myMenu->move(mapToGlobal(event->pos()));
            myMenu->show();
            connect(zoomIn, SIGNAL(triggered()), this, SLOT(Zoom_In()));
            connect(zoomOut, SIGNAL(triggered()), this, SLOT(Zoom_Out()));
            connect(Brightness, SIGNAL(triggered()), this, SLOT(incBright()));
            connect(Darkness, SIGNAL(triggered()), this, SLOT(incdarkness()));
            connect(ContrastInc, SIGNAL(triggered()), this, SLOT(incContrast()));
        }
        else if (event->button() == Qt::LeftButton)
        {
            lastpos = event->pos();

        }
    }
}

void QtWidgetspainter::mouseMoveEvent(QMouseEvent* event) //get mouse move event for making the image protable with the mouse
{
    trans.setX(event->x());
    trans.setY(event->y());
    ui.openglWidget->update();
    update();
  //  m_timer.start();
    
}

void QtWidgetspainter::Zoom_Out()
{
qDebug() << "ZoomOut called";
 qreal s = 0.5;
 m_scale *= s;
 ui.openglWidget->update();
 update();
}
void QtWidgetspainter::Zoom_In()
{
    qreal s = 1.5;
    m_scale *= s;
    ui.openglWidget->update();
    update();
}
//Demo for Increasing and Decreasing the Brightness better to use opencv

//Code to Increase Brightness
void QtWidgetspainter::incBright()
{
    QImage imge(filepath);
    if ( highBrightnessVal < 100) { // to set maximum brightness can be done to be +100
        highBrightnessVal += 50; 
        int imgsize = imge.height() * imge.width();
        int i = 0;
        QColor pixColor;
        qDebug() << "Width " << imge.width() << " Height " << imge.height() << "\n";
        for (int h = 0; h < imge.height(); h++) {
            for (int w = 0; w < imge.width(); w++) {
                pixColor = imge.pixelColor(w, h);
                int red = pixColor.red() + 50;
                int green = pixColor.green() + 50;
                int blue = pixColor.blue() + 50;
                int arr_color[] = { red,green,blue };
                if (red > 255)
                    red = 255;
                if (green > 255)
                    green = 255;
                if (blue >255)
                    blue = 255;
                pixColor.setRed(red);
                pixColor.setGreen(green);
                pixColor.setBlue(blue);
                imge.setPixelColor(w, h, pixColor);

            }
        }
        filepath = "ct2.jpg"; // a new image inorder not to corrupt the original image
        lowBrightnessVal += 50;
        imge.save(filepath);
        ui.openglWidget->update();
        update();
    }

}

//Code to Decrease Brightness
void QtWidgetspainter::incdarkness() {
    QImage imge(filepath);
    if (lowBrightnessVal > -100) // to set maximum darkness can be done to be -100
    {
        lowBrightnessVal -= 50;

        int imgsize = imge.height() * imge.width();
        int i = 0;
        QColor pixColor;
        qDebug() << "Width " << imge.width() << " Height " << imge.height() << "\n";
        for (int h = 0; h < imge.height(); h++) {
            for (int w = 0; w < imge.width(); w++) {


                pixColor = imge.pixelColor(w, h);
                int red = pixColor.red() - 50;
                int green = pixColor.green() - 50;
                int blue = pixColor.blue() - 50;
                int arr_color[] = { red,green,blue };
                if (red <0 )
                    red = 0;
                if (green < 0)
                    green = 0;
                if (blue < 0)
                    blue = 0;
                pixColor.setRed(arr_color[0]);
                pixColor.setGreen(arr_color[1]);
                pixColor.setBlue(arr_color[2]);
                imge.setPixelColor(w, h, pixColor);
                qDebug() << red << " " << green << " " << blue<<"\n";
            }
        }
        filepath = "ct2.jpg";
        highBrightnessVal -= 50;
        imge.save(filepath);
        ui.openglWidget->update();
        update();
    }

}
//Code To increase the contast

void QtWidgetspainter::incContrast()
{
    QImage imge(filepath);
    if (contrastFactor == 1) { //Contrast Can be done 1 timet
        int imgsize = imge.height() * imge.width();
        int i = 0;
        QColor pixColor;
        int maxlightness=0;
        int minLightness=255 ;
        qDebug() << "Width " << imge.width() << " Height " << imge.height() << "\n";
        for (int h = 0; h < imge.height(); h++) {
            for (int w = 0; w < imge.width(); w++) {


                pixColor = imge.pixelColor(w, h);
                int lightness = pixColor.lightness();// to get the lightness which is approximately equal the luminance to get contrast
                qDebug() << "Lightness" << lightness << "\n";
                if (lightness < minLightness) {
                    minLightness = lightness;
                }
                else if (lightness > maxlightness) {
                    maxlightness = lightness;
                }
            }
        }

        int Michelson_contrast = std::floor((maxlightness - minLightness) / (maxlightness + minLightness));//Contrast calculation According to Michelson
        if (Michelson_contrast==1)
        {
            Michelson_contrast = 2;
            contrastFactor = 2;

        }

        for (int h = 0; h < imge.height(); h++) {
            for (int w = 0; w < imge.width(); w++) {
                pixColor = imge.pixelColor(w, h);

                int red =(int)( pixColor.red() * Michelson_contrast);
                int green = (int)(pixColor.green() * Michelson_contrast);
                int blue = (int)(pixColor.blue() * Michelson_contrast);
                int arr_color[] = { red,green,blue };
                if (red > 255)
                    red = 255;
                if (green > 255)
                    green = 255;
                if (blue > 255)
                    blue = 255;
                if (red < 0)
                    red = 0;
                if (green < 0)
                    green = 0;
                if (blue < 0)
                    blue = 0;
                pixColor.setRed(arr_color[0]);
                pixColor.setGreen(arr_color[1]);
                pixColor.setBlue(arr_color[2]);
                imge.setPixelColor(w, h, pixColor);

            }
        }

        filepath = "ct2.jpg";
        imge.save(filepath);
        ui.openglWidget->update();
        update();
    }
}

void QtWidgetspainter::choosefile()
{
    filepath = QFileDialog::getOpenFileName(this, tr("File Chooser"), ":/", tr("Image Files(*.png *.jpg)"));
}
