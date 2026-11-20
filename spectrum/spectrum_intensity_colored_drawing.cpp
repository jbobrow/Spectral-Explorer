#include "spectrum_intensity_colored_drawing.h"
#include <QDebug>

SpectrumIntensityColoredDrawing::SpectrumIntensityColoredDrawing(SpectrumData data) :
    SpectrumDrawingStrategy(data)
{
    mOffsetLeft = 60;
    mOffsetRight = 55;
    mOffsetBottom = 30;
    mOffsetTop = 30;
    mDrawTargets = true;
}

void SpectrumIntensityColoredDrawing::draw(QPainter *painter, QRect evRect)
{
    if (mData.mSpecWidth <= 0 || mData.mData.isEmpty() || evRect.width() <= 0)
    {
        painter->fillRect(evRect, QBrush(mBackgroundColor));
        return;
    }

    float scale = (float)evRect.width() / (float)mData.mSpecWidth;
    int effectiveHeight = evRect.height() - mOffsetTop - mOffsetBottom;
    effectiveHeight *= mEffectiveHeightScale;

    painter->save();
    QPainterPath path;
    QPen pen(mGraphColor);
    pen.setWidth(mGraphWidth);
    QLinearGradient gradient(QPointF(0, 0), QPointF(evRect.width(), 0));
    painter->setPen(pen);

    painter->fillRect(evRect, QBrush(mBackgroundColor));
    if (mDrawAxis) drawAxis(painter, evRect);

    painter->translate(0,evRect.height()-mOffsetBottom);
    painter->scale(1,-1);

    path.moveTo(mOffsetLeft,0);
    foreach (SpecPoint p, mData.mData) {
        if (p.x*scale >= mOffsetLeft && p.x*scale <= evRect.width()-mOffsetRight)
        {
            path.lineTo(p.x*scale,p.intensity*effectiveHeight);
            gradient.setColorAt((p.x*scale)/evRect.width(),QColor(p.r*255,p.g*255,p.b*255,mColorOpacity*255));
        }
    }
    //fix odd results with gradient when the path's y-coordinate is not 0
    path.lineTo(evRect.width(),0);

    painter->setBrush(gradient);
    painter->drawPath(path);
    painter->restore();
}

qreal SpectrumIntensityColoredDrawing::colorOpacity() const
{
    return mColorOpacity;
}

void SpectrumIntensityColoredDrawing::setColorOpacity(const qreal &colorOpacity)
{
    mColorOpacity = colorOpacity;
}

qreal SpectrumIntensityColoredDrawing::effectiveHeightScale() const
{
    return mEffectiveHeightScale;
}

void SpectrumIntensityColoredDrawing::setEffectiveHeightScale(const qreal &effectiveHeightScale)
{
    mEffectiveHeightScale = effectiveHeightScale;
}

