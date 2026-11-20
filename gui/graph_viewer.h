#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QOpenGLWidget>
#include <QPaintEvent>
#include "spectrum/measurement_target.h"
#include "spectrum/spectrum_data.h"
#include "spectrum/spectrum_drawing_strategy.h"

class GraphViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GraphViewer(QWidget *parent = nullptr);

signals:
    void sizeChanged();
    void newCFLX(int x);
    void targetSelected(MeasurementTarget target, int mode);

public slots:
    void showSpectrumGraph(SpectrumDrawingStrategy *dr, bool deleteStrategy = true);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    SpectrumDrawingStrategy* mSpectrumDrawing = nullptr;
    int mMouseX = 0;
    int mMouseY = 0;

};

#endif // GRAPHVIEWER_H
