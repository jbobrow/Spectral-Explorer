#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class CaptureWorker : public QObject
{
    Q_OBJECT
public:
    explicit CaptureWorker(cv::VideoCapture *capture, int id, QObject *parent = nullptr);
    ~CaptureWorker();

    cv::Mat getFrame();
    int getTimoutCnt();

    bool cvPropAvailable(int propId);

public slots:
    void start();
    void setAverageFrames(int num);
    void setCvProp(int propId, int value);
    void setActive(bool active);

private slots:
    void capture();

signals:
    void newFrame();
    void cameraTimeout();

private:
    void run();
    cv::Mat averageFrames();

    cv::VideoCapture *mCapture = nullptr;
    int mID;
    QList<cv::Mat> mFrameQueue;
    cv::Mat mPublishedFrame;
    QMutex mMutexCapture;
    QMutex mMutexFrame;
    QMutex mMutexTimeout;
    QMutex mMutexTimer;
    QMutex mMutexAvFrames;
    QTime *mTime = nullptr;
    int mTimeoutCnt = 0;
    QTimer *mTimer = nullptr;
    int mAverageFrames = 3;
};

#endif // CAPTURETHREAD_H
