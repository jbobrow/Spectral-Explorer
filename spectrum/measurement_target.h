#ifndef MEASUREMENTTARGET_H
#define MEASUREMENTTARGET_H

#include <QString>

class MeasurementTarget
{
public:
    MeasurementTarget(float wavelength, float intensity) {
        this->wavelength = wavelength;
        this->intensity = intensity;
    }
    operator QString() const { return QString("Target(%1, %2)").arg(wavelength).arg(intensity); }

    float wavelength;
    float intensity;
};

#endif // MEASUREMENTTARGET_H
