#include "capture_worker.h"
#include "settings.h"

#include <QDebug>
#include <QThread>
#include <QTime>

CaptureWorker::CaptureWorker(cv::VideoCapture *capture, int id, QObject *parent) : QObject(parent)
{
    mID = id;
    if (capture == nullptr)
    {
        qWarning() << "[CapWor]" << "CaptureWorker::CaptureWorker" << mID << "*capture is nullptr.";
    }
    mMutexCapture.lock();
    mCapture = capture;
    mMutexCapture.unlock();
}

CaptureWorker::~CaptureWorker()
{
    qDebug() << "[CapWor]" << "CaptureWorker shutdown" << mID;
    mMutexTimer.lock();
    if (mTimer != nullptr)
    {
        if (QThread::currentThread() == mTimer->thread())
            mTimer->stop();
        delete mTimer;
        mTimer = nullptr;
    }
    mMutexTimer.unlock();
    delete mTime;
    mTime = nullptr;

    mMutexCapture.lock();
    if (mCapture == nullptr)
         qCritical() << "[CapWor]" << "CaptureWorker shutdown: mCapture is nullptr" << mID;
    else
    {
        mMutexTimeout.lock();
        // This if is important, because otherwise the app crashes when disconnecting camera.
        if (mTimeoutCnt == 0)
            mCapture->set(CV_CAP_PROP_EXPOSURE,-1);
        mMutexTimeout.unlock();
        mCapture->release();
        delete mCapture;
        mCapture = nullptr;
    }
    mMutexCapture.unlock();
}

void CaptureWorker::start()
{
    qDebug() << "[CapWor]" << "CaptureWorker start" << mID;

    mMutexTimer.lock();
    mTimer = new QTimer();
    connect(mTimer, SIGNAL(timeout()), this, SLOT(capture()));
    mTimer->setInterval(1000/60);
    mMutexTimer.unlock();

    qDebug() << "[CapWor]" << "CaptureWorker started" << mID;
}

void CaptureWorker::setActive(bool active)
{
    qDebug() << "[CapWor]" << "CaptureWorker"  << mID << "setActive" << active;
    mMutexTimer.lock();
    if (active) {
        mTimer->start();
    }
    else {
        mTimer->stop();
    }
    mMutexTimer.unlock();
}

//-------------------------------------------------------------------------------------------------
//Camera handling
//-------------------------------------------------------------------------------------------------
void CaptureWorker::capture()
{
//    qDebug() << "[CapWork] capture" << mID;

    cv::Mat temp;
    bool camTimeout = false;

    if (mTime == nullptr)
    {
        mTime = new QTime();
        mTime->start();
    }

    mMutexCapture.lock();
    if (mCapture->grab())
    {
        if (!mCapture->retrieve(temp))
        {
            qDebug() << "[CapWor]" << "mCapture->retrieve returned false." << mID;
            camTimeout = true;
        }
    }
    else
    {
        qDebug() << "[CapWor]" << "mCapture->grab returned false." << mID;
        camTimeout = true;
    }
    mMutexCapture.unlock();

    mMutexTimeout.lock();
    if (camTimeout) {
        mTimeoutCnt++;
        qDebug() << "[CapWor]" << "Number of camera timeouts: " << mTimeoutCnt << "ID:" << mID;
        // Sometimes the first timeout is only temporary. Don't bother the user.
        if (mTimeoutCnt > 1) emit cameraTimeout();
    }
    else
        mTimeoutCnt = 0;
    mMutexTimeout.unlock();

    if (temp.cols > 0 && temp.rows > 0)
    {
        mFrameQueue.append(temp);
        //don't publish frames too often so that the ui isn't
        if (mTime->elapsed() >= 1000/30)
        {
            mMutexFrame.lock();
            mPublishedFrame = averageFrames().clone();
            mMutexFrame.unlock();
            emit newFrame();
            mTime->restart();
        }

        mMutexAvFrames.lock();
        int maxQueue = qMax(1, mAverageFrames);
        while (mFrameQueue.length() >= maxQueue)
            mFrameQueue.removeFirst();
        mMutexAvFrames.unlock();
    }
}

cv::Mat CaptureWorker::averageFrames()
{
    cv::Mat avgFrame;
    avgFrame = cv::Mat::zeros(mFrameQueue[0].rows,mFrameQueue[0].cols,CV_32FC3);
    for (int i = 0; i < mFrameQueue.length(); i++)
    {
        if (mFrameQueue[i].size != avgFrame.size || mFrameQueue[i].channels() != avgFrame.channels()) {
            qWarning() << "[CapWor]" << "accumulate skipped queue " << i << "ID:" << mID;
            continue;
        }

        cv::accumulate(mFrameQueue[i],avgFrame);
    }
    avgFrame = avgFrame / mFrameQueue.length();
    avgFrame.convertTo(avgFrame,CV_8UC3);

    return avgFrame;
}

cv::Mat CaptureWorker::getFrame()
{
    QMutexLocker locker(&mMutexFrame);
    return mPublishedFrame;
}

int CaptureWorker::getTimoutCnt()
{
    QMutexLocker locker(&mMutexTimeout);
    return mTimeoutCnt;
}

//-------------------------------------------------------------------------------------------------
// Camera Settings
//-------------------------------------------------------------------------------------------------
void CaptureWorker::setCvProp(int propId, int value)
{
    QMutexLocker locker(&mMutexCapture);
    QMutexLocker locker2(&mMutexTimeout);
    if (mTimeoutCnt > 0)
        return;
    mCapture->set(propId,value);
}

bool CaptureWorker::cvPropAvailable(int propId)
{
    QMutexLocker locker(&mMutexCapture);
    QMutexLocker locker2(&mMutexTimeout);
    if (mTimeoutCnt > 0)
        return false;
    return static_cast<int>(mCapture->get(propId)) != 0;
}

void CaptureWorker::setAverageFrames(int num)
{
    mMutexAvFrames.lock();
    mAverageFrames = qMax(1, num);
    mMutexAvFrames.unlock();
}
