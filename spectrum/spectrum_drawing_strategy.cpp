#include "spectrum_drawing_strategy.h"
#include <QDebug>

SpectrumDrawingStrategy::SpectrumDrawingStrategy(SpectrumData data) :
    mData(data)
{
    mTitle = "";
    mBackgroundColor = QColor(68,68,68);
    mGraphColor = QColor(255,255,255);
    mAxisColor = QColor(128,128,128);
    mCFL1LineColor = QColor(0,0,255);
    mCFL2LineColor = QColor(255,0,0);
    mTextColor = QColor(255,255,255);
    mMouseAxisColor = QColor(255,255,0);
    mTargetColor = QColor(255,0,0);
    mTargetColor2 = QColor(255,0,0,80);
    mGraphWidth = 1;
    mCFLLineWidth = 1;
    mFontSize = 10;
}

SpectrumDrawingStrategy::~SpectrumDrawingStrategy()
{
}

void SpectrumDrawingStrategy::setTitle(QString title)
{
    mTitle = title;
}

void SpectrumDrawingStrategy::setBackgroundColor(QColor back)
{
    mBackgroundColor = back;
}

void SpectrumDrawingStrategy::setGraphColor(QColor c)
{
    mGraphColor = c;
}

void SpectrumDrawingStrategy::setGraphWidth(int w)
{
    mGraphWidth = w;
}

void SpectrumDrawingStrategy::setOffset(int top, int right, int bottom, int left)
{
    mOffsetTop = top;
    mOffsetRight = right;
    mOffsetBottom = bottom;
    mOffsetLeft = left;
}

int SpectrumDrawingStrategy::intensityToY(float intensity, int height)
{
    int effectiveHeight = height - mOffsetTop - mOffsetBottom;
    return height - intensity*effectiveHeight - mOffsetBottom;
}

int SpectrumDrawingStrategy::wavelengthToX(float wavelength, int width)
{
    float scale = (float)width / (float)mData.mSpecWidth;
    return mData.w2x(wavelength)*scale;
}

float SpectrumDrawingStrategy::yToIntensity(int y, int height)
{
    int effectiveHeight = height - mOffsetTop - mOffsetBottom;
    return (float)(height - mOffsetBottom - y) / (float) effectiveHeight;
}

float SpectrumDrawingStrategy::xToWavelength(int x, int width)
{
    float scale = (float)width / (float)mData.mSpecWidth;
    return mData.x2w(x/scale);
}

void SpectrumDrawingStrategy::drawMouseAxis(QPainter* painter, QRect evRect, int x, int y)
{
    if (!mDrawMouseAxis) return;

    painter->save();
    QPen axis = QPen(mMouseAxisColor);
    QPen text = QPen(mMouseAxisColor);
    QFont font = painter->font() ;
    text.setWidth(1);
    axis.setWidth(1);
    font.setPointSize(mFontSize);
    painter->setFont(font);

    //draw mouse axis
    if (x > mOffsetLeft && x < evRect.width() - mOffsetRight)
    {
        float w = xToWavelength(x,evRect.width());
        painter->setPen(text);
        painter->drawText(x - 20, mOffsetTop/1.5, QString::number(w,'f',1) + QString(" nm"));
        painter->setPen(axis);
        painter->drawLine(x, mOffsetTop , x, evRect.height()-mOffsetBottom);
    }

    if (y > mOffsetTop && y < evRect.height() - mOffsetBottom)
    {
        float i = yToIntensity(y,evRect.height()) * 100;
        painter->setPen(text);
        painter->drawText(evRect.width()-mOffsetRight+10, y+5, QString::number(i,'f',1) + QString(" %"));
        painter->setPen(axis);
        painter->drawLine(mOffsetLeft, y,evRect.width()-mOffsetRight, y);
    }
    painter->restore();
}

