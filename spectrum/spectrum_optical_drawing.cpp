#include "spectrum_optical_drawing.h"
#include <QImage>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

SpectrumOpticalDrawing::SpectrumOpticalDrawing(QList<SpectrumData>* data) :
    SpectrumDrawingStrategy((data != nullptr && !data->isEmpty()) ? data->at(0) : SpectrumData())
{
    mFloatingData = data;
    mOffsetLeft = 0;
    mOffsetRight = 0;
    mOffsetBottom = 0;
    mOffsetTop = 0;
}

SpectrumOpticalDrawing::~SpectrumOpticalDrawing()
{
}

void SpectrumOpticalDrawing::draw(QPainter *painter, QRect evRect)
{
    painter->fillRect(evRect, QBrush(mBackgroundColor));

    if (mFloatingData == nullptr || mFloatingData->isEmpty() ||
        evRect.width() <= 0 || mFloatingData->at(0).mSpecLine.empty())
        return;

//    if (mImg.width() != evRect.width() || mImg.height()!= evRect.height())
//    {
//        mImg = QImage(evRect.width(), evRect.height(), QImage::Format_RGB888);
//        mImg.fill(Qt::black);
//    }

    cv::Mat tmp(1,evRect.width(),CV_8UC3);
    if (mFloatingData->at(0).mSpecLine.cols != evRect.width())
        (*mFloatingData)[0].scaleSpecLine(evRect.width());
    tmp = (mFloatingData->at(0).mSpecLine);
    for (int r = 1; r < mFloatingData->length(); r++)
    {
        if (mFloatingData->at(r).mSpecLine.cols != evRect.width())
            (*mFloatingData)[r].scaleSpecLine(evRect.width());
        tmp.push_back(mFloatingData->at(r).mSpecLine);
    }
    cv::flip(tmp,tmp,0);

    mImg = QImage((const unsigned char*)(tmp.data),
                          tmp.cols, tmp.rows,
                          tmp.step, QImage::Format_RGB888).rgbSwapped();
    painter->drawImage(0,0,mImg);
}

void SpectrumOpticalDrawing::drawMouseAxis(QPainter *painter, QRect evRect, int x, int y)
{
}
