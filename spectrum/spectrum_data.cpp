#include "spectrum_data.h"
#include <opencv2/imgproc/imgproc.hpp>

SpectrumData::SpectrumData()
{
    mSpecWidth = 0;
    mGraphColor = Qt::white;
}

SpectrumData::SpectrumData(cv::Mat specLine, int id, int cflx1, int cflx2,
                           IntensityConfiguration* intensityConfig)
{
    double r,g,b;
    mID = id;
    mCFLX1 = cflx1;
    mCFLX2 = cflx2;
    mSpecLine.create(1,specLine.cols,CV_8UC3);

    for (int col = 0; col < specLine.cols; col++)
    {
        r=0;
        g=0;
        b=0;

        for (int row = 0; row < specLine.rows; row++)
        {
            b += (double) specLine.at<cv::Vec3b>(row,col)[0] / (double) 255;
            g += (double) specLine.at<cv::Vec3b>(row,col)[1] / (double) 255;
            r += (double) specLine.at<cv::Vec3b>(row,col)[2] / (double) 255;
        }
        r /= specLine.rows;
        g /= specLine.rows;
        b /= specLine.rows;
        mData.append(SpecPoint(col,r,g,b));
        mSpecLine.at<cv::Vec3b>(0,col)[0] = b*255;
        mSpecLine.at<cv::Vec3b>(0,col)[1] = g*255;
        mSpecLine.at<cv::Vec3b>(0,col)[2] = r*255;
    }

    mSpecWidth = specLine.cols;

    configure(cflx1, 436.6, cflx2, 611.6);

    if (intensityConfig != nullptr && !intensityConfig->empty())
    {
        for (int i = 0; i < mData.length(); i++)
        {
            mData[i].configuredIntensity *=
                    intensityConfig->wavelength2factor(mData[i].wavelength);
        }
    }
}

SpectrumData SpectrumData::operator-(const SpectrumData d)
{
    SpectrumData res;
    int i;

    for (i = 0; i < mData.length() && i < d.mData.length(); i++)
    {
        //TOOD: wavelength
        res.mData.append(SpecPoint(i,
                                   mData[i].r - d.mData[i].r,
                                   mData[i].g - d.mData[i].g,
                                   mData[i].b - d.mData[i].b));
        res.mData[i].wavelength = mData[i].wavelength;
        res.mData[i].configuredIntensity =
                mData[i].configuredIntensity - d.mData[i].configuredIntensity;
    }
    res.mID = mID;
    res.mSpecWidth = i;
    res.mFunctionM = mFunctionM;
    res.mFunctionN = mFunctionN;
    res.mCFLX1 = mCFLX1;
    res.mCFLX2 = mCFLX2;

    return res;
}

SpectrumData SpectrumData::operator+(const SpectrumData d)
{
    SpectrumData res;
    int i;

    for (i = 0; i < mData.length() && i < d.mData.length(); i++)
    {
        //TOOD: wavelength
        res.mData.append(SpecPoint(i,
                                   mData[i].r + d.mData[i].r,
                                   mData[i].g + d.mData[i].g,
                                   mData[i].b + d.mData[i].b));
        res.mData[i].wavelength = mData[i].wavelength;
        res.mData[i].configuredIntensity =
                mData[i].configuredIntensity + d.mData[i].configuredIntensity;
    }
    res.mID = mID;
    res.mSpecWidth = i;
    res.mSpecLine = d.mSpecLine;
    res.mFunctionM = mFunctionM;
    res.mFunctionN = mFunctionN;
    res.mCFLX1 = mCFLX1;
    res.mCFLX2 = mCFLX2;

    return res;
}

SpectrumData SpectrumData::operator*(const double factor)
{
    SpectrumData res;
    int i;

    for (i = 0; i < mData.length(); i++)
    {
        //TOOD: wavelength
        res.mData.append(SpecPoint(i,
                                   mData[i].r * factor,
                                   mData[i].g * factor,
                                   mData[i].b * factor));
        res.mData[i].wavelength = mData[i].wavelength;
        res.mData[i].configuredIntensity =
                mData[i].configuredIntensity * factor;
    }
    res.mID = mID;
    res.mSpecLine = mSpecLine;
    res.mSpecWidth = i;
    res.mFunctionM = mFunctionM;
    res.mFunctionN = mFunctionN;
    res.mCFLX1 = mCFLX1;
    res.mCFLX2 = mCFLX2;

    return res;
}

void SpectrumData::configure(int x1, float wavelength1, int x2, float wavelength2)
{
    if (x2 == x1)
        return;

    mFunctionM = (double)(wavelength2 - wavelength1) / (double)(x2 - x1);
    mFunctionN = (double) wavelength1 - mFunctionM * (double) x1;

    for (int i = 0; i < mData.length(); i++)
    {
        mData[i].wavelength = x2w(mData[i].x);
    }
}

double SpectrumData::normalizeIntensity(double lastScale, double smooth)
{
    double maxIntensity = 0;
    foreach (SpecPoint p, mData)
    {
        if (p.intensity > maxIntensity)
            maxIntensity = p.intensity;
    }
    if (maxIntensity <= 0.0)
        return 1.0;

    double scale = 1/maxIntensity;
    if (lastScale != -1)
        scale = lastScale*smooth + scale*(1-smooth);
    if (scale >= 5)
        scale = 5;

    for (int i = 0; i < mData.length(); i++)
    {
        mData[i].intensity = mData[i].intensity * scale;
    }
    return scale;
}

void SpectrumData::scaleSpecLine(int x)
{
    if (x <= 0 || mSpecLine.empty())
        return;
    cv::resize(mSpecLine,mSpecLine,cv::Size(x,1));
}

double SpectrumData::x2w(double x)
{
    return mFunctionM * x + mFunctionN;
}

double SpectrumData::w2x(double w)
{
    return (w - mFunctionN) / mFunctionM;
}

double SpectrumData::w2I(double w)
{
    if (mData.isEmpty())
        return 0;

    bool smaller = w < mData.first().wavelength;
    SpecPoint* p1;
    SpecPoint* p2;
    double slope;
    double intensity;

    //w not within spectrum data
    if (smaller)
        return 0;

    for (int i = 1; i < mData.length(); i++)
    {
        if (smaller != w < mData[i].wavelength)
        {
            smaller = !smaller;
            p1 = &mData[i-1];
            p2 = &mData[i];
            break;
        }
    }

    //w not within spectrum data
    if (!smaller)
        return 0;

    slope = (p2->intensity - p1->intensity)/(p2->wavelength - p1->wavelength);
    intensity = p1->intensity + slope * (w - p1->wavelength);

    return intensity;
}

QStringList SpectrumData::exportCSV()
{
    QStringList csv;
    for (int i = 0; i < mData.length(); i++)
    {
        csv.append(QString::number(mData[i].wavelength) + ";" + QString::number(mData[i].intensity));
    }
    return csv;
}

QList<MeasurementTarget> SpectrumData::targets() const
{
    return mTargets;
}

void SpectrumData::setTargets(const QList<MeasurementTarget> &targets)
{
    mTargets = targets;
}
