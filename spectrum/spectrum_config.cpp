#include "spectrum_config.h"
#include <QSettings>
#include <QDebug>
#include "math.h"

SpectrumConfig::SpectrumConfig(Camera* cam)
{
    qDebug() << "[SpecConf] Create spectrum config for " << cam->getCamID();
    mCamera = cam;
    mIntensityConfig = new IntensityConfiguration();
    load();
}

SpectrumConfig::~SpectrumConfig()
{
    delete mIntensityConfig;
    mIntensityConfig = nullptr;
}

SpectrumConfig::SpectrumConfig(const SpectrumConfig& rhs)
{
    mCamera = rhs.mCamera;
    mSpectrumRow = rhs.mSpectrumRow;
    mExposure = rhs.mExposure;
    mGain = rhs.mGain;
    mFilterEnabled = rhs.mFilterEnabled;
    mAverageFrames = rhs.mAverageFrames;
    mCFLX1 = rhs.mCFLX1;
    mCFLX2 = rhs.mCFLX2;
    mLeftOffset = rhs.mLeftOffset;
    mRightOffset = rhs.mRightOffset;
    mIntensityConfig = new IntensityConfiguration(*rhs.mIntensityConfig);
}

void SpectrumConfig::save(QString file)
{
    QSettings *settings;
    if (file == "")
        settings = new QSettings;
    else
        settings = new QSettings(file,QSettings::IniFormat);

    settings->beginGroup("Camera");
    settings->setValue("Exposure",mExposure);
    settings->setValue("Gain",mGain);
    settings->setValue("AverageFrames",mAverageFrames);
    settings->endGroup();

    settings->beginGroup("WavelengthConfiguration");
    settings->setValue("CameraSpectrumRow",mSpectrumRow);
    settings->setValue("CFLX1",mCFLX1+mLeftOffset);
    settings->setValue("CFLX2",mCFLX2+mLeftOffset);
    settings->setValue("LeftOffset",mLeftOffset);
    settings->setValue("RightOffset",mRightOffset);
    settings->endGroup();
    delete settings;

    mIntensityConfig->save(file);
}

void SpectrumConfig::load(QString file)
{
    qDebug() << "[SpecConf] Load config" << file;
    QSettings *settings;
    if (file == "")
        settings = new QSettings;
    else
        settings = new QSettings(file,QSettings::IniFormat);

    settings->beginGroup("Camera");
    setExposure(settings->value("Exposure",mExposure).toInt());
    setGain(settings->value("Gain",mGain).toInt());
    setAverageFrames(settings->value("AverageFrames",mAverageFrames).toInt());
    settings->endGroup();

    settings->beginGroup("WavelengthConfiguration");
    setSpectrumRow(settings->value("CameraSpectrumRow",
                                  mCamera->getHeight()/2).toInt());
    mLeftOffset = settings->value("LeftOffset",mLeftOffset).toInt();
    mRightOffset = settings->value("RightOffset",mRightOffset).toInt();
    mCFLX1 = settings->value("CFLX1",mCFLX1Default).toInt()-mLeftOffset;
    mCFLX2 = settings->value("CFLX2",mCFLX2Default).toInt()-mLeftOffset;
    settings->endGroup();
    delete settings;    

    mIntensityConfig->load(file);
}

//---------------------------------------------------------------------------------------
// Spectrum row
//---------------------------------------------------------------------------------------
void SpectrumConfig::setSpectrumRow(int row)
{
    if (mCamera->rowInRange(row))
    {
        mSpectrumRow = row;
    }
    else
    {
        qWarning() << "[SpecConf]" << Q_FUNC_INFO << "Row not in range.";
    }
}

//---------------------------------------------------------------------------------------
// CFL
//---------------------------------------------------------------------------------------
void SpectrumConfig::setCFLX(int x1, int x2)
{
    mCFLX1 = x1;
    mCFLX2 = x2;
}

//---------------------------------------------------------------------------------------
// Spectrum Offset
//---------------------------------------------------------------------------------------
void SpectrumConfig::setLeftOffset(int o)
{
    // recalculate cfl config pixels because they depend on left offset
    int cflDiff = mLeftOffset - o;
    mCFLX1 += cflDiff;
    mCFLX2 += cflDiff;

    mLeftOffset = o;
    mCamera->requestFrame();
}

void SpectrumConfig::setRightOffset(int o)
{
    mRightOffset = o;
    mCamera->requestFrame();
}

//---------------------------------------------------------------------------------------
// Camera Settings
//---------------------------------------------------------------------------------------
void SpectrumConfig::setExposure(int exp)
{
    qDebug() << "[SpecConf] Set exposure" << exp;
    mExposure = exp;
    mCamera->setExposure(exp);
}

void SpectrumConfig::setGain(int gain)
{
    mGain = gain;
    mCamera->setGain(gain);
}

void SpectrumConfig::updateCameraProp()
{
    qDebug() << "[SpecConf] Update camera properties.";
    setExposure(mExposure);
    setGain(mGain);
    setFilterEnabled(mFilterEnabled);
    setAverageFrames(getAverageFrames());
}

void SpectrumConfig::setAverageFrames(int num)
{
    mAverageFrames = num;
    mCamera->setAverageFrames(getAverageFrames());
}

void SpectrumConfig::setFilterEnabled(bool e)
{
    mFilterEnabled = e;
    mCamera->setAverageFrames(getAverageFrames());
}

//---------------------------------------------------------------------------------------
// intensity configuration
//---------------------------------------------------------------------------------------
void SpectrumConfig::configureIntensity(SpectrumData specData, int temperature)
{
    mIntensityConfig->configure(specData, temperature);
}
