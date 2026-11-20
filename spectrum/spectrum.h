#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include "camera/camera.h"
#include "spectrum/spectrum_config.h"
#include "spectrum/spectrum_data.h"
#include "spectrum/measurement_target.h"


class Spectrum : public QObject
{
    Q_OBJECT
public:
    explicit Spectrum(QString file, QObject *parent = nullptr);
    explicit Spectrum(int cameraDevice, QObject *parent = nullptr);
    ~Spectrum();
    QString mName;
    int mID;

    void setFloatingSpecSize(int w, int h);

    Camera* getCamera() {return mCamera;}
    SpectrumConfig* getConfig() {return mConfig;}
    SpectrumData getSpectrumData() {return mSpectrumData;}
    void setConfig(SpectrumConfig* config);

public slots:
    void setActive(bool a);
    void setFreeze(bool f) {setActive(!f);}
    bool isActive();
    void onTargetSelected(MeasurementTarget targ, int mode);

signals:
    void newSpectrumData(SpectrumData data);
    void spectrumSizeChanged(int w);

private:
    void initCam(QString name);
    SpectrumData combineData(QList<SpectrumData> data);

private slots:
    void onNewFrame(cv::Mat frame);
    void onCameraSizeChanged(int w,int h);

private:
    double mFloatingSpecR;
    int mFloatingSpecW;
    int mFloatingSpecH;
    SpectrumConfig* mConfig = nullptr;
    Camera* mCamera = nullptr;
    cv::Mat mSpectrumLine;
    cv::Mat mFloatingSpectrum;
    QList<SpectrumData> mTempData;
    SpectrumData mSpectrumData;
    QList<MeasurementTarget> mTargets;
};

#endif // SPECTRUM_H
