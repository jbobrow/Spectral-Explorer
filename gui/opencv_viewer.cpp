#include "opencv_viewer.h"
#include <QDebug>
#include <QSizePolicy>
#include <QPainter>

OpenCVViewer::OpenCVViewer(QWidget *parent) :
    QOpenGLWidget(parent)
{
    mBackgroundColor = QColor(68,68,68);


//    mOutH = 0;
//    mOutW = 0;
//    mImgRatio = 4.0f/3.0f;

//    mPosX = 0;
//    mPosY = 0;
}

void OpenCVViewer::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
    emit sizeChanged();
}

void OpenCVViewer::paintEvent(QPaintEvent *event)
{
    QOpenGLWidget::paintEvent(event);

    QPainter painter;

    painter.begin(this);
    painter.drawImage(0,0,mRenderQtImg);
    painter.setPen(Qt::white);
    painter.drawText(10,20,tr("Camera View"));
    if (mOrigHeight > 0)
    {
        painter.setPen(Qt::yellow);
        painter.drawLine(0, mSpecRow * ((float)height() / (float)mOrigHeight),
                         width(), mSpecRow * ((float)height() / (float)mOrigHeight));
    }
    painter.end();

    //do this to fix filedialog drawing (only necessary on linux)
#ifdef Q_OS_UNIX
    QApplication::instance()->processEvents();
#endif
}


bool OpenCVViewer::showImage( cv::Mat image, int specRow )
{
    if (image.empty() || width() <= 0 || height() <= 0)
        return false;

    mSpecRow = specRow;
    mOrigWidth = image.cols;
    mOrigHeight = image.rows;
    try {
        mOrigImage = image.clone();
        cv::resize(mOrigImage,mOrigImage,cv::Size(width(),height()));
    } catch (const cv::Exception &e) {
        qCritical() << "[OpenCVViewer] OpenCV exception in showImage:" << e.what();
        return false;
    }

    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                          mOrigImage.cols, mOrigImage.rows,
                          mOrigImage.step, QImage::Format_RGB888).rgbSwapped();

    update();

    return true;
}

void OpenCVViewer::mousePressEvent ( QMouseEvent * event )
{
    QOpenGLWidget::mousePressEvent(event);
    if (mOrigWidth <= 0 || mOrigHeight <= 0 || width() <= 0 || height() <= 0)
        return;

    float x = (int) event->x();
    float y = (int) event->y();
    x = x * ((float)mOrigWidth / (float)width());
    y = y * ((float)mOrigHeight / (float)height());

    qDebug() << x << y;

    if (x >= 0 && y >= 0 && x <= mOrigWidth && y <= mOrigHeight)
        emit cvImageClicked(x, y);
}


