void SpectrumDrawingStrategy::drawAxis(QPainter* painter, QRect evRect)
{
    painter->save();
    QPen axis = QPen(mAxisColor);
    QPen text = QPen(mTextColor);
    QFont font = painter->font() ;
    text.setWidth(1);
    axis.setWidth(1);
    font.setPointSize(mFontSize);
    painter->setFont(font);

    //draw intensity axis
    for (float intensity = 0; intensity <= 1; intensity += 0.25)
    {
        int py = intensityToY(intensity,evRect.height());
        painter->setPen(text);
        painter->drawText(10, py+5, QString::number(intensity*100) + QString(" %"));
        painter->setPen(axis);
        painter->drawLine(mOffsetLeft, py,evRect.width()-mOffsetRight, py);
    }

    //draw wavelength axis
    for (int w = 0; w <= 1000; w += mWavelengthSteps)
    {
        int px = wavelengthToX(w,evRect.width());
        if (px < mOffsetLeft + 20 || px >= evRect.width() - mOffsetRight - 20)
            continue;
        painter->setPen(text);
        painter->drawText(px - 20, evRect.height() - mOffsetBottom/3, QString::number(w) + QString(" nm"));
        painter->setPen(axis);
        painter->drawLine(px, mOffsetTop , px, evRect.height()-mOffsetBottom);
    }

    //draw spectral lines for cfl
    drawCFLLines(painter,evRect);
    drawFraunhoferLines(painter,evRect);
    drawTargets(painter,evRect);

    painter->restore();
}

void SpectrumDrawingStrategy::drawTargets(QPainter *painter, QRect evRect)
{
    int targetSize = 10;

    painter->save();
    QPen target = QPen(mTargetColor);
    QPen targetExtension = QPen(mTargetColor2);
    QPen text = QPen(mTargetColor);
    QFont font = painter->font() ;
    text.setWidth(1);
    target.setWidth(2);
    targetExtension.setWidth(1);
    font.setPointSize(mFontSize);
    painter->setFont(font);

    const QList<MeasurementTarget> &targets = mData.targets();
    QList<QList<const MeasurementTarget*>> targetLvlsW;
    QList<QList<const MeasurementTarget*>> targetLvlsI;
    int distX = 58;
    int distY = 16;
    for (int t = 0; t < targets.length(); t++)
    {
        qDebug() << "Draw target" << t;
        int x = wavelengthToX(targets[t].wavelength,evRect.width());
        int y = intensityToY(targets[t].intensity,evRect.height());
        // Draw target
        if (x > mOffsetLeft && x < evRect.width() - mOffsetRight &&
            y > mOffsetTop && y < evRect.height() - mOffsetBottom)
        {
            //Calculate level
            int lvlW;
            for (lvlW = 0; lvlW < 6; lvlW++) {
                targetLvlsW.append(QList<const MeasurementTarget*>());
                if (lvlW == 5) {
                    targetLvlsW[lvlW].append(&targets[t]);
                    break;
                }
                bool incLvl = false;
                for (int j = 0; j < targetLvlsW[lvlW].size(); j++) {
                    int xComp = wavelengthToX(targetLvlsW[lvlW][j]->wavelength,evRect.width());
                    if (abs(x-xComp) < distX) {
                        incLvl = true;
                        break;
                    }
                }
                if (!incLvl) {
                    targetLvlsW[lvlW].append(&targets[t]);
                    break;
                }
            }
            int lvlI;
            for (lvlI = 0; lvlI < 6; lvlI++) {
                targetLvlsI.append(QList<const MeasurementTarget*>());
                if (lvlI == 5) {
                    targetLvlsI[lvlI].append(&targets[t]);
                    break;
                }
                bool incLvl = false;
                for (int j = 0; j < targetLvlsI[lvlI].size(); j++) {
                    int yComp = intensityToY(targetLvlsI[lvlI][j]->intensity,evRect.height());
                    if (abs(y-yComp) < distY) {
                        incLvl = true;
                        break;
                    }
                }
                if (!incLvl) {
                    targetLvlsI[lvlI].append(&targets[t]);
                    break;
                }
            }



            // Wavelength
            float w = targets[t].wavelength;
            painter->setPen(text);
            painter->drawText(x - 20, mOffsetTop/1.5 + lvlW*distY, QString::number(w,'f',1)+QString(" nm"));
            painter->setPen(targetExtension);
            painter->drawLine(x, mOffsetTop + lvlW*distY , x, y - targetSize);
            painter->setPen(target);
            painter->drawLine(x, y - targetSize , x, y + targetSize);
            painter->setPen(targetExtension);
            painter->drawLine(x, y + targetSize , x, evRect.height()-mOffsetBottom);

            // Intensity
            float i = targets[t].intensity*100;
            painter->setPen(text);
            painter->drawText(evRect.width()-mOffsetRight+10 - lvlI*44, y+5, QString::number(i,'f',1) + QString(" %"));
            painter->setPen(targetExtension);
            painter->drawLine(mOffsetLeft, y,x-targetSize, y);
            painter->setPen(target);
            painter->drawLine(x-targetSize, y,x+targetSize, y);
            painter->setPen(targetExtension);
            painter->drawLine(x+targetSize, y,evRect.width()-mOffsetRight - lvlI*42, y);
        }
    }
    painter->restore();
}

