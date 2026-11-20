#include "intensity_configuration.h"
#include <QSettings>

IntensityConfiguration::IntensityConfiguration()
{
    load();
}

IntensityConfiguration::IntensityConfiguration(const IntensityConfiguration &rhs)
{
    mConfig = rhs.mConfig;
    mLowerWavelength = rhs.mLowerWavelength;
    mUpperWavelength = rhs.mUpperWavelength;
    mBlackBodyTemp = rhs.mBlackBodyTemp;
    mIntConfigStepSize = rhs.mIntConfigStepSize;
    mMinMeasuredIntensity = rhs.mMinMeasuredIntensity;
}

IntensityConfiguration::~IntensityConfiguration()
{

}

//---------------------------------------------------------------------------------------
// loading and saving
//---------------------------------------------------------------------------------------

void IntensityConfiguration::load(QString file)
{
#ifdef DEVELOP
    QSettings *settings;
    if (file == "")
        settings = new QSettings;
    else
        settings = new QSettings(file,QSettings::IniFormat);

    settings->beginGroup("IntensityConfiguration");
    mBlackBodyTemp = settings->value("BlackBodyTemp",mBlackBodyTemp).toInt();
    mLowerWavelength = settings->value("LowerWavelength",mLowerWavelength).toInt();
    mUpperWavelength = settings->value("UpperWavelength",mUpperWavelength).toInt();
    mIntConfigStepSize = settings->value("ConfigStepSize",mIntConfigStepSize).toDouble();
    mMinMeasuredIntensity =
            settings->value("MinMeasuredIntensity",mMinMeasuredIntensity).toDouble();
    stringListToConfig(settings->value("Config",QStringList()).toStringList());
    settings->endGroup();
    delete settings;
#endif
}

void IntensityConfiguration::save(QString file)
{
#ifdef DEVELOP
    QSettings *settings;
    if (file == "")
        settings = new QSettings;
    else
        settings = new QSettings(file,QSettings::IniFormat);

    settings->beginGroup("IntensityConfiguration");
    settings->setValue("BlackBodyTemp",mBlackBodyTemp);
    settings->setValue("LowerWavelength",mLowerWavelength);
    settings->setValue("UpperWavelength",mUpperWavelength);
    settings->setValue("ConfigStepSize",mIntConfigStepSize);
    settings->setValue("MinMeasuredIntensity",mMinMeasuredIntensity);
    settings->setValue("Config",configToStringList());
    settings->endGroup();
    delete settings;
#endif
}

QStringList IntensityConfiguration::configToStringList()
{
#ifdef DEVELOP
    QStringList strList;
    foreach (IntensityFactor f, mConfig)
        strList.append(f.toString());
    return strList;
#else
    return QStringList();
#endif
}

void IntensityConfiguration::stringListToConfig(QStringList strList)
{
#ifdef DEVELOP
    mConfig.clear();
    foreach (QString s, strList)
    {
        mConfig.append(IntensityFactor(0,0));
        mConfig.last().fromString(s);
    }
#endif
}

QString IntensityFactor::toString()
{
#ifdef DEVELOP
    QString s;
    s = QString::number(wavelength) + ";" + QString::number(factor);
    return s;
#else
    return "";
#endif
}

void IntensityFactor::fromString(QString s)
{
#ifdef DEVELOP
    QStringList strList = s.split(";");
    wavelength = strList[0].toDouble();
    factor = strList[1].toDouble();
#endif
}

//---------------------------------------------------------------------------------------
// configuration
//---------------------------------------------------------------------------------------

void IntensityConfiguration::configure(SpectrumData specData, int temperature)
{
#ifdef DEVELOP
    double measuredIntensity;
    double blackBodyIntensity;
    double factor;
    mBlackBodyTemp = temperature;
    mConfig.clear();

    for (double w = mLowerWavelength; w <= mUpperWavelength; w+=mIntConfigStepSize)
    {
        measuredIntensity = specData.w2I(w);
        if (measuredIntensity == 0)
           mConfig.append(IntensityFactor(w,0));
        else
        {
            blackBodyIntensity = blackBodyWavToInt(w);
            factor = blackBodyIntensity/measuredIntensity;
            if (measuredIntensity < mMinMeasuredIntensity)
                factor = 1;
            mConfig.append(IntensityFactor(w,factor/pow(10,12)/1.5));
        }
    }
#endif
}

double IntensityConfiguration::wavelength2factor(double w)
{
#ifdef DEVELOP
    bool smaller = w < mConfig.first().wavelength;
    IntensityFactor* p1;
    IntensityFactor* p2;
    double slope;
    double factor;

    //w not within spectrum data
    if (smaller)
        return 0;

    for (int i = 1; i < mConfig.length(); i++)
    {
        if (smaller != w < mConfig[i].wavelength)
        {
            smaller = !smaller;
            p1 = &mConfig[i-1];
            p2 = &mConfig[i];
            break;
        }
    }

    //w not within spectrum data
    if (!smaller)
        return 0;

    slope = (p2->factor - p1->factor)/(p2->wavelength - p1->wavelength);
    factor = p1->factor + slope * (w - p1->wavelength);

    return factor;
#else
    return 0.0;
#endif
}

double IntensityConfiguration::blackBodyWavToInt(double wavelength)
{
#ifdef DEVELOP
    double intensity;
    double wavelengthNM = wavelength*pow(10,-9);
    double c1 = 3.74177118 * pow(10,-16);
    double c2 = 0.0143877516;
    intensity = (c1 / pow(wavelengthNM,5)) *
                (1 / (exp(c2/(wavelengthNM*mBlackBodyTemp)) - 1));
    return intensity;
#else
    return 0.0;
#endif
}
