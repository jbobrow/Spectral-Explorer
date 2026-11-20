#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

#include <QString>
#include <QList>
#include <QSettings>
#include "camera/camera.h"
#include "spectrum/spectrum_data.h"
#include "spectrum/intensity_configuration.h"

class SpectrumConfig
{
public:
    SpectrumConfig(Camera *cam);
    SpectrumConfig(const SpectrumConfig& rhs);
    ~SpectrumConfig();

    SpectrumConfig* clone();
    void save(QString file = "");
    void load(QString file = "");
    static QSettings getSettingsFromFile(QString file);

    void setSpectrumRow(int row);
    void setCFLX(int x1, int x2);
    void setExposure(int exp);
    void setGain(int gain);
    void setAverageFrames(int num);
    void setFilterEnabled(bool e);
    void setLeftOffset(int o);
    void setRightOffset(int o);
    void updateCameraProp();

    int getSpectrumRow() {return mSpectrumRow;}
    int getExposure() {return mExposure;}
    int getGain() {return mGain;}
    int getCFLX1() {return mCFLX1;}
    int getCFLX2() {return mCFLX2;}
    int getLeftOffset() {return mLeftOffset;}
    int getRightOffset() {return mRightOffset;}
    Camera* getCamera() {return mCamera;}
    int getAverageFrames() { return mFilterEnabled ? mAverageFrames : 1; }
    int getRawAverageFrames() { return mAverageFrames; }

    IntensityConfiguration* getIntensityConfig() {return mIntensityConfig;}
    int getBlackBodyTemp() {return mIntensityConfig->getBlackBodyTemp();}
    void configureIntensity(SpectrumData specData, int temperature);

private:
    Camera* mCamera = nullptr;

    int mSpectrumRow = 0;
    int mExposure = 6;
    int mGain = 255;
    const int mCFLX1Default = 510;
    const int mCFLX2Default = 860;
    int mCFLX1;
    int mCFLX2;
    int mLeftOffset = 380;
    int mRightOffset = 250;
    int mAverageFrames = 10;
    bool mFilterEnabled = true;
    IntensityConfiguration *mIntensityConfig = nullptr;
};

#endif // CAMERACONFIG_H
