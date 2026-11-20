#include "graph_viewer.h"
#include <QPainter>
#include <QDebug>
#include <QApplication>

GraphViewer::GraphViewer(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setAutoFillBackground(false);
    setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
    setMouseTracking(true);
}

void GraphViewer::showSpectrumGraph(SpectrumDrawingStrategy* dr, bool deleteStrategy)
{
    if (deleteStrategy) delete mSpectrumDrawing;
    mSpectrumDrawing = dr;
    update();
}

void GraphViewer::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
    emit sizeChanged();
}

void GraphViewer::paintEvent(QPaintEvent *event)
{
    QOpenGLWidget::paintEvent(event);

    QPainter painter;

    painter.begin(this);
    if (mSpectrumDrawing != nullptr)
    {
        mSpectrumDrawing->draw(&painter,event->rect());
        if (underMouse())
            mSpectrumDrawing->drawMouseAxis(&painter, event->rect(), mMouseX, mMouseY);
        mSpectrumDrawing->drawConfigLines(&painter,event->rect());
        mSpectrumDrawing->drawTitle(&painter, event->rect());
    }
    painter.end();

    //do this to fix filedialog drawing (only necessary on linux)
#ifdef Q_OS_UNIX
    QApplication::instance()->processEvents();
#endif
}

void GraphViewer::mouseMoveEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseMoveEvent(event);
    mMouseX = event->x();
    mMouseY = event->y();
    update();
}

void GraphViewer::mousePressEvent ( QMouseEvent * event )
{
    QOpenGLWidget::mousePressEvent(event);
    if (mSpectrumDrawing == nullptr) return;

    qDebug() << "[GraphViewer] Mouse pressed: " << event;

    SpectrumData data = mSpectrumDrawing->getData();
    if (data.mSpecWidth <= 0 || this->width() <= 0 || this->height() <= 0)
        return;

    float scale = static_cast<float>(this->width()) / static_cast<float>(data.mSpecWidth);
    int x = static_cast<int>(round( event->x()/scale ));
    emit newCFLX(x);

    // Calculate data for target selection
    float wavelength = mSpectrumDrawing->xToWavelength(event->x(),width());
    float intensity = mSpectrumDrawing->yToIntensity(event->y(),height());
    MeasurementTarget target(wavelength,intensity);
    int mode = -1;
    if (event->button() == Qt::LeftButton) mode = 0;
    if (event->button() == Qt::RightButton) mode = 1;
    if (mode != -1) {
        qDebug() << "[GraphViewer] Emit target with" << wavelength << "nm and" << intensity << "%";
        emit targetSelected(target,mode);
    }
}

