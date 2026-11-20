#ifndef SPECTRUMRGBDRAWING_H
#define SPECTRUMRGBDRAWING_H

#include "spectrum_drawing_strategy.h"

class SpectrumRGBDrawing : public SpectrumDrawingStrategy
{
public:
    SpectrumRGBDrawing(SpectrumData data);
    void draw(QPainter *painter, QRect evRect);

protected:
    QColor mGraphR, mGraphG, mGraphB;
};

#endif // SPECTRUMRGBDRAWING_H
