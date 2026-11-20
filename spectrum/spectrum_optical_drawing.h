#ifndef SPECTRUMOPTICALDRAWING_H
#define SPECTRUMOPTICALDRAWING_H

#include <QList>
#include "spectrum/spectrum_data.h"
#include "spectrum/spectrum_drawing_strategy.h"

class SpectrumOpticalDrawing : public SpectrumDrawingStrategy
{
public:
    SpectrumOpticalDrawing(QList<SpectrumData>* data);
    ~SpectrumOpticalDrawing();
    void draw(QPainter *painter, QRect evRect);
    void drawMouseAxis(QPainter* painter, QRect evRect, int x, int y);

private:
    QList<SpectrumData>* mFloatingData;
    QImage mImg;
};

#endif // SPECTRUMOPTICALDRAWING_H
