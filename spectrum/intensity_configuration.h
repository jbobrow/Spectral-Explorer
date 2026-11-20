#ifndef INTENSITYCONFIGURATION_H
#define INTENSITYCONFIGURATION_H

#include <QList>
#include <QString>
#include "spectrum/spectrum_data.h"

class SpectrumData;

class IntensityFactor
{
public:
    IntensityFactor(double w,double f) {
        wavelength = w;
        factor = f;
    }
    double wavelength;
    double factor;
    QString toString();
    void fromString(QString s);
};

class IntensityConfiguration
{
public:
    IntensityConfiguration();
    IntensityConfiguration(const IntensityConfiguration& rhs);
    ~IntensityConfiguration();

    void save(QString file = "");
    void load(QString file = "");

    QList<IntensityFactor> getConfig() {return mConfig;}
    int getLowerWavelength() {return mLowerWavelength;}
    int getUpperWavelength() {return mUpperWavelength;}
    int getBlackBodyTemp() {return mBlackBodyTemp;}
    double getConfigStepSize() {return mIntConfigStepSize;}
    bool empty() {return mConfig.empty();}

    void configure(SpectrumData specData, int temperature);
    double wavelength2factor(double w);

private:
    QList<IntensityFactor> mConfig;
    int mLowerWavelength = 300;
    int mUpperWavelength = 800;
    int mBlackBodyTemp = 2700;
    double mMinMeasuredIntensity = 0.05;
    double mIntConfigStepSize = 1;

    QStringList configToStringList();
    void stringListToConfig(QStringList strList);
    double blackBodyWavToInt(double wavelength);
};

#endif // INTENSITYCONFIGURATION_H
