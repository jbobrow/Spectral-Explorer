#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <opencv2/highgui/highgui.hpp>

class CameraDisposer;

class Camera : public QObject
{
    Q_OBJECT
public:
    /// Child class should call this constructor.
    Camera(QVariant camID);
    ~Camera();

    virtual void start() = 0;
    virtual void setActive(bool a);
    virtual bool isVirtual() = 0;
    virtual bool isTimedOut();
    bool isActive() {return mActive;}

    virtual bool exposureAvailable() const;
    virtual bool gainAvailable() const;
    virtual bool brightnessAvailable() const;
    virtual bool sharpnessAvailable() const;

    bool rowInRange(int r);
    int getHeight();            //don't use mFrame.rows directly because it isn't accessible at startup
    int getWidth();
    cv::Mat getFrame();
    QVariant getCamID() const;
    void setCamID(const QVariant &camID);

public slots:
    void requestFrame();

    virtual void setExposure(int exp) {}
    virtual void setGain(int gain) {}
    virtual void setBrightness(int br) {}
    virtual void setSharpness(int sh) {}
    virtual void setAverageFrames(int num) {}

    void rotate90();
    void resetRotation();

signals:
    void newFrame(cv::Mat frame);
    void cameraSizeChanged(int w, int h);
    void cameraTimeout();

protected slots:
    void updateFrame(cv::Mat frame);
    void rotate_image_90n(cv::Mat &src, cv::Mat &dst, int angle);

protected:
    cv::Mat mFrame;
    QVariant mCamID;

    bool mActive = false;
    bool mIsStarted = false;

    int mOldW, mOldH;
    int mHeight = 0;
    int mWidth = 0;
    int mAngle = 0;

};

#endif // CAMERA_H
