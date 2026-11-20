#include "spectrum_rgb_drawing.h"

SpectrumRGBDrawing::SpectrumRGBDrawing(SpectrumData data) :
    SpectrumDrawingStrategy(data)
{
    mGraphR = Qt::red;
    mGraphG = Qt::green;
    mGraphB = Qt::blue;

    mOffsetLeft = 60;
    mOffsetRight = 55;
    mOffsetBottom = 30;
    mOffsetTop = 30;
    mDrawTargets = true;
}

void SpectrumRGBDrawing::draw(QPainter *painter, QRect evRect)
{
    if (mData.mSpecWidth <= 0 || mData.mData.isEmpty())
    {
        painter->fillRect(evRect, QBrush(mBackgroundColor));
        return;
    }

    float scale = (float)evRect.width() / (float)mData.mSpecWidth;
    int effectiveHeight = evRect.height() - mOffsetTop - mOffsetBottom;

    painter->save();
    QPainterPath path;
    QPen pen(mGraphColor);
    pen.setWidth(mGraphWidth);
    painter->setPen(pen);

    painter->fillRect(evRect, QBrush(mBackgroundColor));
    if (mDrawAxis) drawAxis(painter, evRect);

    painter->translate(0,evRect.height()-mOffsetBottom);
    painter->scale(1,-1);

    //red
    path.moveTo(mOffsetLeft,mData.mData[0].r*effectiveHeight);
    foreach (SpecPoint p, mData.mData) {
        if (p.x*scale >= mOffsetLeft && p.x*scale <= evRect.width()-mOffsetRight)
            path.lineTo(p.x*scale,p.r*effectiveHeight);
    }
    pen.setColor(mGraphR);
    painter->setPen(pen);
    painter->drawPath(path);

    //green
    path = QPainterPath();
    path.moveTo(mOffsetLeft,mData.mData[0].g*effectiveHeight);
    foreach (SpecPoint p, mData.mData) {
        if (p.x*scale >= mOffsetLeft && p.x*scale <= evRect.width()-mOffsetRight)
            path.lineTo(p.x*scale,p.g*effectiveHeight);
    }
    pen.setColor(mGraphG);
    painter->setPen(pen);
    painter->drawPath(path);

    //blue
    path = QPainterPath();
    path.moveTo(mOffsetLeft,mData.mData[0].b*effectiveHeight);
    foreach (SpecPoint p, mData.mData) {
        if (p.x*scale >= mOffsetLeft && p.x*scale <= evRect.width()-mOffsetRight)
            path.lineTo(p.x*scale,p.b*effectiveHeight);
    }
    pen.setColor(mGraphB);
    painter->setPen(pen);
    painter->drawPath(path);
    painter->restore();
}
