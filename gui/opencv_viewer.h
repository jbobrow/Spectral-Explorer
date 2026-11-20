#ifndef OPENCVVIEWER_H
#define OPENCVVIEWER_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class OpenCVViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OpenCVViewer(QWidget *parent = nullptr);

signals:
    void    imageSizeChanged( int outW, int outH );
    void    cvImageClicked(int x, int y);
    void    sizeChanged();

public slots:
    bool    showImage( cv::Mat image, int specRow ); /// Used to set the image to be viewed

protected:
    void    mousePressEvent(QMouseEvent* event);
    void    paintEvent(QPaintEvent *event);
    void    resizeEvent(QResizeEvent *event);

private:
    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown
    int         mOrigWidth;
    int         mOrigHeight;
    int         mSpecRow;

    QColor      mBackgroundColor;
};

#endif // OPENCVVIEWER_H
