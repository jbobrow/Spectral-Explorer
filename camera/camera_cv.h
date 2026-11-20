#ifndef CAMERACV_H
#define CAMERACV_H

#include "camera/camera.h"
#include "camera/capture_worker.h"

class CameraCV : public Camera
{
    Q_OBJECT
public:
    CameraCV(int devID);
    CameraCV(int devID, cv::VideoCapture *capture);
    ~CameraCV();

    virtual void start();
    virtual void setActive(bool a);
    virtual bool isVirtual();
    virtual bool isTimedOut();

    virtual bool exposureAvailable() const;
    virtual bool gainAvailable() const;
    virtual bool brightnessAvailable() const;
    virtual bool sharpnessAvailable() const;
    bool checkCaptureWorker() const;

public slots:
    virtual void setExposure(int exp);
    virtual void setGain(int gain);
    virtual void setBrightness(int br);
    virtual void setSharpness(int sh);
    virtual void setAverageFrames(int num);

signals:
    void startCaptureThread();
    void setCaptureThreadActive(bool a);

private slots:
    void onNewWorkerFrame();

private:
    int mDevID;
    QThread *mCaptureThread = nullptr;
    CaptureWorker* mCaptureWorker = nullptr;
    cv::VideoCapture *mCapture = nullptr;
};

#endif // CAMERACV_H
