#ifndef SPECTRUMDATA_H
#define SPECTRUMDATA_H

#include <QList>
#include <QColor>
#include <opencv2/core/core.hpp>
#include "spectrum/intensity_configuration.h"
#include "spectrum/measurement_target.h"

class IntensityConfiguration;

class SpecPoint {
public:
    SpecPoint(int x, double r, double g, double b) {
        this->x = x;
        this->intensity = (r+g+b)/3;
        this->configuredIntensity = this->intensity;
        this->r = r;
        this->g = g;
        this->b = b;
    }

    double x, r, g, b;
    double intensity, wavelength, configuredIntensity;
};


class SpectrumData
{
public:
    SpectrumData();
    SpectrumData( cv::Mat specLine, int id, int cflx1, int cflx2,
                  IntensityConfiguration* intensityConfig);
    SpectrumData operator-(const SpectrumData d);
    SpectrumData operator+(const SpectrumData d);
    SpectrumData operator*(const double factor);

    QList<MeasurementTarget> targets() const;
    void setTargets(const QList<MeasurementTarget> &targets);

    void configure(int x1, float wavelength1, int x2, float wavelength2);
    double normalizeIntensity(double lastScale = -1, double smooth = 0);
    void scaleSpecLine(int x);
    double x2w(double x);
    double w2x(double w);
    double w2I(double w);
    QStringList exportCSV();

    double mFunctionM, mFunctionN;
    QList<SpecPoint> mData;
    cv::Mat mSpecLine;
    int mSpecWidth;
    int mCFLX1, mCFLX2;
    int mID;
    QColor mGraphColor;

private:
    QList<MeasurementTarget> mTargets;
};

#endif // SPECTRUMDATA_H