struct Peak
{
    float wavelength;
    QString element;
};

void SpectrumDrawingStrategy::drawCFLLines(QPainter *painter, QRect evRect)
{
    if (!mDrawCFLLines)
        return;

    Peak peaks[] = {Peak{405.4,"Hg"},
                       {436.6,"Hg"},
                       {487.7,"Tb3+"},
                       {542.4,"Tb3+"},
                       {546.5,"Hg"},
                       {577.0,"Hg"},
                       {579.1,"Hg"},
                       {580.0,"Tb3+"},
                       {584.0,"Tb3+"},
                       {587.6,"Eu3+"},
                       {593.4,"Eu3+"},
                       {599.7,"Eu3+"},
                       {611.6,"Eu3+"},
                       {625.7,"Tb3+"},
                       {631.1,"Eu3+"},
                       {650.8,"Eu3+"},
                       {662.6,"Eu3+"}};
    bool hgText = false, tbText = false, euText = false, drawText = false;

    painter->save();
    for (Peak p : peaks)
    {
        int px = wavelengthToX(p.wavelength,evRect.width());
        if (px < mOffsetLeft + 20 || px >= evRect.width() - mOffsetRight - 20)
            continue;
        if (p.element == "Hg") {
            painter->setPen(QPen(QColor(32,32,128)));
            if (!hgText) {
                drawText = true;
                hgText = true;
            }
        }
        else if (p.element == "Eu3+") {
            painter->setPen(QPen(QColor(128,32,32)));
            if (!euText) {
                drawText = true;
                euText = true;
            }
        }
        else if (p.element == "Tb3+") {
            painter->setPen(QPen(QColor(32,128,32)));
            if (!tbText) {
                drawText = true;
                tbText = true;
            }
        }

        if (drawText)
            painter->drawText(px-6, mOffsetTop/1.5, p.element);
        drawText = false;
        painter->drawLine(px, mOffsetTop , px, evRect.height()-mOffsetBottom);
    }
    painter->restore();
}

void SpectrumDrawingStrategy::drawFraunhoferLines(QPainter *painter, QRect evRect)
{
    if (!mDrawFraunhoferLines)
        return;

    Peak peaks[] = {Peak
//                    {656.3,"H"},
//                    {627.7,"O2"},
                    {589.5,"D: Na"},
//                    {546.1,"Hg"},
//                    {537.0,""},
                    {527.0,"E: Fe"},
                    {518.4,"b"},
                    {517.3,/*"b: Fe/Mg"*/""},
                    {516.8,""},
//                    {507.0,""},
//                    {495.8,"Fe"},
                    {486.1,"F: H"},
//                    {466.8,"Fe"},
//                    {438.4,"Fe"},
//                    {434.0,"H"},
                    {430.8,"G: Ca/Fe"}};

    painter->save();
    for (Peak p : peaks)
    {
        int px = wavelengthToX(p.wavelength,evRect.width());
        if (px < mOffsetLeft + 20 || px >= evRect.width() - mOffsetRight - 20)
            continue;

        painter->setPen(QPen(QColor(32,128,32)));

        painter->drawText(px-6, mOffsetTop/1.5, p.element);
        painter->drawLine(px, mOffsetTop , px, evRect.height()-mOffsetBottom);
    }
    painter->restore();
}

void SpectrumDrawingStrategy::drawConfigLines(QPainter *painter, QRect evRect)
{
    if (!mDrawConfigLines)
        return;

    painter->save();
    QPen line = QPen(mCFL1LineColor);
    line.setWidth(mCFLLineWidth);
    painter->setPen(line);

    //draw cfl lines
    float scale = (float)evRect.width() / (float)mData.mSpecWidth;
    int px = mData.mCFLX1*scale;
    if (px > mOffsetLeft + 20 || px <= evRect.width() - mOffsetRight - 20)
        painter->drawLine(px, mOffsetTop , px, evRect.height()-mOffsetBottom);
    line.setColor(mCFL2LineColor);
    painter->setPen(line);
    px = mData.mCFLX2*scale;
    if (px > mOffsetLeft + 20 || px <= evRect.width() - mOffsetRight - 20)
        painter->drawLine(px, mOffsetTop , px, evRect.height()-mOffsetBottom);

    painter->restore();
}

void SpectrumDrawingStrategy::drawTitle(QPainter *painter, QRect evRect)
{
    if (mTitle != "")
    {
        painter->save();
        painter->setPen(Qt::white);
        painter->drawText(10,20,mTitle);
        painter->restore();
    }
}
