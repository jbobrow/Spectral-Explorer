#ifndef SPECTRUMDRAWINGSTRATEGY_H
#define SPECTRUMDRAWINGSTRATEGY_H

#include "spectrum_data.h"
#include <QPainter>
#include <QColor>
#include <QRect>

class SpectrumDrawingStrategy
{
public:
    SpectrumDrawingStrategy(SpectrumData data);
    virtual ~SpectrumDrawingStrategy();

    virtual void draw(QPainter* painter, QRect evRect) = 0;
    virtual void drawMouseAxis(QPainter* painter, QRect evRect, int x, int y);
    void drawConfigLines(QPainter* painter, QRect evRect);
    void drawCFLLines(QPainter* painter, QRect evRect);
    void drawFraunhoferLines(QPainter* painter, QRect evRect);
    void drawAxis(QPainter* painter, QRect evRect);
    void drawTargets(QPainter* painter, QRect evRect);
    void drawTitle(QPainter* painter, QRect evRect);
    void setTitle(QString title);
    void setBackgroundColor(QColor back);
    void setGraphColor(QColor c);
    void setGraphWidth(int w);
    void setWavelengthSteps(int stepWidth) {mWavelengthSteps = stepWidth;}
    void setAxisEnabled(bool e) {mDrawAxis = e;}
    void setMouseAxisEnabled(bool e) {mDrawMouseAxis = e;}
    void setConfigLinesEnabled(bool e) {mDrawConfigLines = e;}
    void setCFLLinesEnabled(bool e) {mDrawCFLLines = e;}
    void setFraunhoferLinesEnabled(bool e) {mDrawFraunhoferLines = e;}
    void setDrawTargets(bool drawTargets) {mDrawTargets = drawTargets;}

    SpectrumData getData() {return mData;}
    int getOffsetBottom() {return mOffsetBottom;}
    int getOffsetTop() {return mOffsetTop;}

    void setOffset(int top, int right, int bottom, int left);

    int intensityToY(float intensity, int height);
    int wavelengthToX(float wavelength, int width);
    float yToIntensity(int y, int height);
    float xToWavelength(int x, int width);

protected:
    SpectrumData mData;
    QString mTitle;
    QColor mBackgroundColor;
    QColor mGraphColor;
    QColor mAxisColor;
    QColor mCFL1LineColor;
    QColor mCFL2LineColor;
    QColor mMouseAxisColor;
    QColor mTargetColor;
    QColor mTargetColor2;
    QColor mTextColor;
    int mGraphWidth;
    int mCFLLineWidth;
    int mFontSize;
//    int mOffsetLabeled;
//    int mOffsetUnlabeled;
    int mOffsetTop;
    int mOffsetBottom;
    int mOffsetLeft;
    int mOffsetRight;
    int mWavelengthSteps = 50;
    bool mDrawConfigLines = false;
    bool mDrawCFLLines = false;
    bool mDrawFraunhoferLines = false;
    bool mDrawAxis = true;
    bool mDrawMouseAxis = true;
    bool mDrawTargets = false;
};

#endif // SPECTRUMDRAWINGSTRATEGY_H
