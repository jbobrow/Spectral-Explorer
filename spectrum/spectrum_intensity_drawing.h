#ifndef SPECTRUMINTENSITYDRAWING_H
#define SPECTRUMINTENSITYDRAWING_H

#include "spectrum_drawing_strategy.h"

class SpectrumIntensityDrawing : public SpectrumDrawingStrategy
{
public:
    SpectrumIntensityDrawing(SpectrumData data);
    void addSpectrumData(SpectrumData data);
    void removeSpectrumData(int id);
    void draw(QPainter *painter, QRect evRect);

protected:
    QList<SpectrumData> mGraphs;
};

#endif // SPECTRUMINTENSITYDRAWING_H
