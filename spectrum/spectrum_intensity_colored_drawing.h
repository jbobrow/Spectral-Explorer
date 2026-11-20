#ifndef SPECTRUMINTENSITYCOLOREDDRAWING_H
#define SPECTRUMINTENSITYCOLOREDDRAWING_H

#include "spectrum_drawing_strategy.h"

class SpectrumIntensityColoredDrawing : public SpectrumDrawingStrategy
{
public:
    SpectrumIntensityColoredDrawing(SpectrumData data);
    void draw(QPainter *painter, QRect evRect);

    qreal colorOpacity() const;
    void setColorOpacity(const qreal &colorOpacity);

    qreal effectiveHeightScale() const;
    void setEffectiveHeightScale(const qreal &effectiveHeightScale);

private:
    qreal mColorOpacity = 1.0;
    qreal mEffectiveHeightScale = 1.0;
};

#endif // SPECTRUMINTENSITYCOLOREDDRAWING_H
