#include "spectrum_intensity_drawing.h"
#include <QDebug>

SpectrumIntensityDrawing::SpectrumIntensityDrawing(SpectrumData data) :
    SpectrumDrawingStrategy(data)
{
    mOffsetLeft = 60;
    mOffsetRight = 55;
    mOffsetBottom = 30;
    mOffsetTop = 30;
    mDrawTargets = true;
}

void SpectrumIntensityDrawing::addSpectrumData(SpectrumData data)
{
    mData = data;
    for (int i = 0; i < mGraphs.length(); i++)
    {
        if (mGraphs[i].mID == data.mID)
        {
            mGraphs.replace(i,data);
            return;
        }
    }
    mGraphs.append(data);
}

void SpectrumIntensityDrawing::removeSpectrumData(int id)
{
    for (int i = 0; i < mGraphs.length(); i++)
    {
        if (mGraphs[i].mID == id)
        {
            mGraphs.removeAt(i);
            return;
        }
    }
}

void SpectrumIntensityDrawing::draw(QPainter *painter, QRect evRect)
{
    int effectiveHeight = evRect.height() - mOffsetTop - mOffsetBottom;

    painter->save();

//    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillRect(evRect, QBrush(mBackgroundColor));
    if (mData.mSpecWidth <= 0 || effectiveHeight <= 0)
    {
        painter->restore();
        return;
    }

    if (mDrawAxis) drawAxis(painter, evRect);

    painter->translate(0,evRect.height()-mOffsetBottom);
    painter->scale(1,-1);

    //use mData (last graph) for scale and w2x, so all graphs have
    //the same scale and w2x conversion
    float scale = (float)evRect.width() / (float)mData.mSpecWidth;
    for (SpectrumData d : mGraphs)
    {
        if (d.mData.isEmpty())
            continue;
        QPainterPath path;
        QPen pen(d.mGraphColor);
        pen.setWidth(mGraphWidth);
        painter->setPen(pen);

        path.moveTo(mOffsetLeft,d.mData[0].configuredIntensity*effectiveHeight);
        foreach (SpecPoint p, d.mData) {
            int px = mData.w2x(p.wavelength) * scale;
            if (px >= mOffsetLeft && px <= evRect.width()-mOffsetRight)
                path.lineTo(px,p.configuredIntensity*effectiveHeight);
        }
        painter->drawPath(path);
    }
    painter->restore();
}
